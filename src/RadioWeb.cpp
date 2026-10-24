#include "RadioWeb.h"
#include "RadioWebPage.h"
#include "StationCsv.h"
#include <esp_system.h>
#include <lwip/sockets.h>
#include <errno.h>
#include <algorithm>
#include <cctype>

std::string radioJson(const std::string& value) {
    std::string out = "\"";
    const char* hex = "0123456789abcdef";
    for (unsigned char c : value) {
        if (c == '"' || c == '\\') { out += '\\'; out += c; }
        else if (c < 32) { out += "\\u00"; out += hex[c >> 4]; out += hex[c & 15]; }
        else out += c;
    }
    return out + '"';
}

void RadioWeb::begin(Handler handler) {
    if (started_) return;
    handler_ = handler;
    char token[33];
    snprintf(token, sizeof(token), "%08lx%08lx%08lx%08lx",
        (unsigned long)esp_random(), (unsigned long)esp_random(),
        (unsigned long)esp_random(), (unsigned long)esp_random());
    token_ = token;
    server_.begin(); server_.setNoDelay(true); started_ = true;
}

void RadioWeb::reset() {
    client_.stop();
    input_.clear(); header_.clear(); output_.clear(); body.clear();
    method.clear(); path.clear(); revision.clear(); confirm.clear();
    payload_ = nullptr; payloadSize_ = sent_ = headerSent_ = bodySize_ = 0;
    headersRead_ = responding_ = false;
}

void RadioWeb::reply(int status, const char* type, const std::string& data) {
    output_ = data;
    payload_ = output_.data(); payloadSize_ = output_.size();
    header_ = "HTTP/1.1 " + std::to_string(status) + " " +
        (status == 200 ? "OK" : status == 409 ? "Conflict" : status == 403 ? "Forbidden" : "Error") +
        "\r\nContent-Type: " + type + "\r\nContent-Length: " + std::to_string(payloadSize_) +
        "\r\nConnection: close\r\nCache-Control: no-store\r\nX-Content-Type-Options: nosniff"
        "\r\nContent-Security-Policy: default-src 'self'; script-src 'unsafe-inline'; style-src 'unsafe-inline'; connect-src 'self'; object-src 'none'; frame-ancestors 'none'"
        "\r\nReferrer-Policy: no-referrer\r\n\r\n";
    responding_ = true;
    accepted_ = activity_ = millis(); // A station connect may have taken several seconds.
}

void RadioWeb::replyPage() {
    reply(200, "text/html; charset=utf-8", "");
    payload_ = RADIO_WEB_PAGE; payloadSize_ = sizeof(RADIO_WEB_PAGE) - 1;
    const auto start = header_.find("Content-Length: ") + 16;
    header_.replace(start, header_.find("\r\n", start) - start, std::to_string(payloadSize_));
}

bool RadioWeb::parseHeaders() {
    const auto line = input_.find("\r\n");
    const auto space = input_.find(' '), next = input_.find(' ', space + 1);
    if (line == std::string::npos || space == std::string::npos || next == std::string::npos || next > line)
        return false;
    method = input_.substr(0, space); path = input_.substr(space + 1, next - space - 1);
    if (method != "GET" && method != "POST") return false;
    bool lengthSeen = false;
    std::string token, host, origin;
    size_t pos = line + 2;
    while (pos < input_.size() - 2) {
        const auto end = input_.find("\r\n", pos), colon = input_.find(':', pos);
        if (end == std::string::npos || colon == std::string::npos || colon >= end) return false;
        std::string key = input_.substr(pos, colon - pos), value = input_.substr(colon + 1, end - colon - 1);
        std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c){return std::tolower(c);});
        const auto first = value.find_first_not_of(" \t");
        value = first == std::string::npos ? "" : value.substr(first, value.find_last_not_of(" \t") - first + 1);
        if (key == "content-length") {
            if (lengthSeen || value.empty() || value.size() > 6) return false;
            lengthSeen = true; bodySize_ = 0;
            for (char c : value) {
                if (c < '0' || c > '9') return false;
                bodySize_ = bodySize_ * 10 + c - '0';
            }
            if (bodySize_ > MAX_STATION_CSV_BYTES) return false;
        } else if (key == "transfer-encoding") return false;
        else if (key == "x-radio-token") token = value;
        else if (key == "x-radio-revision") revision = value;
        else if (key == "x-radio-confirm") confirm = value;
        else if (key == "host") host = value;
        else if (key == "origin") origin = value;
        pos = end + 2;
    }
    const std::string ip = WiFi.localIP().toString().c_str();
    if ((host != ip && host != ip + ":80") ||
        (!origin.empty() && origin != "http://" + ip && origin != "http://" + ip + ":80") ||
        (method == "POST" && (token != token_.c_str() || !lengthSeen))) {
        reply(403, "text/plain; charset=utf-8", "Bu isteğe izin verilmedi. Sayfayı cihazın IP adresinden açın.");
        return true;
    }
    if (method == "GET" && bodySize_) return false;
    headersRead_ = true;
    return true;
}

void RadioWeb::loop() {
    if (!started_) return;
    if (client_.fd() < 0) {
        client_ = server_.available();
        if (client_.fd() < 0) return;
        accepted_ = activity_ = millis();
    }
    if (millis() - activity_ > 3000 || millis() - accepted_ > 15000) { reset(); return; }
    if (responding_) {
        const bool headers = headerSent_ < header_.size();
        const char* data = headers ? header_.data() + headerSent_ : payload_ + sent_;
        const size_t remaining = headers ? header_.size() - headerSent_ : payloadSize_ - sent_;
        if (!remaining) { reset(); return; }
        const int written = ::send(client_.fd(), data, std::min(remaining, size_t(768)), MSG_DONTWAIT);
        if (written > 0) {
            if (headers) headerSent_ += written; else sent_ += written;
            activity_ = millis();
        } else if (written < 0 && errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR) reset();
        return;
    }
    // Limit each pass so slow uploads cannot monopolize audio.loop().
    size_t budget = 768;
    while (budget-- && client_.available()) {
        const int c = client_.read();
        if (c < 0) break;
        activity_ = millis();
        if (!headersRead_) {
            input_ += static_cast<char>(c);
            if (input_.size() > 2048) { reply(400, "text/plain", "HTTP headers too large"); return; }
            if (input_.size() >= 4 && input_.compare(input_.size() - 4, 4, "\r\n\r\n") == 0) {
                if (!parseHeaders()) reply(400, "text/plain; charset=utf-8", "Geçersiz HTTP isteği veya boyut sınırı.");
                input_.clear();
                if (responding_) return;
            }
        } else body += static_cast<char>(c);
        if (headersRead_ && body.size() == bodySize_) {
            handler_(*this);
            if (!responding_) reply(404, "text/plain", "Not found");
            return;
        }
    }
    if (!client_.connected() && !client_.available()) reset();
}
