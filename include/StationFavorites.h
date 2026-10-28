#pragma once
#include "StationStore.h"
#include <bitset>

using StationFavoriteMask = std::bitset<MAX_STATIONS>;

// Separate from settings and the station catalog: identity is the exact stream URL.
class StationFavorites {
public:
    bool load();
    bool contains(const char* url) const;
    StationFavoriteMask mask(const RadioStation* stations, size_t count) const;
    bool toggle(const RadioStation* stations, size_t count, size_t index, String& error);
private:
    std::vector<StationRecord> records_;
};
