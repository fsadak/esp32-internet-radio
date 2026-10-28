#pragma once
#include <string>
#include <vector>
#include <stddef.h>

constexpr size_t MAX_STATIONS = 128;
constexpr size_t MAX_STATION_CSV_BYTES = 12 * 1024;
struct StationRecord { std::string name, url; };
// Independent of Arduino so the exact firmware parser can be tested on a PC.
bool parseStationCsv(const std::string& csv, std::vector<StationRecord>& result,
                     std::string& error);
std::string writeStationCsv(const std::vector<StationRecord>& stations);
