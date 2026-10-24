#pragma once
#include <map>
#include <string>
#include <vector>
#include <stdint.h>
#include <cstring>

// Atomic in-memory NVS fake with deterministic failure injection.
class Preferences {
    std::string prefix_;
    bool readOnly_ = false;
    std::string key(const char* name) const { return prefix_ + "/" + name; }
public:
    inline static std::map<std::string, std::vector<uint8_t>> storage;
    inline static int failAfter = -1;
    bool begin(const char* name, bool readOnly) { prefix_ = name; readOnly_ = readOnly; return true; }
    void end() {}
    bool isKey(const char* name) const { return storage.find(key(name)) != storage.end(); }
    size_t getBytesLength(const char* name) { return storage[key(name)].size(); }
    size_t getBytes(const char* name, void* data, size_t length) {
        const auto& bytes = storage[key(name)];
        if (bytes.size() != length) return 0;
        std::memcpy(data, bytes.data(), length); return length;
    }
    size_t putBytes(const char* name, const void* data, size_t length) {
        if (readOnly_ || failAfter == 0) return 0;
        if (failAfter > 0) --failAfter;
        const auto* bytes = static_cast<const uint8_t*>(data);
        storage[key(name)] = std::vector<uint8_t>(bytes, bytes + length); return length;
    }
    uint32_t getUInt(const char* name, uint32_t fallback = 0) {
        uint32_t value; return getBytes(name, &value, sizeof(value)) ? value : fallback;
    }
    size_t putUInt(const char* name, uint32_t value) { return putBytes(name, &value, sizeof(value)); }
    uint8_t getUChar(const char* name, uint8_t fallback = 0) {
        uint8_t value; return getBytes(name, &value, sizeof(value)) ? value : fallback;
    }
    size_t putUChar(const char* name, uint8_t value) { return putBytes(name, &value, sizeof(value)); }
};
