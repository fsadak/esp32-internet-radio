#include "StationFavorites.h"
#include <Preferences.h>

bool StationFavorites::contains(const char* url) const {
    for (const auto& item : records_) if (item.url == url) return true;
    return false;
}

StationFavoriteMask StationFavorites::mask(const RadioStation* stations, size_t count) const {
    StationFavoriteMask result;
    for (size_t i = 0; i < count && i < MAX_STATIONS; ++i)
        if (contains(stations[i].url)) result.set(i);
    return result;
}

bool StationFavorites::load() {
    Preferences prefs;
    if (!prefs.begin("favorites", false)) return false;
    const size_t size = prefs.isKey("csv1") ? prefs.getBytesLength("csv1") : 0;
    std::string csv;
    if (size && size <= MAX_STATION_CSV_BYTES) {
        csv.resize(size);
        if (prefs.getBytes("csv1", &csv[0], size) != size) csv.clear();
    }
    prefs.end();
    if (csv == "name;url\n") { records_.clear(); return true; }
    std::vector<StationRecord> candidate;
    std::string error;
    if (!parseStationCsv(csv, candidate, error)) return false;
    records_.swap(candidate);
    return true;
}

bool StationFavorites::toggle(const RadioStation* stations, size_t count, size_t index, String& error) {
    if (!stations || index >= count || count > MAX_STATIONS) {
        error = "İstasyon bulunamadı.";
        return false;
    }
    const bool removing = contains(stations[index].url);
    std::vector<StationRecord> candidate;
    // Rebuild from the current catalog: refresh names and discard unavailable URLs
    // on an explicit edit, without adding periodic/automatic NVS writes.
    for (size_t i = 0; i < count; ++i)
        if (i == index ? !removing : contains(stations[i].url))
            candidate.push_back({stations[i].name, stations[i].url});
    const std::string csv = writeStationCsv(candidate);
    if (csv.size() > MAX_STATION_CSV_BYTES) {
        error = "Favoriler kaydedilemedi. Önceki favoriler korundu.";
        return false;
    }
    Preferences prefs;
    if (!prefs.begin("favorites", false)) {
        error = "Favoriler kaydedilemedi. Önceki favoriler korundu.";
        return false;
    }
    const bool saved = prefs.putBytes("csv1", csv.data(), csv.size()) == csv.size();
    prefs.end();
    if (!saved) {
        error = "Favoriler kaydedilemedi. Önceki favoriler korundu.";
        return false;
    }
    records_.swap(candidate);
    return true;
}
