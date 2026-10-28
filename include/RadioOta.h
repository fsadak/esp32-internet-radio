#pragma once
#include "RadioFirmwareImage.h"
#include <Arduino.h>
#include <esp_ota_ops.h>

class RadioOta {
public:
    size_t capacity() const;
    void checkBoot(bool healthy);
    bool begin(size_t size);
    bool write(const uint8_t* data, size_t size);
    bool finish();
    void abort();
    bool active() const { return active_; }
    bool pendingRestart() const { return pendingRestart_; }
    bool restartReady() const { return pendingRestart_ && millis() - finishedAt_ >= 2000; }
    unsigned percent() const { return total_ ? received_ * 100 / total_ : 0; }
    const std::string& error() const { return error_; }
private:
    bool fail(const char* message);
    const esp_partition_t* partition_ = nullptr;
    esp_ota_handle_t handle_ = 0;
    size_t total_ = 0, received_ = 0;
    bool active_ = false, pendingRestart_ = false;
    uint32_t finishedAt_ = 0;
    RadioFirmwareImage image_;
    std::string error_;
};
