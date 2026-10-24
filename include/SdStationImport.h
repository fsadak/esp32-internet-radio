#pragma once
#include <Arduino.h>
#include "StationCsv.h"
using SdService = void (*)();
bool listSdStationFiles(std::vector<String>& files, String& error, SdService service);
bool readSdStationFile(const String& path, std::vector<StationRecord>& result,
                       String& error, SdService service);
