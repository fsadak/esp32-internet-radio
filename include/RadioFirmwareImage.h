#pragma once
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

// Product compatibility marker, not a cryptographic signature.
constexpr const char* RADIO_FIRMWARE_ID = "fsadak.sunton-8048s070.radio.ota.v1";
class RadioFirmwareImage {
public:
    void reset() { prefixSize_ = 0; markerFound_ = false; tail_.clear(); }
    bool feed(const uint8_t* data, size_t size) {
        const size_t copy = std::min(size, sizeof(prefix_) - prefixSize_);
        memcpy(prefix_ + prefixSize_, data, copy); prefixSize_ += copy;
        if (prefixSize_ == sizeof(prefix_) && !validHeader()) return false;
        if (!markerFound_) {
            tail_.append(reinterpret_cast<const char*>(data), size);
            markerFound_ = tail_.find(RADIO_FIRMWARE_ID) != std::string::npos;
            const size_t keep = strlen(RADIO_FIRMWARE_ID) - 1;
            if (tail_.size() > keep) tail_.erase(0, tail_.size() - keep);
        }
        return true;
    }
    bool compatible() const { return prefixSize_ == sizeof(prefix_) && validHeader() && markerFound_; }
private:
    bool validHeader() const {
        // Application image (not merged flash), ESP32-S3, appended SHA-256,
        // app descriptor magic at the start of the first segment.
        return prefix_[0] == 0xe9 && prefix_[12] == 9 && prefix_[13] == 0 && prefix_[23] == 1 &&
            prefix_[32] == 0x32 && prefix_[33] == 0x54 && prefix_[34] == 0xcd && prefix_[35] == 0xab;
    }
    uint8_t prefix_[36]{};
    size_t prefixSize_ = 0;
    bool markerFound_ = false;
    std::string tail_;
};
