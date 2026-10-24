#pragma once

#include <Arduino.h>
#include <esp32_smartdisplay.h>
#include "RadioSettings.h"
#include "StationStore.h"
#include "SdMusicFiles.h"

enum class RadioAction : uint8_t {
    None, VolumeDown, VolumeUp, ToggleMute, PreviousStation, NextStation,
    SelectStation, OpenStations, CloseStations, OpenSettings, CloseSettings,
    BeginSettings, PreviewSettings, SaveSettings, CancelSettings,
    OpenImport, SelectImportFile, ConfirmImport, CancelImport,
    ImportPreviousPage, ImportNextPage, StationsPreviousPage, StationsNextPage,
    OpenEqualizer, CloseEqualizer, StartPlayback, StopPlayback,
    OpenWifi, CloseWifi, ScanWifi, ConnectWifi,
    OpenMusic, CloseMusic, SelectMusic, MusicPreviousPage, MusicNextPage,
    ToggleMusicPause, SwitchRadio, ScanMusic, ToggleRepeatOne, ToggleRepeatAll, ToggleShuffle,
    MusicParent, MusicSeekBack, MusicSeekForward
};

struct RadioCommand {
    RadioAction action = RadioAction::None;
    size_t stationIndex = 0;
    RadioSettings settings;
    String wifiSsid;
    String wifiPassword;
    String musicPath;
};

class RadioDisplay {
public:
    bool init(const RadioStation* stations, size_t count);
    void loop();
    void present(); // One-shot refresh before a blocking station connection.
    void updateVu(uint16_t level, uint16_t peak); // 0..1000, no animation.
    bool vuActive() const { return vuEnabled_ && !backlightOff_; }
    void update(const String& station, const String& track,
                const String& status, const String& genre, const String& format,
                bool wifiConnected, uint8_t volume,
                bool muted, size_t currentStation);
    bool takeCommand(RadioCommand& command);
    void configureSettings(const RadioSettings& settings);
    void applyScreenSettings(const RadioSettings& settings);
    void finishSettings(bool close, const char* message = "");
    void setVolumeLimit(uint8_t limit) { volumeLimit_ = limit; }
    void setStations(const RadioStation* stations, size_t count);
    void showImportFiles(const std::vector<String>& files, const String& message);
    void showImportPreview(const String& file, size_t count);
    void showImportMessage(const String& message);
    void closeImport();
    void openWifi(const String& savedSsid);
    void closeWifi();
    void clearWifiPassword();
    void showWifiNetworks(const std::vector<String>& names);
    void updateWifi(const String& message, bool busy, bool scanning);
    void setMusicState(bool source, bool playing, bool paused, size_t index, size_t count,
                       uint32_t elapsed, uint32_t duration, uint8_t repeat, bool shuffle,
                       const String& playingPath, bool canSeek);
    void showMusicFolder(const String& folder, const std::vector<SdMusicEntry>& entries, const String& message);

private:
    static void onButton(lv_event_t* event);
    static void onStation(lv_event_t* event);
    static void onSettingChanged(lv_event_t* event);
    static void onSettingsTab(lv_event_t* event);
    static void onEqualizer(lv_event_t* event);
    static void onWifiInput(lv_event_t* event);
    static void onWifiNetwork(lv_event_t* event);
    static void onMusicFile(lv_event_t* event);
    static void onWake(lv_event_t* event);
    static void onImportFile(lv_event_t* event);
    void refreshStationRows();
    void refreshImportRows();
    void buildImport();
    void buildSettings();
    void buildWifi();
    void buildMusic();
    void refreshMusicRows();
    void openSettings();
    void refreshSettingsLabels();
    void refreshDeviceInfo();
    void enqueue(RadioAction action, size_t stationIndex = 0);
    lv_obj_t* button(lv_obj_t* parent, const char* text, int x, int y,
                     int width, int height, RadioAction action, bool accent = false, bool localize = true);
    lv_display_t* display_ = nullptr;
    const RadioStation* stations_ = nullptr;
    size_t stationCount_ = 0;
    lv_obj_t* station_ = nullptr;
    lv_obj_t* vuTrack_ = nullptr;
    lv_obj_t* vuFill_ = nullptr;
    lv_obj_t* vuPeak_ = nullptr;
    uint16_t vuWidth_ = UINT16_MAX, vuPeakPosition_ = UINT16_MAX;
    lv_obj_t* sourceButton_ = nullptr;
    lv_obj_t* sourceCaption_ = nullptr;
    lv_obj_t* listButton_ = nullptr;
    lv_obj_t* musicPause_ = nullptr;
    lv_obj_t* musicRepeatOne_ = nullptr;
    lv_obj_t* musicRepeatAll_ = nullptr;
    lv_obj_t* musicShuffle_ = nullptr;
    lv_obj_t* musicSeekBack_ = nullptr;
    lv_obj_t* musicSeekForward_ = nullptr;
    lv_obj_t* track_ = nullptr;
    lv_obj_t* artist_ = nullptr;
    lv_obj_t* status_ = nullptr;
    lv_obj_t* genre_ = nullptr;
    lv_obj_t* format_ = nullptr;
    lv_obj_t* wifi_ = nullptr;
    lv_obj_t* volume_ = nullptr;
    lv_obj_t* stationCountLabel_ = nullptr;
    lv_obj_t* minus_ = nullptr;
    lv_obj_t* plus_ = nullptr;
    lv_obj_t* mute_ = nullptr;
    lv_obj_t* muteIcon_ = nullptr;
    lv_obj_t* previous_ = nullptr;
    lv_obj_t* next_ = nullptr;
    lv_obj_t* stationOverlay_ = nullptr;
    lv_obj_t* stationList_ = nullptr;
    size_t stationPage_ = 0;
    size_t currentStation_ = 0;
    lv_obj_t* stationPageLabel_ = nullptr;
    lv_obj_t* stationPagePrevious_ = nullptr;
    lv_obj_t* stationPageNext_ = nullptr;
    lv_obj_t* importOverlay_ = nullptr;
    lv_obj_t* importList_ = nullptr;
    lv_obj_t* importMessage_ = nullptr;
    lv_obj_t* importFileName_ = nullptr;
    lv_obj_t* importConfirm_ = nullptr;
    lv_obj_t* importCancel_ = nullptr;
    lv_obj_t* importPrevious_ = nullptr;
    lv_obj_t* importNext_ = nullptr;
    lv_obj_t* importRetry_ = nullptr;
    std::vector<String> importFiles_;
    size_t importPage_ = 0;
    lv_obj_t* settingsOverlay_ = nullptr;
    lv_obj_t* equalizerPanel_ = nullptr;
    lv_obj_t* equalizerValues_[3] = {};
    lv_obj_t* equalizerMinus_[3] = {};
    lv_obj_t* equalizerPlus_[3] = {};
    lv_obj_t* settingsPages_[5] = {};
    lv_obj_t* settingsTabs_[5] = {};
    lv_obj_t* brightnessSlider_ = nullptr;
    lv_obj_t* brightnessValue_ = nullptr;
    lv_obj_t* dimDropdown_ = nullptr;
    lv_obj_t* vuSwitch_ = nullptr;
    lv_obj_t* bootVolumeSlider_ = nullptr;
    lv_obj_t* bootVolumeValue_ = nullptr;
    lv_obj_t* maxVolumeSlider_ = nullptr;
    lv_obj_t* maxVolumeValue_ = nullptr;
    lv_obj_t* rememberSwitch_ = nullptr;
    lv_obj_t* autoPlaySwitch_ = nullptr;
    lv_obj_t* resumeSwitch_ = nullptr;
    lv_obj_t* bootStationDropdown_ = nullptr;
    lv_obj_t* settingsMessage_ = nullptr;
    lv_obj_t* deviceInfo_ = nullptr;
    lv_obj_t* languageDropdown_ = nullptr;
    lv_obj_t* wakeOverlay_ = nullptr;
    lv_obj_t* wifiOverlay_ = nullptr;
    lv_obj_t* wifiNetworks_ = nullptr;
    lv_obj_t* wifiSsidInput_ = nullptr;
    lv_obj_t* wifiPasswordInput_ = nullptr;
    lv_obj_t* wifiMessage_ = nullptr;
    lv_obj_t* wifiKeyboard_ = nullptr;
    lv_obj_t* wifiScan_ = nullptr;
    lv_obj_t* wifiConnect_ = nullptr;
    std::vector<String> wifiNames_;
    bool wifiStateValid_ = false;
    bool wifiBusy_ = false, wifiScanning_ = false, wifiNetworksEmpty_ = true;
    lv_obj_t* musicOverlay_ = nullptr;
    lv_obj_t* musicList_ = nullptr;
    lv_obj_t* musicMessage_ = nullptr;
    lv_obj_t* musicPageLabel_ = nullptr;
    lv_obj_t* musicPagePrevious_ = nullptr;
    lv_obj_t* musicPageNext_ = nullptr;
    lv_obj_t* musicParent_ = nullptr;
    lv_obj_t* musicFolderLabel_ = nullptr;
    std::vector<SdMusicEntry> musicEntries_;
    String musicPlayingPath_;
    size_t musicPage_ = 0, musicIndex_ = 0, musicCount_ = 0;
    bool musicSource_ = false;
    uint32_t musicElapsed_ = 0, musicDuration_ = 0;
    RadioSettings savedSettings_;
    RadioSettings draftSettings_;
    uint8_t normalBrightness_ = 160;
    uint8_t volumeLimit_ = 21;
    uint16_t dimSeconds_ = 120;
    bool dimmed_ = false;
    bool backlightOff_ = false;
    bool vuEnabled_ = true;
    uint32_t dimmedAt_ = 0;
    uint32_t lastDeviceUpdate_ = 0;
    RadioCommand commands_[8];
    uint8_t commandRead_ = 0;
    uint8_t commandWrite_ = 0;
    uint32_t lastTick_ = 0;
};
