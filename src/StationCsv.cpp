#include "StationCsv.h"

namespace {
bool validUtf8(const std::string& s) {
    for (size_t i = 0; i < s.size();) {
        const unsigned char c = s[i++];
        if (c < 0x80) { if (c < 0x20 || c == 0x7f) return false; continue; }
        unsigned int value; size_t extra;
        if (c >= 0xc2 && c <= 0xdf) { value = c & 31; extra = 1; }
        else if (c >= 0xe0 && c <= 0xef) { value = c & 15; extra = 2; }
        else if (c >= 0xf0 && c <= 0xf4) { value = c & 7; extra = 3; }
        else return false;
        if (i + extra > s.size()) return false;
        for (size_t j = 0; j < extra; ++j) {
            const unsigned char next = s[i++];
            if ((next & 0xc0) != 0x80) return false;
            value = (value << 6) | (next & 63);
        }
        if ((extra == 1 && value < 0x80) || (extra == 2 && value < 0x800) ||
            (extra == 3 && value < 0x10000) || value > 0x10ffff ||
            (value >= 0xd800 && value <= 0xdfff) || (value >= 0x80 && value <= 0x9f)) return false;
    }
    return true;
}
std::string trim(const std::string& s) {
    const auto begin = s.find_first_not_of(" \t");
    if (begin == std::string::npos) return "";
    return s.substr(begin, s.find_last_not_of(" \t") - begin + 1);
}
bool fields(const std::string& line, std::vector<std::string>& values) {
    size_t i = 0;
    for (;;) {
        while (i < line.size() && (line[i] == ' ' || line[i] == '\t')) ++i;
        std::string value;
        if (i < line.size() && line[i] == '"') {
            ++i; bool closed = false;
            while (i < line.size()) {
                const char c = line[i++];
                if (c != '"') value += c;
                else if (i < line.size() && line[i] == '"') { value += '"'; ++i; }
                else { closed = true; break; }
            }
            if (!closed) return false;
            while (i < line.size() && (line[i] == ' ' || line[i] == '\t')) ++i;
            if (i < line.size() && line[i] != ';') return false;
        } else {
            while (i < line.size() && line[i] != ';') {
                if (line[i] == '"') return false;
                value += line[i++];
            }
        }
        values.push_back(trim(value));
        if (values.size() > 2) return false;
        if (i == line.size()) return values.size() == 2;
        ++i;
    }
}
bool validUrl(const std::string& url) {
    const size_t start = url.compare(0, 7, "http://") == 0 ? 7 :
                         url.compare(0, 8, "https://") == 0 ? 8 : 0;
    if (!start || url.size() <= start) return false;
    const auto end = url.find_first_of("/?#", start);
    const std::string host = url.substr(start, end - start);
    if (host.empty() || host == ":" || host[0] == ':' || host.find('@') != std::string::npos) return false;
    for (unsigned char c : url) if (c <= 0x20 || c >= 0x7f || c == '"' || c == '\\') return false;
    return true;
}
std::string quote(const std::string& value) {
    std::string result = "\"";
    for (char c : value) { result += c; if (c == '"') result += '"'; }
    return result + '"';
}
}

bool parseStationCsv(const std::string& csv, std::vector<StationRecord>& result,
                     std::string& error) {
    error.clear();
    if (csv.size() > MAX_STATION_CSV_BYTES) { error = "Dosya 12 KB sınırını aşıyor."; return false; }
    std::vector<StationRecord> parsed;
    size_t offset = csv.compare(0, 3, "\xef\xbb\xbf") == 0 ? 3 : 0;
    size_t lineNumber = 0; bool header = false;
    auto fail = [&](const char* message) {
        error = "Satır " + std::to_string(lineNumber) + ": " + message; return false;
    };
    while (offset < csv.size()) {
        ++lineNumber;
        size_t end = csv.find_first_of("\r\n", offset);
        if (end == std::string::npos) end = csv.size();
        const std::string line = csv.substr(offset, end - offset);
        offset = end;
        if (offset < csv.size() && csv[offset++] == '\r' && offset < csv.size() && csv[offset] == '\n') ++offset;
        if (trim(line).empty()) continue;
        if (line.size() > 1024) return fail("Satır çok uzun.");
        std::vector<std::string> values;
        if (!fields(line, values)) return fail("İki sütun gerekli: name;url. Tırnakları kontrol edin.");
        if (!header) {
            if (values[0] != "name" || values[1] != "url") return fail("İlk satır name;url olmalı.");
            header = true; continue;
        }
        if (values[0].empty() || values[0].size() > 96) return fail("İsim boş veya 96 bayttan uzun.");
        if (!validUtf8(values[0])) return fail("İsim UTF-8 olmalı; kontrol karakteri kullanılamaz.");
        if (values[1].size() > 512 || !validUrl(values[1])) return fail("Geçerli http/https yayın adresi gerekli (en fazla 512 bayt).");
        for (const auto& station : parsed) if (station.url == values[1]) return fail("Aynı yayın adresi tekrar ediyor.");
        if (parsed.size() == MAX_STATIONS) return fail("En fazla 128 istasyon yüklenebilir.");
        parsed.push_back({values[0], values[1]});
    }
    if (!header || parsed.empty()) { error = "Dosyada en az bir istasyon olmalı."; return false; }
    result.swap(parsed); // No partial list escapes on any validation failure.
    return true;
}

std::string writeStationCsv(const std::vector<StationRecord>& stations) {
    std::string csv = "name;url\n";
    for (const auto& station : stations) csv += quote(station.name) + ";" + quote(station.url) + "\n";
    return csv;
}
