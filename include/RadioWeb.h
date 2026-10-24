#pragma once
#include <WiFi.h>
#include <string>

// One bounded, nonblocking HTTP connection, serviced by the audio/UI main loop.
class RadioWeb {
public:
    using Handler = void (*)(RadioWeb&);
    void begin(Handler handler);
    void loop();
    void reply(int status, const char* type, const std::string& body);
    void replyPage();
    const String& token() const { return token_; }
    std::string method, path, body, revision, confirm;
private:
    void reset();
    bool parseHeaders();
    WiFiServer server_{80, 2};
    WiFiClient client_;
    Handler handler_ = nullptr;
    String token_;
    std::string input_, header_, output_;
    const char* payload_ = nullptr;
    size_t payloadSize_ = 0, sent_ = 0, headerSent_ = 0, bodySize_ = 0;
    uint32_t activity_ = 0, accepted_ = 0;
    bool started_ = false, headersRead_ = false, responding_ = false;
};

std::string radioJson(const std::string& value);
