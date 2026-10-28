#pragma once
#include <stdint.h>
#include <stddef.h>

constexpr const char* RADIO_VERSION = "0.14.1";
void setStationSettingsIdentity(uint32_t identity);
enum class RadioLanguage : uint8_t { English = 0, Turkish = 1 };

struct RadioSettings {
    uint8_t brightness = 160;
    uint16_t dimSeconds = 120; // Idle delay to 50% brightness; off 15 s later. 0 disables both.
    uint8_t bootVolume = 11;
    uint8_t maxVolume = 21;
    bool rememberVolume = true;
    bool autoPlay = true;
    bool resumeStation = true;
    uint16_t bootStation = 0;
    int8_t bass = 0;
    int8_t mid = 0;
    int8_t treble = 0;
    RadioLanguage language = RadioLanguage::English;
    bool vuMeter = true;

    void validate(size_t stationCount) {
        if (language != RadioLanguage::English && language != RadioLanguage::Turkish)
            language = RadioLanguage::English;
        if (brightness < 13) brightness = 13;
        if (maxVolume > 21) maxVolume = 21;
        if (bootVolume > maxVolume) bootVolume = maxVolume;
        if (dimSeconds != 0 && dimSeconds != 30 && dimSeconds != 60 &&
            dimSeconds != 120 && dimSeconds != 300 && dimSeconds != 600) dimSeconds = 120;
        if (bootStation >= stationCount) bootStation = 0;
        if (bass < -12) bass = -12;
        if (bass > 6) bass = 6;
        if (mid < -12) mid = -12;
        if (mid > 6) mid = 6;
        if (treble < -12) treble = -12;
        if (treble > 6) treble = 6;
    }
};

bool loadRadioSettings(RadioSettings& settings, size_t stationCount);
bool saveRadioSettings(const RadioSettings& settings);
void loadLastPlayback(uint8_t& volume, size_t& station, size_t stationCount);
bool saveLastPlayback(uint8_t volume, size_t station);
