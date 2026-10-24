#include "RadioSettings.h"
#include <Preferences.h>

namespace {
uint32_t stationIdentity = 0;
struct StoredSettings {
    uint32_t version = 4;
    RadioSettings settings;
};
// Keep the original ABI layout for existing version-1 NVS blobs.
struct LegacySettings {
    uint8_t brightness;
    uint16_t dimSeconds;
    uint8_t bootVolume;
    uint8_t maxVolume;
    bool rememberVolume;
    bool autoPlay;
    bool resumeStation;
    uint16_t bootStation;
};
struct LegacyStoredSettings {
    uint32_t version;
    LegacySettings settings;
};
struct LegacySettingsV2 {
    uint8_t brightness;
    uint16_t dimSeconds;
    uint8_t bootVolume, maxVolume;
    bool rememberVolume, autoPlay, resumeStation;
    uint16_t bootStation;
    int8_t bass, mid, treble;
};
struct LegacyStoredSettingsV2 {
    uint32_t version;
    LegacySettingsV2 settings;
};
struct LegacySettingsV3 {
    uint8_t brightness;
    uint16_t dimSeconds;
    uint8_t bootVolume, maxVolume;
    bool rememberVolume, autoPlay, resumeStation;
    uint16_t bootStation;
    int8_t bass, mid, treble;
    RadioLanguage language;
};
struct LegacyStoredSettingsV3 {
    uint32_t version;
    LegacySettingsV3 settings;
};
static_assert(sizeof(LegacySettings) == 12, "Version-1 settings layout changed");
static_assert(sizeof(LegacySettingsV2) == 16, "Version-2 settings layout changed");
static_assert(sizeof(LegacySettingsV3) == 16, "Version-3 settings layout changed");
static_assert(offsetof(LegacySettingsV3, language) == 15, "Version-3 language layout changed");
static_assert(sizeof(RadioSettings) == 18, "Version-4 settings layout changed");
}

void setStationSettingsIdentity(uint32_t identity) { stationIdentity = identity; }

bool loadRadioSettings(RadioSettings& settings, size_t stationCount) {
    Preferences prefs;
    // Create the namespace on first boot; a missing saved config is normal.
    if (!prefs.begin("radio", false)) return false;
    StoredSettings stored;
    const size_t length = prefs.isKey("config") ? prefs.getBytesLength("config") : 0;
    bool found = length == sizeof(stored) &&
        prefs.getBytes("config", &stored, sizeof(stored)) == sizeof(stored) && stored.version == 4;
    if (found) settings = stored.settings;
    else if (length == sizeof(LegacyStoredSettingsV3)) {
        // V2 and V3 share the same size and common fields. Only V3's final
        // byte is a language; V2's padding must never be interpreted as one.
        LegacyStoredSettingsV3 legacy{};
        found = prefs.getBytes("config", &legacy, sizeof(legacy)) == sizeof(legacy) &&
            (legacy.version == 2 || legacy.version == 3);
        if (found) {
            const auto& old = legacy.settings;
            settings = RadioSettings{};
            settings.brightness = old.brightness; settings.dimSeconds = old.dimSeconds;
            settings.bootVolume = old.bootVolume; settings.maxVolume = old.maxVolume;
            settings.rememberVolume = old.rememberVolume; settings.autoPlay = old.autoPlay;
            settings.resumeStation = old.resumeStation; settings.bootStation = old.bootStation;
            settings.bass = old.bass; settings.mid = old.mid; settings.treble = old.treble;
            if (legacy.version == 3) settings.language = old.language;
            // New VU preference defaults to on for every legacy version.
        }
    }
    else if (length == sizeof(LegacyStoredSettings)) {
        LegacyStoredSettings legacy{};
        found = prefs.getBytes("config", &legacy, sizeof(legacy)) == sizeof(legacy) && legacy.version == 1;
        if (found) {
            const auto& old = legacy.settings;
            settings = RadioSettings{};
            settings.brightness = old.brightness;
            settings.dimSeconds = old.dimSeconds;
            settings.bootVolume = old.bootVolume;
            settings.maxVolume = old.maxVolume;
            settings.rememberVolume = old.rememberVolume;
            settings.autoPlay = old.autoPlay;
            settings.resumeStation = old.resumeStation;
            settings.bootStation = old.bootStation;
        }
    }
    const bool sameList = prefs.getUInt("configList", 0) == stationIdentity;
    prefs.end();
    if (!sameList) settings.bootStation = 0;
    settings.validate(stationCount);
    return found;
}

bool saveRadioSettings(const RadioSettings& settings) {
    Preferences prefs;
    if (!prefs.begin("radio", false)) return false;
    StoredSettings stored{};
    stored.settings = settings;
    // A single NVS blob commits the complete settings transaction.
    // Write identity last. A reset between these writes falls back to station zero.
    const bool saved = prefs.putBytes("config", &stored, sizeof(stored)) == sizeof(stored) &&
        (prefs.getUInt("configList", 0) == stationIdentity ||
         prefs.putUInt("configList", stationIdentity) == sizeof(uint32_t));
    prefs.end();
    return saved;
}

void loadLastPlayback(uint8_t& volume, size_t& station, size_t stationCount) {
    Preferences prefs;
    if (!prefs.begin("radio", true)) return;
    volume = prefs.getUChar("lastVolume", volume);
    if (prefs.getUInt("playList", 0) == stationIdentity)
        station = prefs.getUInt("lastStation", station);
    prefs.end();
    if (volume > 21) volume = 11;
    if (station >= stationCount) station = 0;
}

bool saveLastPlayback(uint8_t volume, size_t station) {
    Preferences prefs;
    if (!prefs.begin("radio", false)) return false;
    bool saved = true;
    if (prefs.getUChar("lastVolume", 255) != volume)
        saved = prefs.putUChar("lastVolume", volume) == sizeof(uint8_t);
    if (prefs.getUInt("lastStation", UINT32_MAX) != station)
        saved = (prefs.putUInt("lastStation", station) == sizeof(uint32_t)) && saved;
    if (saved && prefs.getUInt("playList", 0) != stationIdentity)
        saved = prefs.putUInt("playList", stationIdentity) == sizeof(uint32_t);
    prefs.end();
    return saved;
}
