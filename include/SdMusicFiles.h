#pragma once
#include <Arduino.h>
#include <vector>

using SdMusicService = void (*)();

// Caller must close any local audio file before remounting.
bool mountRadioSd(String& error);
struct SdMusicEntry {
    String path;
    bool directory;
};
bool validSdMusicPath(const String& path);
String sdParentFolder(const String& path);
bool listSdMusicFolder(const String& folder, std::vector<SdMusicEntry>& entries,
                       String& error, bool remount, SdMusicService service);
