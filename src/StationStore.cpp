#include "StationStore.h"
#include <Preferences.h>

StationStore::StationStore() {
    records_.push_back({"Groove Salad", "https://ice5.somafm.com/groovesalad-128-mp3"});
    rebuild();
}
void StationStore::rebuild() {
    views_.clear();
    for (const auto& record : records_) views_.push_back({record.name.c_str(), record.url.c_str()});
    // Stable identity prevents saved indices from a different list being reused.
    identity_ = 2166136261u;
    for (unsigned char c : writeStationCsv(records_)) identity_ = (identity_ ^ c) * 16777619u;
}
bool StationStore::load() {
    Preferences prefs;
    if (!prefs.begin("stations", true)) return false;
    const size_t bytes = prefs.getBytesLength("csv1");
    std::string csv;
    if (bytes && bytes <= MAX_STATION_CSV_BYTES) {
        csv.resize(bytes);
        if (prefs.getBytes("csv1", &csv[0], bytes) != bytes) csv.clear();
    }
    prefs.end();
    std::vector<StationRecord> candidate; std::string error;
    if (!parseStationCsv(csv, candidate, error)) return false;
    records_.swap(candidate); rebuild(); return true;
}
bool StationStore::replace(const std::vector<StationRecord>& candidate, String& error) {
    const std::string csv = writeStationCsv(candidate);
    // Revalidate the canonical representation (also used by a future web import).
    std::vector<StationRecord> checked; std::string detail;
    if (!parseStationCsv(csv, checked, detail)) { error = detail.c_str(); return false; }
    std::vector<RadioStation> views;
    for (const auto& record : checked) views.push_back({record.name.c_str(), record.url.c_str()});
    Preferences prefs;
    if (!prefs.begin("stations", false)) { error = "Kalıcı bellek açılamadı. Mevcut liste korundu."; return false; }
    // One NVS blob commits the complete list, without deleting the old value first.
    const bool saved = prefs.putBytes("csv1", csv.data(), csv.size()) == csv.size();
    prefs.end();
    if (!saved) { error = "Liste kaydedilemedi. Mevcut liste korundu."; return false; }
    records_.swap(checked); views_.swap(views);
    identity_ = 2166136261u;
    for (unsigned char c : csv) identity_ = (identity_ ^ c) * 16777619u;
    return true;
}
