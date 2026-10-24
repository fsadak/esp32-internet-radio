#pragma once
#include <Arduino.h>
#include "StationCsv.h"

struct RadioStation { const char* name; const char* url; };

class StationStore {
public:
    StationStore();
    bool load();
    bool replace(const std::vector<StationRecord>& candidate, String& error);
    size_t count() const { return records_.size(); }
    const RadioStation* data() const { return views_.data(); }
    const RadioStation& operator[](size_t i) const { return views_[i]; }
    uint32_t identity() const { return identity_; }
private:
    void rebuild();
    std::vector<StationRecord> records_;
    std::vector<RadioStation> views_;
    uint32_t identity_ = 0;
};
