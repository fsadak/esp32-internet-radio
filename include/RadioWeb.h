#pragma once
#include <WiFi.h>
#include <string>

// One bounded, nonblocking HTTP connection, serviced by the audio/UI main loop.
class RadioWeb {
public:
    using Handler = void (*)(RadioWeb&);
    enum class Upload { Begin, Data, End, Abort };
    using UploadHandler = bool (*)(RadioWeb&, Upload, const uint8_t*, size_t);
    void begin(Handler handler, UploadHandler uploadHandler = nullptr);
    void loop();
    void reply(int status, const char* type, const std::string& body);
    void replyPage();
    const String& token() const { return token_; }
    size_t contentLength() const { return bodySize_; }
    void deferUpload(uint32_t durationMs); // Nonblocking notice before flash writes.
    std::string method, path, body, revision, confirm;
private:
    void reset();
    bool parseHeaders();
    WiFiServer server_{80, 2};
    WiFiClient client_;
    Handler handler_ = nullptr;
    UploadHandler uploadHandler_ = nullptr;
    String token_;
    std::string input_, header_, output_;
    const char* payload_ = nullptr;
    size_t payloadSize_ = 0, sent_ = 0, headerSent_ = 0, bodySize_ = 0;
    uint32_t activity_ = 0, accepted_ = 0;
    bool started_ = false, headersRead_ = false, responding_ = false;
    bool uploading_ = false;
    bool uploadDeferred_ = false;
    uint32_t uploadDeferredAt_ = 0, uploadDelayMs_ = 0;
    size_t uploaded_ = 0;
    uint8_t uploadChunk_[4096]{}; // Keep flash-write buffers off the Arduino loop stack.
};

std::string radioJson(const std::string& value);
