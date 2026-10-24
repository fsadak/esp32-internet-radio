#include "RadioDisplay.h"
#include "RadioFonts.h"
#include "RadioLanguage.h"
#include <WiFi.h>

namespace {
constexpr uint32_t BG = 0x080808;
constexpr uint32_t PANEL = 0x161616;
constexpr uint32_t LINE = 0x292929;
constexpr uint32_t WHITE = 0xF5F5F5;
constexpr uint32_t MUTED = 0xA5A5A5;
constexpr uint32_t ORANGE = 0xFF7A00;
constexpr uint16_t VU_WIDTH = 340;

// Only our UI labels own these bindings. Metadata labels are never bound.
void releaseUiText(lv_event_t* event) {
    auto* object = static_cast<lv_obj_t*>(lv_event_get_current_target(event));
    delete static_cast<String*>(lv_obj_get_user_data(object));
    lv_obj_set_user_data(object, nullptr);
}

void setUiText(lv_obj_t* object, const char* canonical) {
    auto* source = lv_obj_has_flag(object, LV_OBJ_FLAG_USER_1) ?
        static_cast<String*>(lv_obj_get_user_data(object)) : nullptr;
    if (source && *source == canonical) return; // Language changes refresh all bound labels once.
    if (!source) {
        source = new String;
        lv_obj_set_user_data(object, source);
        lv_obj_add_flag(object, LV_OBJ_FLAG_USER_1);
        lv_obj_add_event_cb(object, releaseUiText, LV_EVENT_DELETE, nullptr);
    }
    if (*source != canonical) *source = canonical;
    const String translated = uiText(*source);
    if (strcmp(lv_label_get_text(object), translated.c_str()) != 0)
        lv_label_set_text(object, translated.c_str());
}

void refreshUiText(lv_obj_t* object) {
    if (lv_obj_has_flag(object, LV_OBJ_FLAG_USER_1)) {
        const auto* source = static_cast<String*>(lv_obj_get_user_data(object));
        const String translated = uiText(*source);
        if (strcmp(lv_label_get_text(object), translated.c_str()) != 0)
            lv_label_set_text(object, translated.c_str());
    }
    for (size_t i = 0; i < lv_obj_get_child_count(object); ++i)
        refreshUiText(lv_obj_get_child(object, i));
}

lv_obj_t* rectangle(lv_obj_t* parent, int x, int y, int width, int height,
                    uint32_t color, int radius = 0) {
    auto* object = lv_obj_create(parent);
    lv_obj_remove_style_all(object);
    lv_obj_set_pos(object, x, y);
    lv_obj_set_size(object, width, height);
    lv_obj_set_style_bg_color(object, lv_color_hex(color), 0);
    lv_obj_set_style_bg_opa(object, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(object, radius, 0);
    lv_obj_set_scrollbar_mode(object, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scrollable(object, false);
    return object;
}

lv_obj_t* label(lv_obj_t* parent, const char* text, int x, int y,
                int width, const lv_font_t* font, uint32_t color) {
    auto* object = lv_label_create(parent);
    lv_obj_remove_style_all(object);
    lv_obj_set_pos(object, x, y);
    lv_obj_set_size(object, width, lv_font_get_line_height(font) + 4);
    lv_obj_set_style_text_font(object, font, 0);
    lv_obj_set_style_text_color(object, lv_color_hex(color), 0);
    lv_label_set_long_mode(object, LV_LABEL_LONG_MODE_DOTS);
    if (text[0]) setUiText(object, text);
    else lv_label_set_text(object, text);
    return object;
}

void setText(lv_obj_t* object, const char* text) {
    // Converting a UI placeholder to real metadata must drop its translation key.
    if (lv_obj_has_flag(object, LV_OBJ_FLAG_USER_1)) {
        delete static_cast<String*>(lv_obj_get_user_data(object));
        lv_obj_set_user_data(object, nullptr);
        lv_obj_remove_flag(object, LV_OBJ_FLAG_USER_1);
        lv_obj_remove_event_cb(object, releaseUiText);
    }
    if (strcmp(lv_label_get_text(object), text) != 0) {
        lv_label_set_text(object, text);
    }
}

String musicTime(uint32_t seconds) {
    char value[24];
    snprintf(value, sizeof(value), "%02lu:%02lu", (unsigned long)(seconds / 60), (unsigned long)(seconds % 60));
    return String(value);
}
} // namespace

lv_obj_t* RadioDisplay::button(lv_obj_t* parent, const char* text,
                               int x, int y, int width, int height,
                               RadioAction action, bool accent, bool localize) {
    auto* object = lv_button_create(parent);
    lv_obj_remove_style_all(object);
    lv_obj_set_pos(object, x, y);
    lv_obj_set_size(object, width, height);
    lv_obj_set_scrollable(object, false);
    lv_obj_set_style_bg_opa(object, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(object, lv_color_hex(accent ? ORANGE : PANEL), 0);
    lv_obj_set_style_bg_color(object, lv_color_hex(ORANGE), LV_STATE_PRESSED);
    lv_obj_set_style_border_width(object, accent ? 0 : 1, 0);
    lv_obj_set_style_border_color(object, lv_color_hex(LINE), 0);
    lv_obj_set_style_radius(object, 12, 0);
    lv_obj_set_style_opa(object, LV_OPA_30, LV_STATE_DISABLED);
    lv_obj_set_style_text_color(object, lv_color_hex(accent ? BG : WHITE), 0);
    lv_obj_set_style_text_color(object, lv_color_hex(BG), LV_STATE_PRESSED);
    auto* textObject = lv_label_create(object);
    lv_obj_set_style_text_font(textObject, radioFont(18), 0);
    if (localize) setUiText(textObject, text);
    else lv_label_set_text(textObject, text);
    lv_obj_center(textObject);
    lv_obj_set_user_data(object, reinterpret_cast<void*>(static_cast<uintptr_t>(action)));
    lv_obj_add_event_cb(object, onButton, LV_EVENT_CLICKED, this);
    return object;
}

bool RadioDisplay::init(const RadioStation* stations, size_t count) {
    initRadioFonts();
    if (!stations || count == 0) {
        return false;
    }
    stations_ = stations;
    stationCount_ = count;
    smartdisplay_init();
    display_ = lv_display_get_default();
    if (!display_) {
        return false;
    }
    lv_display_set_rotation(display_, LV_DISPLAY_ROTATION_0);
    smartdisplay_lcd_set_backlight(160.0f / 255.0f);
    lastTick_ = millis();
    auto* root = lv_display_get_screen_active(display_);
    lv_obj_remove_style_all(root);
    lv_obj_set_style_bg_color(root, lv_color_hex(BG), 0);
    lv_obj_set_style_bg_opa(root, LV_OPA_COVER, 0);
    lv_obj_set_scrollable(root, false);

    label(root, LV_SYMBOL_AUDIO "  İNTERNET RADYOSU", 28, 28, 360,
          radioFont(18), ORANGE);
    vuTrack_ = rectangle(root, 28, 58, VU_WIDTH, 10, LINE);
    vuFill_ = rectangle(vuTrack_, 0, 0, 1, 10, ORANGE);
    vuPeak_ = rectangle(vuTrack_, 0, 0, 2, 10, WHITE);
    lv_obj_set_hidden(vuPeak_, true);
    for (auto* object : {vuTrack_, vuFill_, vuPeak_})
        lv_obj_remove_flag(object, LV_OBJ_FLAG_CLICKABLE);
    updateVu(0, 0);
    sourceButton_ = button(root, "SD MP3", 410, 12, 150, 56, RadioAction::OpenMusic);
    wifi_ = label(root, LV_SYMBOL_WIFI "  Bekliyor", 580, 28, 136,
                  radioFont(14), MUTED);
    button(root, LV_SYMBOL_SETTINGS, 724, 12, 56, 56, RadioAction::OpenSettings);
    rectangle(root, 28, 76, 744, 1, LINE);

    sourceCaption_ = label(root, "İSTASYON", 28, 96, 744, radioFont(14), MUTED);
    station_ = label(root, "", 28, 120, 744, radioFont(36), WHITE);
    musicRepeatOne_ = button(root, "Tekrar: 1", 238, 120, 154, 52, RadioAction::ToggleRepeatOne);
    musicRepeatAll_ = button(root, "Tekrar: tüm", 406, 120, 174, 52, RadioAction::ToggleRepeatAll);
    musicShuffle_ = button(root, LV_SYMBOL_SHUFFLE " Karışık", 594, 120, 178, 52, RadioAction::ToggleShuffle);
    for (auto* control : {musicRepeatOne_, musicRepeatAll_, musicShuffle_}) {
        lv_obj_set_style_bg_color(control, lv_color_hex(ORANGE), LV_STATE_CHECKED);
        lv_obj_set_style_text_color(control, lv_color_hex(BG), LV_STATE_CHECKED);
        lv_obj_set_style_border_color(control, lv_color_hex(ORANGE), LV_STATE_CHECKED);
        lv_obj_set_hidden(control, true);
    }
    rectangle(root, 28, 194, 3, 104, ORANGE);
    label(root, "ŞİMDİ ÇALIYOR", 46, 194, 710, radioFont(14), ORANGE);
    track_ = label(root, "", 46, 222, 710, radioFont(28), WHITE);
    lv_label_set_long_mode(track_, LV_LABEL_LONG_MODE_SCROLL_CIRCULAR);
    lv_obj_set_style_anim_duration(track_, 12000, 0);
    artist_ = label(root, "", 46, 264, 710, radioFont(18), MUTED);

    previous_ = button(root, LV_SYMBOL_PREV "  Önceki", 28, 314, 160, 56,
                       RadioAction::PreviousStation);
    listButton_ = button(root, LV_SYMBOL_LIST "  İstasyon listesi", 204, 314, 392, 56,
           RadioAction::OpenStations);
    musicPause_ = button(root, LV_SYMBOL_PAUSE, 466, 314, 130, 56, RadioAction::ToggleMusicPause, true);
    lv_obj_set_hidden(musicPause_, true);
    next_ = button(root, "Sonraki  " LV_SYMBOL_NEXT, 612, 314, 160, 56,
                   RadioAction::NextStation);

    auto* footer = rectangle(root, 0, 388, 800, 92, PANEL);
    status_ = label(footer, "", 28, 8, 248, radioFont(14), ORANGE);
    genre_ = label(footer, "Tür: —", 28, 36, 164, radioFont(14), MUTED);
    rectangle(footer, 200, 38, 1, 16, LINE);
    format_ = label(footer, "—", 212, 36, 72, radioFont(14), WHITE);
    stationCountLabel_ = label(footer, "", 28, 64, 248, radioFont(14), MUTED);
    minus_ = button(footer, LV_SYMBOL_MINUS, 300, 12, 64, 68,
                    RadioAction::VolumeDown, true);
    auto* caption = label(footer, "SES", 376, 3, 72, radioFont(14), MUTED);
    volume_ = label(footer, "11", 376, 24, 72, radioFont(36), WHITE);
    lv_obj_set_style_text_align(volume_, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_align(caption, LV_TEXT_ALIGN_CENTER, 0);
    plus_ = button(footer, LV_SYMBOL_PLUS, 460, 12, 64, 68,
                   RadioAction::VolumeUp, true);
    musicSeekBack_ = button(footer, "-10", 540, 12, 72, 68, RadioAction::MusicSeekBack);
    musicSeekForward_ = button(footer, "+10", 624, 12, 72, 68, RadioAction::MusicSeekForward);
    lv_obj_set_hidden(musicSeekBack_, true); lv_obj_set_hidden(musicSeekForward_, true);
    mute_ = button(footer, LV_SYMBOL_VOLUME_MAX, 708, 16, 64, 64,
                   RadioAction::ToggleMute);
    muteIcon_ = lv_obj_get_child(mute_, 0);

    // Overlays stay allocated so opening/closing them never disrupts playback.
    stationOverlay_ = rectangle(root, 0, 0, 800, 480, BG);
    label(stationOverlay_, "İSTASYON LİSTESİ", 28, 28, 650,
          radioFont(24), ORANGE);
    button(stationOverlay_, LV_SYMBOL_CLOSE, 716, 14, 56, 56,
           RadioAction::CloseStations);
    label(stationOverlay_, "Dinlemek için bir istasyona dokunun", 28, 79, 744,
          radioFont(14), MUTED);
    stationList_ = rectangle(stationOverlay_, 28, 106, 744, 290, BG);
    lv_obj_set_scrollable(stationList_, true);
    lv_obj_set_scroll_dir(stationList_, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(stationList_, LV_SCROLLBAR_MODE_AUTO);
    stationPagePrevious_ = button(stationOverlay_, LV_SYMBOL_LEFT, 28, 410, 96, 56, RadioAction::StationsPreviousPage);
    stationPageLabel_ = label(stationOverlay_, "", 152, 426, 460, radioFont(18), MUTED);
    stationPageNext_ = button(stationOverlay_, LV_SYMBOL_RIGHT, 676, 410, 96, 56, RadioAction::StationsNextPage);
    refreshStationRows();
    lv_obj_set_hidden(stationOverlay_, true);

    buildSettings();
    buildImport();
    wakeOverlay_ = rectangle(lv_display_get_layer_top(display_), 0, 0, 800, 480, BG);
    lv_obj_set_style_bg_opa(wakeOverlay_, LV_OPA_TRANSP, 0);
    lv_obj_set_clickable(wakeOverlay_, true);
    lv_obj_add_event_cb(wakeOverlay_, onWake, LV_EVENT_CLICKED, this);
    lv_obj_set_hidden(wakeOverlay_, true);
    return true;
}

void RadioDisplay::onButton(lv_event_t* event) {
    auto* self = static_cast<RadioDisplay*>(lv_event_get_user_data(event));
    auto* object = static_cast<lv_obj_t*>(lv_event_get_current_target(event));
    const auto action = static_cast<RadioAction>(reinterpret_cast<uintptr_t>(lv_obj_get_user_data(object)));
    switch (action) {
    case RadioAction::OpenMusic:
        self->enqueue(self->musicSource_ ? RadioAction::SwitchRadio : RadioAction::OpenMusic); break;
    case RadioAction::CloseMusic: lv_obj_set_hidden(self->musicOverlay_, true); break;
    case RadioAction::SwitchRadio:
        if (self->musicOverlay_) lv_obj_set_hidden(self->musicOverlay_, true);
        self->enqueue(RadioAction::SwitchRadio); break;
    case RadioAction::MusicPreviousPage:
        if (self->musicPage_) --self->musicPage_;
        self->refreshMusicRows(); break;
    case RadioAction::MusicNextPage:
        if ((self->musicPage_ + 1) * 5 < self->musicEntries_.size()) ++self->musicPage_;
        self->refreshMusicRows(); break;
    case RadioAction::OpenStations:
        if (self->musicSource_) { self->enqueue(RadioAction::OpenMusic); break; }
        self->stationPage_ = self->currentStation_ / 8;
        self->refreshStationRows();
        lv_obj_set_hidden(self->stationOverlay_, false); break;
    case RadioAction::CloseStations: lv_obj_set_hidden(self->stationOverlay_, true); break;
    case RadioAction::OpenSettings: self->openSettings(); break;
    case RadioAction::CloseSettings: self->enqueue(RadioAction::CancelSettings); break;
    case RadioAction::OpenEqualizer:
        self->refreshSettingsLabels();
        lv_obj_set_hidden(self->equalizerPanel_, false); break;
    case RadioAction::CloseEqualizer:
        lv_obj_set_hidden(self->equalizerPanel_, true); break;
    case RadioAction::StationsPreviousPage:
        if (self->stationPage_) --self->stationPage_;
        self->refreshStationRows(); break;
    case RadioAction::StationsNextPage:
        if ((self->stationPage_ + 1) * 8 < self->stationCount_) ++self->stationPage_;
        self->refreshStationRows(); break;
    case RadioAction::ImportPreviousPage:
        if (self->importPage_) --self->importPage_;
        self->refreshImportRows(); break;
    case RadioAction::ImportNextPage:
        if ((self->importPage_ + 1) * 4 < self->importFiles_.size()) ++self->importPage_;
        self->refreshImportRows(); break;
    default: self->enqueue(action); break;
    }
}

void RadioDisplay::onStation(lv_event_t* event) {
    auto* self = static_cast<RadioDisplay*>(lv_event_get_user_data(event));
    auto* row = static_cast<lv_obj_t*>(lv_event_get_current_target(event));
    self->enqueue(RadioAction::SelectStation,
                  reinterpret_cast<uintptr_t>(lv_obj_get_user_data(row)));
    lv_obj_set_hidden(self->stationOverlay_, true);
}

void RadioDisplay::enqueue(RadioAction action, size_t stationIndex) {
    if (action == RadioAction::PreviewSettings && commandRead_ != commandWrite_) {
        const uint8_t last = (commandWrite_ + 7) % 8;
        if (commands_[last].action == RadioAction::PreviewSettings) {
            commands_[last].settings = draftSettings_;
            return;
        }
    }
    const uint8_t next = (commandWrite_ + 1) % 8;
    if (next != commandRead_) {
        commands_[commandWrite_].action = action;
        commands_[commandWrite_].stationIndex = stationIndex;
        commands_[commandWrite_].settings = draftSettings_;
        commands_[commandWrite_].wifiSsid = action == RadioAction::ConnectWifi ? lv_textarea_get_text(wifiSsidInput_) : "";
        commands_[commandWrite_].wifiPassword = action == RadioAction::ConnectWifi ? lv_textarea_get_text(wifiPasswordInput_) : "";
        commands_[commandWrite_].musicPath = action == RadioAction::SelectMusic && stationIndex < musicEntries_.size() ? musicEntries_[stationIndex].path : String("");
        commandWrite_ = next;
    }
}

bool RadioDisplay::takeCommand(RadioCommand& command) {
    if (commandRead_ == commandWrite_) {
        return false;
    }
    command = commands_[commandRead_];
    commands_[commandRead_].wifiSsid = "";
    commands_[commandRead_].wifiPassword = "";
    commands_[commandRead_].musicPath = "";
    commandRead_ = (commandRead_ + 1) % 8;
    return true;
}

void RadioDisplay::updateVu(uint16_t level, uint16_t peak) {
    if (!vuFill_ || !vuPeak_) return;
    if (level > 1000) level = 1000;
    if (peak > 1000) peak = 1000;
    const uint16_t width = static_cast<uint32_t>(level) * VU_WIDTH / 1000;
    const uint16_t peakPosition = peak ?
        static_cast<uint32_t>(peak) * (VU_WIDTH - 2) / 1000 : UINT16_MAX;
    if (width != vuWidth_) {
        lv_obj_set_hidden(vuFill_, width == 0);
        if (width) lv_obj_set_width(vuFill_, width);
        vuWidth_ = width;
    }
    if (peakPosition != vuPeakPosition_) {
        lv_obj_set_hidden(vuPeak_, peak == 0);
        if (peak) lv_obj_set_x(vuPeak_, peakPosition);
        vuPeakPosition_ = peakPosition;
    }
}

void RadioDisplay::loop() {
    if (!display_) return;
    const uint32_t now = millis();
    lv_tick_inc(now - lastTick_);
    lastTick_ = now;
    lv_timer_handler();
    if (!dimmed_ && dimSeconds_ &&
        lv_display_get_inactive_time(display_) >= static_cast<uint32_t>(dimSeconds_) * 1000) {
        dimmed_ = true;
        backlightOff_ = false;
        dimmedAt_ = millis();
        smartdisplay_lcd_set_backlight(normalBrightness_ / 510.0f);
        lv_obj_move_foreground(wakeOverlay_);
        lv_obj_set_hidden(wakeOverlay_, false);
    } else if (dimmed_ && !backlightOff_ && millis() - dimmedAt_ >= 15000) {
        backlightOff_ = true;
        smartdisplay_lcd_set_backlight(0.0f);
    }
    if (!lv_obj_is_hidden(settingsOverlay_) && now - lastDeviceUpdate_ >= 1000) {
        lastDeviceUpdate_ = now;
        refreshDeviceInfo();
    }
}

void RadioDisplay::update(const String& station, const String& track,
                          const String& status, const String& genre, const String& format,
                          bool wifiConnected,
                          uint8_t volume, bool muted, size_t currentStation) {
    if (!display_) return;
    setText(station_, station.c_str());
    const int separator = track.indexOf(" - ");
    const String title = separator >= 0 ? track.substring(separator + 3) : track;
    const String artist = separator >= 0 ? track.substring(0, separator) : String("");
    setText(track_, title.c_str());
    setText(artist_, artist.c_str());
    setUiText(status_, status.c_str());
    const String genreText = musicSource_ ? uiText("Süre: ") + musicTime(musicElapsed_) + "/" +
        (musicDuration_ ? musicTime(musicDuration_) : String("--:--")) :
        uiText("Tür: ") + (genre.length() ? genre : String("—"));
    setText(genre_, genreText.c_str());
    setText(format_, format.length() ? format.c_str() : "—");
    setUiText(wifi_, wifiConnected ? LV_SYMBOL_WIFI "  Bağlı" : LV_SYMBOL_WIFI "  Bekliyor");
    const String volumeText = String(volume);
    setText(volume_, volumeText.c_str());
    setText(muteIcon_, muted || volume == 0 ? LV_SYMBOL_MUTE : LV_SYMBOL_VOLUME_MAX);
    lv_obj_set_style_text_color(mute_, lv_color_hex(muted ? ORANGE : WHITE), 0);
    lv_obj_set_state(minus_, LV_STATE_DISABLED, volume == 0);
    lv_obj_set_state(plus_, LV_STATE_DISABLED, volume >= volumeLimit_);
    const size_t itemCount = musicSource_ ? musicCount_ : stationCount_;
    lv_obj_set_state(previous_, LV_STATE_DISABLED, itemCount < 2);
    lv_obj_set_state(next_, LV_STATE_DISABLED, itemCount < 2);
    const String countText = musicSource_ ? String(musicCount_ ? musicIndex_ + 1 : 0) + " / " + String(musicCount_) + " parça" :
        String(currentStation + 1) + " / " + String(stationCount_) + " istasyon";
    currentStation_ = currentStation;
    setUiText(stationCountLabel_, countText.c_str());
    for (size_t i = 0; i < lv_obj_get_child_count(stationList_); ++i) {
        lv_obj_set_state(lv_obj_get_child(stationList_, i), LV_STATE_CHECKED, stationPage_ * 8 + i == currentStation);
    }
}

void RadioDisplay::present() {
    if (!display_) return;
    loop();
    lv_refr_now(display_);
}

void RadioDisplay::refreshStationRows() {
    lv_obj_clean(stationList_);
    const size_t first = stationPage_ * 8;
    for (size_t i = first; i < stationCount_ && i < first + 8; ++i) {
        auto* row = button(stationList_, stations_[i].name, 0, (i - first) * 64, 724, 56, RadioAction::None, false, false);
        lv_obj_remove_event_cb(row, onButton);
        lv_obj_set_user_data(row, reinterpret_cast<void*>(i));
        lv_obj_add_event_cb(row, onStation, LV_EVENT_CLICKED, this);
        lv_obj_set_style_border_color(row, lv_color_hex(ORANGE), LV_STATE_CHECKED);
        lv_obj_set_style_border_width(row, 2, LV_STATE_CHECKED);
        lv_obj_set_state(row, LV_STATE_CHECKED, i == currentStation_);
        auto* text = lv_obj_get_child(row, 0);
        lv_obj_set_width(text, 670);
        lv_label_set_long_mode(text, LV_LABEL_LONG_MODE_DOTS);
        lv_obj_set_style_text_align(text, LV_TEXT_ALIGN_LEFT, 0);
    }
    lv_obj_scroll_to_y(stationList_, 0, LV_ANIM_OFF);
    const String page = "Sayfa " + String(stationPage_ + 1) + " / " + String((stationCount_ + 7) / 8);
    setUiText(stationPageLabel_, page.c_str());
    lv_obj_set_state(stationPagePrevious_, LV_STATE_DISABLED, stationPage_ == 0);
    lv_obj_set_state(stationPageNext_, LV_STATE_DISABLED, (stationPage_ + 1) * 8 >= stationCount_);
}

void RadioDisplay::setStations(const RadioStation* stations, size_t count) {
    stations_ = stations; stationCount_ = count; stationPage_ = 0; currentStation_ = 0;
    savedSettings_.bootStation = 0; draftSettings_.bootStation = 0;
    refreshStationRows();
    lv_dropdown_close(bootStationDropdown_);
    String names;
    for (size_t i = 0; i < count; ++i) { if (i) names += '\n'; names += stations[i].name; }
    lv_dropdown_set_options(bootStationDropdown_, names.c_str());
    lv_dropdown_set_selected(bootStationDropdown_, 0);
}

void RadioDisplay::buildImport() {
    auto* root = lv_display_get_screen_active(display_);
    importOverlay_ = rectangle(root, 0, 0, 800, 480, BG);
    label(importOverlay_, "SD KARTTAN İSTASYON LİSTESİ", 28, 22, 660, radioFont(24), ORANGE);
    button(importOverlay_, LV_SYMBOL_CLOSE, 716, 10, 56, 56, RadioAction::CancelImport);
    importMessage_ = label(importOverlay_, "", 28, 82, 744, radioFont(18), WHITE);
    lv_obj_set_height(importMessage_, 96);
    lv_label_set_long_mode(importMessage_, LV_LABEL_LONG_MODE_WRAP);
    importFileName_ = label(importOverlay_, "", 28, 180, 744, radioFont(18), ORANGE);
    importList_ = rectangle(importOverlay_, 28, 208, 744, 180, BG);
    lv_obj_set_scrollable(importList_, true);
    lv_obj_set_scroll_dir(importList_, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(importList_, LV_SCROLLBAR_MODE_AUTO);
    importPrevious_ = button(importOverlay_, LV_SYMBOL_LEFT, 28, 410, 96, 56, RadioAction::ImportPreviousPage);
    importRetry_ = button(importOverlay_, "Yeniden tara", 144, 410, 512, 56, RadioAction::OpenImport);
    importNext_ = button(importOverlay_, LV_SYMBOL_RIGHT, 676, 410, 96, 56, RadioAction::ImportNextPage);
    importCancel_ = button(importOverlay_, "Vazgeç", 28, 410, 354, 56, RadioAction::CancelImport);
    importConfirm_ = button(importOverlay_, "Listeyi değiştir", 402, 410, 370, 56, RadioAction::ConfirmImport, true);
    lv_obj_set_hidden(importConfirm_, true);
    lv_obj_set_hidden(importCancel_, true);
    lv_obj_set_hidden(importOverlay_, true);
}

void RadioDisplay::refreshImportRows() {
    lv_obj_clean(importList_);
    const size_t first = importPage_ * 4;
    for (size_t i = first; i < importFiles_.size() && i < first + 4; ++i) {
        auto* row = button(importList_, importFiles_[i].c_str() + 1, 0, (i - first) * 56, 724, 48, RadioAction::None, false, false);
        lv_obj_remove_event_cb(row, onButton);
        lv_obj_set_user_data(row, reinterpret_cast<void*>(i));
        lv_obj_add_event_cb(row, onImportFile, LV_EVENT_CLICKED, this);
        auto* text = lv_obj_get_child(row, 0);
        lv_obj_set_width(text, 680);
        lv_label_set_long_mode(text, LV_LABEL_LONG_MODE_DOTS);
        lv_obj_set_style_text_align(text, LV_TEXT_ALIGN_LEFT, 0);
    }
    lv_obj_scroll_to_y(importList_, 0, LV_ANIM_OFF);
    const String caption = importFiles_.empty() ? "" : "Sayfa " + String(importPage_ + 1) + " / " + String((importFiles_.size() + 3) / 4);
    setUiText(importFileName_, caption.c_str());
    lv_obj_set_state(importPrevious_, LV_STATE_DISABLED, importPage_ == 0);
    lv_obj_set_state(importNext_, LV_STATE_DISABLED, (importPage_ + 1) * 4 >= importFiles_.size());
}

void RadioDisplay::showImportFiles(const std::vector<String>& files, const String& message) {
    importFiles_ = files; importPage_ = 0;
    refreshImportRows();
    setUiText(importMessage_, message.c_str());
    lv_obj_set_hidden(importList_, files.empty());
    lv_obj_set_hidden(importConfirm_, true); lv_obj_set_hidden(importCancel_, true);
    lv_obj_set_hidden(importPrevious_, false); lv_obj_set_hidden(importNext_, false);
    lv_obj_set_hidden(importRetry_, false);
    lv_obj_set_hidden(importOverlay_, false);
}

void RadioDisplay::showImportPreview(const String& file, size_t count) {
    const String text = "Dosya doğrulandı: " + String(count) + " istasyon.\nMevcut istasyon listesi değiştirilecek.\nYeni liste kalıcı bellekte saklanacak.\nOnaylıyor musunuz?";
    setUiText(importMessage_, text.c_str()); setText(importFileName_, file.c_str());
    lv_obj_set_hidden(importList_, true);
    lv_obj_set_hidden(importPrevious_, true); lv_obj_set_hidden(importNext_, true);
    lv_obj_set_hidden(importRetry_, true);
    lv_obj_set_hidden(importConfirm_, false); lv_obj_set_hidden(importCancel_, false);
    lv_obj_set_hidden(importOverlay_, false);
}

void RadioDisplay::showImportMessage(const String& message) {
    showImportFiles({}, message);
}

void RadioDisplay::closeImport() {
    lv_obj_set_hidden(importOverlay_, true);
    lv_obj_clean(importList_); importFiles_.clear();
}

void RadioDisplay::onImportFile(lv_event_t* event) {
    auto* self = static_cast<RadioDisplay*>(lv_event_get_user_data(event));
    auto* row = static_cast<lv_obj_t*>(lv_event_get_current_target(event));
    self->enqueue(RadioAction::SelectImportFile, reinterpret_cast<uintptr_t>(lv_obj_get_user_data(row)));
}

void RadioDisplay::buildSettings() {
    auto* root = lv_display_get_screen_active(display_);
    settingsOverlay_ = rectangle(root, 0, 0, 800, 480, BG);
    label(settingsOverlay_, LV_SYMBOL_SETTINGS "  AYARLAR", 28, 22, 650,
          radioFont(24), ORANGE);
    button(settingsOverlay_, LV_SYMBOL_CLOSE, 716, 10, 56, 56, RadioAction::CloseSettings);
    const char* titles[] = {"Ekran", "Ses", "Radyo", "İstasyonlar", "Cihaz"};
    for (size_t i = 0; i < 5; ++i) {
        settingsTabs_[i] = button(settingsOverlay_, titles[i], 28 + i * 152, 78, 136, 48, RadioAction::None);
        lv_obj_remove_event_cb(settingsTabs_[i], onButton);
        lv_obj_set_user_data(settingsTabs_[i], reinterpret_cast<void*>(i));
        lv_obj_add_event_cb(settingsTabs_[i], onSettingsTab, LV_EVENT_CLICKED, this);
        lv_obj_set_style_border_color(settingsTabs_[i], lv_color_hex(ORANGE), LV_STATE_CHECKED);
        settingsPages_[i] = rectangle(settingsOverlay_, 28, 144, 744, 230, BG);
        lv_obj_set_hidden(settingsPages_[i], i != 0);
    }
    lv_obj_set_state(settingsTabs_[0], LV_STATE_CHECKED, true);
    auto slider = [this](lv_obj_t* parent, int y, int max) {
        auto* object = lv_slider_create(parent);
        lv_obj_set_pos(object, 18, y);
        lv_obj_set_size(object, 690, 16);
        lv_slider_set_range(object, 0, max);
        lv_obj_set_ext_click_area(object, 18);
        lv_obj_set_style_bg_color(object, lv_color_hex(LINE), LV_PART_MAIN);
        lv_obj_set_style_bg_color(object, lv_color_hex(ORANGE), LV_PART_INDICATOR);
        lv_obj_set_style_bg_color(object, lv_color_hex(ORANGE), LV_PART_KNOB);
        lv_obj_add_event_cb(object, onSettingChanged, LV_EVENT_VALUE_CHANGED, this);
        return object;
    };
    auto toggle = [this](lv_obj_t* parent, const char* text, int y) {
        label(parent, text, 0, y + 8, 600, radioFont(18), WHITE);
        auto* object = lv_switch_create(parent);
        lv_obj_set_pos(object, 640, y);
        lv_obj_set_size(object, 72, 40);
        lv_obj_set_style_bg_color(object, lv_color_hex(ORANGE), LV_PART_INDICATOR | LV_STATE_CHECKED);
        lv_obj_add_event_cb(object, onSettingChanged, LV_EVENT_VALUE_CHANGED, this);
        return object;
    };
    auto dropdown = [this](lv_obj_t* parent, int y, const char* options) {
        auto* object = lv_dropdown_create(parent);
        lv_obj_set_pos(object, 384, y);
        lv_obj_set_size(object, 328, 52);
        lv_dropdown_set_options(object, options);
        lv_obj_set_style_bg_color(object, lv_color_hex(PANEL), 0);
        lv_obj_set_style_text_color(object, lv_color_hex(WHITE), 0);
        lv_obj_set_style_text_font(object, radioFont(18), 0);
        auto* list = lv_dropdown_get_list(object);
        lv_obj_set_style_bg_color(list, lv_color_hex(PANEL), 0);
        lv_obj_set_style_text_color(list, lv_color_hex(WHITE), 0);
        lv_obj_set_style_max_height(list, 250, 0);
        lv_obj_set_style_text_font(list, radioFont(18), 0);
        lv_obj_add_event_cb(object, onSettingChanged, LV_EVENT_VALUE_CHANGED, this);
        return object;
    };
    auto* page = settingsPages_[0];
    brightnessValue_ = label(page, "", 0, 0, 720, radioFont(18), WHITE);
    brightnessSlider_ = slider(page, 48, 255);
    lv_slider_set_range(brightnessSlider_, 13, 255);
    label(page, "Karartma süresi", 0, 96, 350, radioFont(18), WHITE);
    dimDropdown_ = dropdown(page, 80, uiText("Kapalı\n30 saniye\n1 dakika\n2 dakika\n5 dakika\n10 dakika").c_str());
    label(page, "Süre dolunca %50 parlaklık; 15 sn sonra arka ışık kapanır.", 0, 140, 720, radioFont(14), MUTED);
    label(page, "İlk dokunuş yalnızca ekranı uyandırır.", 0, 162, 720, radioFont(14), MUTED);
    vuSwitch_ = toggle(page, "VU metre", 186);
    page = settingsPages_[1];
    bootVolumeValue_ = label(page, "", 0, 0, 720, radioFont(18), WHITE);
    bootVolumeSlider_ = slider(page, 34, 21);
    maxVolumeValue_ = label(page, "", 0, 60, 720, radioFont(18), WHITE);
    maxVolumeSlider_ = slider(page, 94, 21);
    rememberSwitch_ = toggle(page, "Son ses seviyesini hatırla", 126);
    button(page, "Ekolayzer", 0, 178, 712, 48, RadioAction::OpenEqualizer);
    page = settingsPages_[2];
    autoPlaySwitch_ = toggle(page, "Açılışta otomatik yayın", 0);
    resumeSwitch_ = toggle(page, "Son istasyonu hatırla", 70);
    label(page, "Açılış istasyonu", 0, 160, 350, radioFont(18), WHITE);
    String names;
    for (size_t i = 0; i < stationCount_; ++i) {
        if (i) names += '\n';
        names += stations_[i].name;
    }
    bootStationDropdown_ = dropdown(page, 148, names.c_str());
    page = settingsPages_[3];
    label(page, "CSV dosyası seçerek tüm istasyon listesini değiştirin.", 0, 0, 720, radioFont(18), WHITE);
    button(page, LV_SYMBOL_SD_CARD "  SD karttan liste yükle", 0, 56, 712, 56, RadioAction::OpenImport);
    label(page, "Liste cihaza kaydedilir; SD kart olmadan da korunur.", 0, 144, 720, radioFont(14), MUTED);
    label(settingsPages_[4], "Dil / Language", 0, 12, 350, radioFont(18), WHITE);
    languageDropdown_ = dropdown(settingsPages_[4], 0, "English\nTürkçe");
    deviceInfo_ = label(settingsPages_[4], "", 0, 62, 720, radioFont(14), WHITE);
    lv_obj_set_height(deviceInfo_, 106);
    lv_label_set_long_mode(deviceInfo_, LV_LABEL_LONG_MODE_WRAP);
    button(settingsPages_[4], "Wi-Fi ağını ayarla / değiştir", 0, 174, 712, 52, RadioAction::OpenWifi);
    // Child panel shares the settings transaction and its Save/Cancel buttons.
    equalizerPanel_ = rectangle(settingsOverlay_, 28, 78, 744, 296, BG);
    label(equalizerPanel_, "EKOLAYZER", 0, 10, 340, radioFont(24), ORANGE);
    auto* flat = button(equalizerPanel_, "Düz", 390, 0, 160, 44, RadioAction::None);
    lv_obj_remove_event_cb(flat, onButton);
    lv_obj_set_user_data(flat, reinterpret_cast<void*>(6));
    lv_obj_add_event_cb(flat, onEqualizer, LV_EVENT_CLICKED, this);
    button(equalizerPanel_, "Geri", 568, 0, 160, 44, RadioAction::CloseEqualizer);
    const char* bands[] = {"Bas", "Orta", "Tiz"};
    for (size_t i = 0; i < 3; ++i) {
        const int y = 60 + i * 70;
        label(equalizerPanel_, bands[i], 0, y + 14, 220, radioFont(24), WHITE);
        equalizerMinus_[i] = button(equalizerPanel_, "-", 260, y, 80, 52, RadioAction::None);
        equalizerValues_[i] = label(equalizerPanel_, "", 350, y + 14, 180, radioFont(24), ORANGE);
        lv_obj_set_style_text_align(equalizerValues_[i], LV_TEXT_ALIGN_CENTER, 0);
        equalizerPlus_[i] = button(equalizerPanel_, "+", 540, y, 80, 52, RadioAction::None);
        lv_obj_t* controls[] = {equalizerMinus_[i], equalizerPlus_[i]};
        for (size_t j = 0; j < 2; ++j) {
            lv_obj_remove_event_cb(controls[j], onButton);
            lv_obj_set_user_data(controls[j], reinterpret_cast<void*>(i * 2 + j));
            lv_obj_add_event_cb(controls[j], onEqualizer, LV_EVENT_CLICKED, this);
        }
    }
    lv_obj_set_hidden(equalizerPanel_, true);
    settingsMessage_ = label(settingsOverlay_, "", 28, 376, 744, radioFont(14), ORANGE);
    button(settingsOverlay_, "Vazgeç", 28, 410, 354, 56, RadioAction::CancelSettings);
    button(settingsOverlay_, "Kaydet", 402, 410, 370, 56, RadioAction::SaveSettings, true);
    lv_obj_set_hidden(settingsOverlay_, true);
}

void RadioDisplay::configureSettings(const RadioSettings& settings) {
    savedSettings_ = settings;
    volumeLimit_ = settings.maxVolume;
    if (radioLanguage() != settings.language) {
        setRadioLanguage(settings.language);
        refreshUiText(lv_display_get_screen_active(display_));
        lv_dropdown_close(dimDropdown_);
        lv_dropdown_close(languageDropdown_);
        lv_dropdown_set_options(dimDropdown_, uiText("Kapalı\n30 saniye\n1 dakika\n2 dakika\n5 dakika\n10 dakika").c_str());
        const uint16_t timeouts[] = {0, 30, 60, 120, 300, 600};
        for (size_t i = 0; i < 6; ++i) if (timeouts[i] == settings.dimSeconds) lv_dropdown_set_selected(dimDropdown_, i);
        if (wifiOverlay_) showWifiNetworks(wifiNames_);
        refreshDeviceInfo();
    }
    lv_dropdown_set_selected(languageDropdown_, static_cast<uint8_t>(settings.language));
}

void RadioDisplay::applyScreenSettings(const RadioSettings& settings) {
    vuEnabled_ = settings.vuMeter;
    if (vuTrack_) lv_obj_set_hidden(vuTrack_, !vuEnabled_);
    const bool brightnessChanged = normalBrightness_ != settings.brightness;
    const bool wasDimmed = dimmed_;
    normalBrightness_ = settings.brightness;
    dimSeconds_ = settings.dimSeconds;
    dimmed_ = false;
    backlightOff_ = false;
    dimmedAt_ = 0;
    if (wasDimmed && wakeOverlay_) lv_obj_set_hidden(wakeOverlay_, true);
    if (brightnessChanged || wasDimmed)
        smartdisplay_lcd_set_backlight(normalBrightness_ / 255.0f);
    lv_display_trigger_activity(display_);
}

void RadioDisplay::openSettings() {
    draftSettings_ = savedSettings_;
    lv_dropdown_set_selected(languageDropdown_, static_cast<uint8_t>(draftSettings_.language));
    lv_obj_set_hidden(equalizerPanel_, true);
    lv_slider_set_value(brightnessSlider_, draftSettings_.brightness, LV_ANIM_OFF);
    lv_slider_set_value(maxVolumeSlider_, draftSettings_.maxVolume, LV_ANIM_OFF);
    lv_slider_set_range(bootVolumeSlider_, 0, draftSettings_.maxVolume ? draftSettings_.maxVolume : 1);
    lv_obj_set_state(bootVolumeSlider_, LV_STATE_DISABLED, draftSettings_.maxVolume == 0);
    lv_slider_set_value(bootVolumeSlider_, draftSettings_.bootVolume, LV_ANIM_OFF);
    const uint16_t timeouts[] = {0, 30, 60, 120, 300, 600};
    for (size_t i = 0; i < 6; ++i) if (timeouts[i] == draftSettings_.dimSeconds) lv_dropdown_set_selected(dimDropdown_, i);
    lv_obj_set_state(rememberSwitch_, LV_STATE_CHECKED, draftSettings_.rememberVolume);
    lv_obj_set_state(vuSwitch_, LV_STATE_CHECKED, draftSettings_.vuMeter);
    lv_obj_set_state(autoPlaySwitch_, LV_STATE_CHECKED, draftSettings_.autoPlay);
    lv_obj_set_state(resumeSwitch_, LV_STATE_CHECKED, draftSettings_.resumeStation);
    lv_dropdown_set_selected(bootStationDropdown_, draftSettings_.bootStation);
    refreshSettingsLabels();
    refreshDeviceInfo();
    setUiText(settingsMessage_, "Değişiklikleri deneyin, sonra Kaydet veya Vazgeç.");
    enqueue(RadioAction::BeginSettings);
    lv_obj_set_hidden(settingsOverlay_, false);
}

void RadioDisplay::onSettingChanged(lv_event_t* event) {
    auto* self = static_cast<RadioDisplay*>(lv_event_get_user_data(event));
    auto& draft = self->draftSettings_;
    draft.brightness = lv_slider_get_value(self->brightnessSlider_);
    draft.maxVolume = lv_slider_get_value(self->maxVolumeSlider_);
    draft.bootVolume = lv_slider_get_value(self->bootVolumeSlider_);
    draft.rememberVolume = lv_obj_has_state(self->rememberSwitch_, LV_STATE_CHECKED);
    draft.vuMeter = lv_obj_has_state(self->vuSwitch_, LV_STATE_CHECKED);
    draft.autoPlay = lv_obj_has_state(self->autoPlaySwitch_, LV_STATE_CHECKED);
    draft.resumeStation = lv_obj_has_state(self->resumeSwitch_, LV_STATE_CHECKED);
    draft.bootStation = lv_dropdown_get_selected(self->bootStationDropdown_);
    draft.language = static_cast<RadioLanguage>(lv_dropdown_get_selected(self->languageDropdown_));
    const uint16_t timeouts[] = {0, 30, 60, 120, 300, 600};
    draft.dimSeconds = timeouts[lv_dropdown_get_selected(self->dimDropdown_)];
    draft.validate(self->stationCount_);
    lv_slider_set_range(self->bootVolumeSlider_, 0, draft.maxVolume ? draft.maxVolume : 1);
    lv_obj_set_state(self->bootVolumeSlider_, LV_STATE_DISABLED, draft.maxVolume == 0);
    lv_slider_set_value(self->bootVolumeSlider_, draft.bootVolume, LV_ANIM_OFF);
    self->refreshSettingsLabels();
    self->enqueue(RadioAction::PreviewSettings);
}

void RadioDisplay::refreshSettingsLabels() {
    const int8_t gains[] = {draftSettings_.bass, draftSettings_.mid, draftSettings_.treble};
    for (size_t i = 0; i < 3; ++i) {
        const String value = String(gains[i] > 0 ? "+" : "") + String(static_cast<int>(gains[i])) + " dB";
        setText(equalizerValues_[i], value.c_str());
        lv_obj_set_state(equalizerMinus_[i], LV_STATE_DISABLED, gains[i] <= -12);
        lv_obj_set_state(equalizerPlus_[i], LV_STATE_DISABLED, gains[i] >= 6);
    }
    const String brightness = "Parlaklık: %" + String((draftSettings_.brightness * 100 + 127) / 255);
    const String boot = "Açılış ses seviyesi: " + String(draftSettings_.bootVolume);
    const String max = "Azami ses seviyesi: " + String(draftSettings_.maxVolume);
    setUiText(brightnessValue_, brightness.c_str());
    setUiText(bootVolumeValue_, boot.c_str());
    setUiText(maxVolumeValue_, max.c_str());
    lv_obj_set_state(bootStationDropdown_, LV_STATE_DISABLED, draftSettings_.resumeStation);
}

void RadioDisplay::onEqualizer(lv_event_t* event) {
    auto* self = static_cast<RadioDisplay*>(lv_event_get_user_data(event));
    auto* target = static_cast<lv_obj_t*>(lv_event_get_current_target(event));
    const auto control = reinterpret_cast<uintptr_t>(lv_obj_get_user_data(target));
    auto& draft = self->draftSettings_;
    if (control == 6) draft.bass = draft.mid = draft.treble = 0;
    else if (control < 6) {
        int8_t* gains[] = {&draft.bass, &draft.mid, &draft.treble};
        *gains[control / 2] += (control % 2) ? 1 : -1;
    } else return;
    draft.validate(self->stationCount_);
    self->refreshSettingsLabels();
    self->enqueue(RadioAction::PreviewSettings);
}

void RadioDisplay::onSettingsTab(lv_event_t* event) {
    auto* self = static_cast<RadioDisplay*>(lv_event_get_user_data(event));
    lv_dropdown_close(self->dimDropdown_);
    lv_dropdown_close(self->bootStationDropdown_);
    lv_dropdown_close(self->languageDropdown_);
    const auto index = reinterpret_cast<uintptr_t>(lv_obj_get_user_data(static_cast<lv_obj_t*>(lv_event_get_current_target(event))));
    for (size_t i = 0; i < 5; ++i) {
        lv_obj_set_hidden(self->settingsPages_[i], i != index);
        lv_obj_set_state(self->settingsTabs_[i], LV_STATE_CHECKED, i == index);
    }
}

void RadioDisplay::refreshDeviceInfo() {
    const bool connected = WiFi.status() == WL_CONNECTED;
    String text = "Wi-Fi: " + String(connected ? "Bağlı" : "Bağlı değil");
    if (connected) text += "\nSinyal: " + String(WiFi.RSSI()) + " dBm\nIP: " + WiFi.localIP().toString();
    text += "\nYazılım: " + String(RADIO_VERSION);
    text += connected ? "\nWeb: http://" + WiFi.localIP().toString() + "/" : "\nWeb: Wi-Fi bağlantısı gerekli";
    setUiText(deviceInfo_, text.c_str());
}

void RadioDisplay::finishSettings(bool close, const char* message) {
    setUiText(settingsMessage_, message);
    if (close) {
        lv_obj_set_hidden(equalizerPanel_, true);
        lv_dropdown_close(dimDropdown_);
        lv_dropdown_close(bootStationDropdown_);
        lv_dropdown_close(languageDropdown_);
        lv_obj_set_hidden(settingsOverlay_, true);
    }
}

void RadioDisplay::onWake(lv_event_t* event) {
    auto* self = static_cast<RadioDisplay*>(lv_event_get_user_data(event));
    self->dimmed_ = false;
    self->backlightOff_ = false;
    self->dimmedAt_ = 0;
    smartdisplay_lcd_set_backlight(self->normalBrightness_ / 255.0f);
    lv_obj_set_hidden(self->wakeOverlay_, true);
    lv_display_trigger_activity(self->display_);
}

void RadioDisplay::buildWifi() {
    auto* root = lv_display_get_screen_active(display_);
    wifiOverlay_ = rectangle(root, 0, 0, 800, 480, BG);
    label(wifiOverlay_, "WI-FI KURULUMU", 28, 22, 650, radioFont(24), ORANGE);
    label(wifiOverlay_, "2,4 GHz ağ seçin veya gizli ağın adını yazın.", 28, 58, 744, radioFont(14), MUTED);
    button(wifiOverlay_, LV_SYMBOL_CLOSE, 716, 10, 56, 56, RadioAction::CloseWifi);
    wifiNetworks_ = lv_dropdown_create(wifiOverlay_);
    lv_obj_set_pos(wifiNetworks_, 28, 82); lv_obj_set_size(wifiNetworks_, 584, 44);
    lv_dropdown_set_options(wifiNetworks_, uiText("Ağları taramak için Tara'ya dokunun").c_str());
    lv_obj_set_style_bg_color(wifiNetworks_, lv_color_hex(PANEL), 0);
    lv_obj_set_style_text_color(wifiNetworks_, lv_color_hex(WHITE), 0);
    lv_obj_set_style_text_font(wifiNetworks_, radioFont(18), 0);
    auto* list = lv_dropdown_get_list(wifiNetworks_);
    lv_obj_set_style_bg_color(list, lv_color_hex(PANEL), 0);
    lv_obj_set_style_text_color(list, lv_color_hex(WHITE), 0);
    lv_obj_set_style_text_font(list, radioFont(18), 0);
    lv_obj_set_style_max_height(list, 240, 0);
    lv_obj_add_event_cb(wifiNetworks_, onWifiNetwork, LV_EVENT_VALUE_CHANGED, this);
    wifiScan_ = button(wifiOverlay_, "Tara", 628, 82, 144, 44, RadioAction::ScanWifi);
    label(wifiOverlay_, "Ağ adı (SSID)", 28, 132, 350, radioFont(14), MUTED);
    label(wifiOverlay_, "Şifre (açık ağda boş bırakın)", 408, 132, 364, radioFont(14), MUTED);
    lv_obj_t** inputs[] = {&wifiSsidInput_, &wifiPasswordInput_};
    for (size_t i = 0; i < 2; ++i) {
        auto* input = lv_textarea_create(wifiOverlay_);
        *inputs[i] = input;
        lv_obj_set_pos(input, i ? 408 : 28, 152); lv_obj_set_size(input, i ? 364 : 360, 48);
        lv_textarea_set_one_line(input, true);
        lv_textarea_set_max_length(input, i ? 64 : 32);
        lv_obj_set_style_bg_color(input, lv_color_hex(PANEL), 0);
        lv_obj_set_style_text_color(input, lv_color_hex(WHITE), 0);
        lv_obj_set_style_text_font(input, radioFont(18), 0);
        lv_obj_add_event_cb(input, onWifiInput, LV_EVENT_FOCUSED, this);
        lv_obj_add_event_cb(input, onWifiInput, LV_EVENT_CLICKED, this);
    }
    lv_textarea_set_password_mode(wifiPasswordInput_, true);
    lv_textarea_set_password_show_time(wifiPasswordInput_, 0);
    wifiMessage_ = label(wifiOverlay_, "", 28, 208, 744, radioFont(14), ORANGE);
    lv_obj_set_height(wifiMessage_, 46);
    lv_label_set_long_mode(wifiMessage_, LV_LABEL_LONG_MODE_WRAP);
    wifiConnect_ = button(wifiOverlay_, "Bağlan ve kaydet", 28, 260, 360, 52, RadioAction::ConnectWifi, true);
    button(wifiOverlay_, "Kapat / denemeyi iptal et", 408, 260, 364, 52, RadioAction::CloseWifi);
    wifiKeyboard_ = lv_keyboard_create(wifiOverlay_);
    lv_obj_set_size(wifiKeyboard_, 800, 160);
    // Keyboard constructor uses bottom alignment; y=320 would push it off-screen.
    lv_obj_align(wifiKeyboard_, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(wifiKeyboard_, lv_color_hex(BG), 0);
    lv_obj_set_style_pad_all(wifiKeyboard_, 4, 0);
    lv_obj_set_style_bg_color(wifiKeyboard_, lv_color_hex(PANEL), LV_PART_ITEMS);
    lv_obj_set_style_text_color(wifiKeyboard_, lv_color_hex(WHITE), LV_PART_ITEMS);
    lv_obj_set_style_text_font(wifiKeyboard_, radioFont(18), LV_PART_ITEMS);
    lv_obj_set_style_bg_color(wifiKeyboard_, lv_color_hex(ORANGE), LV_PART_ITEMS | LV_STATE_PRESSED);
    lv_keyboard_set_textarea(wifiKeyboard_, wifiSsidInput_);
    lv_obj_set_hidden(wifiOverlay_, true);
}

void RadioDisplay::openWifi(const String& savedSsid) {
    if (!wifiOverlay_) buildWifi();
    wifiStateValid_ = false;
    lv_dropdown_close(dimDropdown_); lv_dropdown_close(bootStationDropdown_); lv_dropdown_close(languageDropdown_);
    lv_textarea_set_text(wifiSsidInput_, savedSsid.c_str());
    lv_textarea_set_text(wifiPasswordInput_, ""); // Never expose the stored password.
    lv_keyboard_set_textarea(wifiKeyboard_, savedSsid.isEmpty() ? wifiSsidInput_ : wifiPasswordInput_);
    lv_obj_set_hidden(wifiOverlay_, false);
    lv_display_trigger_activity(display_);
}

void RadioDisplay::closeWifi() {
    if (!wifiOverlay_) return;
    lv_dropdown_close(wifiNetworks_);
    lv_textarea_set_text(wifiPasswordInput_, "");
    lv_keyboard_set_textarea(wifiKeyboard_, nullptr);
    lv_obj_set_hidden(wifiOverlay_, true);
}

void RadioDisplay::clearWifiPassword() {
    if (wifiPasswordInput_) lv_textarea_set_text(wifiPasswordInput_, "");
}

void RadioDisplay::showWifiNetworks(const std::vector<String>& names) {
    if (!wifiOverlay_) return;
    wifiNames_ = names;
    String options = uiText("Ağ seçin...");
    for (const auto& name : names) { options += '\n'; options += name; }
    lv_dropdown_close(wifiNetworks_);
    const String shown = names.empty() ? uiText("Liste boş; Tara veya elle ağ adı girişi") : options;
    lv_dropdown_set_options(wifiNetworks_, shown.c_str());
}

void RadioDisplay::updateWifi(const String& message, bool busy, bool scanning) {
    if (!wifiOverlay_) return;
    setUiText(wifiMessage_, message.c_str());
    const bool empty = wifiNames_.empty();
    if (wifiStateValid_ && wifiBusy_ == busy && wifiScanning_ == scanning && wifiNetworksEmpty_ == empty) return;
    wifiStateValid_ = true; wifiBusy_ = busy; wifiScanning_ = scanning; wifiNetworksEmpty_ = empty;
    lv_obj_set_state(wifiConnect_, LV_STATE_DISABLED, busy);
    lv_obj_set_state(wifiScan_, LV_STATE_DISABLED, busy || scanning);
    lv_obj_set_state(wifiNetworks_, LV_STATE_DISABLED, busy || scanning || empty);
    lv_obj_set_state(wifiSsidInput_, LV_STATE_DISABLED, busy);
    lv_obj_set_state(wifiPasswordInput_, LV_STATE_DISABLED, busy);
    if (busy) lv_keyboard_set_textarea(wifiKeyboard_, nullptr);
    else if (!lv_keyboard_get_textarea(wifiKeyboard_)) lv_keyboard_set_textarea(wifiKeyboard_, wifiPasswordInput_);
}

void RadioDisplay::onWifiInput(lv_event_t* event) {
    auto* self = static_cast<RadioDisplay*>(lv_event_get_user_data(event));
    lv_keyboard_set_textarea(self->wifiKeyboard_, static_cast<lv_obj_t*>(lv_event_get_current_target(event)));
}

void RadioDisplay::onWifiNetwork(lv_event_t* event) {
    auto* self = static_cast<RadioDisplay*>(lv_event_get_user_data(event));
    const size_t selected = lv_dropdown_get_selected(self->wifiNetworks_);
    if (!selected || selected > self->wifiNames_.size()) return;
    lv_textarea_set_text(self->wifiSsidInput_, self->wifiNames_[selected - 1].c_str());
    lv_textarea_set_text(self->wifiPasswordInput_, "");
    lv_keyboard_set_textarea(self->wifiKeyboard_, self->wifiPasswordInput_);
}

void RadioDisplay::setMusicState(bool source, bool playing, bool paused, size_t index, size_t count,
                               uint32_t elapsed, uint32_t duration, uint8_t repeat, bool shuffle,
                               const String& playingPath, bool canSeek) {
    if (musicSource_ != source) {
        setUiText(lv_obj_get_child(sourceButton_, 0), source ? "Radyo" : "SD MP3");
        setUiText(sourceCaption_, source ? "SES KAYNAĞI" : "İSTASYON");
        setUiText(lv_obj_get_child(listButton_, 0), source ? LV_SYMBOL_LIST "  Dosya listesi" : LV_SYMBOL_LIST "  İstasyon listesi");
        lv_obj_set_width(listButton_, source ? 246 : 392);
        lv_obj_set_hidden(musicPause_, !source);
        lv_obj_set_width(station_, source ? 196 : 744);
        // Avoid continuous title animation while playing local MP3 files.
        lv_label_set_long_mode(track_, source ? LV_LABEL_LONG_MODE_DOTS : LV_LABEL_LONG_MODE_SCROLL_CIRCULAR);
        for (auto* control : {musicRepeatOne_, musicRepeatAll_, musicShuffle_}) lv_obj_set_hidden(control, !source);
        lv_obj_set_hidden(musicSeekBack_, !source); lv_obj_set_hidden(musicSeekForward_, !source);
    }
    musicSource_ = source; musicIndex_ = index; musicCount_ = count;
    musicElapsed_ = elapsed; musicDuration_ = duration;
    musicPlayingPath_ = playingPath;
    lv_obj_set_state(musicSeekBack_, LV_STATE_DISABLED, !canSeek || elapsed == 0);
    lv_obj_set_state(musicSeekForward_, LV_STATE_DISABLED, !canSeek || duration <= 1 || elapsed >= duration - 1);
    lv_obj_set_state(musicRepeatOne_, LV_STATE_CHECKED, repeat == 1);
    lv_obj_set_state(musicRepeatAll_, LV_STATE_CHECKED, repeat == 2);
    lv_obj_set_state(musicShuffle_, LV_STATE_CHECKED, shuffle);
    setUiText(lv_obj_get_child(musicPause_, 0), playing && !paused ? LV_SYMBOL_PAUSE " Duraklat" : LV_SYMBOL_PLAY " Oynat");
    lv_obj_set_state(musicPause_, LV_STATE_DISABLED, count == 0);
    if (musicList_) {
        for (size_t i = 0; i < lv_obj_get_child_count(musicList_); ++i)
            lv_obj_set_state(lv_obj_get_child(musicList_, i), LV_STATE_CHECKED,
                source && musicPage_ * 5 + i < musicEntries_.size() && musicEntries_[musicPage_ * 5 + i].path == playingPath);
    }
}

void RadioDisplay::buildMusic() {
    musicOverlay_ = rectangle(lv_display_get_screen_active(display_), 0, 0, 800, 480, BG);
    label(musicOverlay_, "SD MÜZİK", 28, 24, 280, radioFont(24), ORANGE);
    musicParent_ = button(musicOverlay_, "Üst klasör", 330, 10, 170, 56, RadioAction::MusicParent);
    button(musicOverlay_, "Radyoya dön", 512, 10, 196, 56, RadioAction::SwitchRadio);
    button(musicOverlay_, LV_SYMBOL_CLOSE, 716, 10, 56, 56, RadioAction::CloseMusic);
    musicFolderLabel_ = label(musicOverlay_, "/", 28, 76, 744, radioFont(18), WHITE);
    musicMessage_ = label(musicOverlay_, "", 28, 104, 744, radioFont(14), MUTED);
    musicList_ = rectangle(musicOverlay_, 28, 128, 744, 260, BG);
    musicPagePrevious_ = button(musicOverlay_, LV_SYMBOL_LEFT, 28, 410, 80, 56, RadioAction::MusicPreviousPage);
    musicPageLabel_ = label(musicOverlay_, "", 128, 430, 280, radioFont(18), WHITE);
    button(musicOverlay_, "Yeniden tara", 420, 410, 244, 56, RadioAction::ScanMusic);
    musicPageNext_ = button(musicOverlay_, LV_SYMBOL_RIGHT, 692, 410, 80, 56, RadioAction::MusicNextPage);
}

void RadioDisplay::refreshMusicRows() {
    lv_obj_clean(musicList_);
    const size_t first = musicPage_ * 5;
    for (size_t i = first; i < musicEntries_.size() && i < first + 5; ++i) {
        const auto& entry = musicEntries_[i];
        String name = entry.path.substring(entry.path.lastIndexOf('/') + 1);
        if (entry.directory) name = LV_SYMBOL_DIRECTORY "  " + name;
        auto* row = button(musicList_, name.c_str(), 0, (i - first) * 52, 744, 46, RadioAction::None, false, false);
        lv_obj_remove_event_cb(row, onButton);
        lv_obj_set_user_data(row, reinterpret_cast<void*>(i));
        lv_obj_add_event_cb(row, onMusicFile, LV_EVENT_CLICKED, this);
        lv_obj_set_style_border_color(row, lv_color_hex(ORANGE), LV_STATE_CHECKED);
        lv_obj_set_state(row, LV_STATE_CHECKED, musicSource_ && !entry.directory && entry.path == musicPlayingPath_);
        auto* text = lv_obj_get_child(row, 0);
        lv_obj_set_width(text, 700);
        lv_label_set_long_mode(text, LV_LABEL_LONG_MODE_DOTS);
    }
    const size_t pages = (musicEntries_.size() + 4) / 5;
    const String caption = "Sayfa " + String(pages ? musicPage_ + 1 : 0) + " / " + String(pages);
    setUiText(musicPageLabel_, caption.c_str());
    lv_obj_set_state(musicPagePrevious_, LV_STATE_DISABLED, musicPage_ == 0);
    lv_obj_set_state(musicPageNext_, LV_STATE_DISABLED, (musicPage_ + 1) * 5 >= musicEntries_.size());
}

void RadioDisplay::showMusicFolder(const String& folder, const std::vector<SdMusicEntry>& entries, const String& message) {
    if (!musicOverlay_) buildMusic();
    musicEntries_ = entries; musicPage_ = 0;
    setText(musicFolderLabel_, folder.c_str());
    lv_obj_set_state(musicParent_, LV_STATE_DISABLED, folder == "/");
    setUiText(musicMessage_, message.c_str());
    refreshMusicRows();
    lv_obj_move_foreground(musicOverlay_);
    lv_obj_set_hidden(musicOverlay_, false);
}

void RadioDisplay::onMusicFile(lv_event_t* event) {
    auto* self = static_cast<RadioDisplay*>(lv_event_get_user_data(event));
    auto* row = static_cast<lv_obj_t*>(lv_event_get_current_target(event));
    const size_t index = reinterpret_cast<uintptr_t>(lv_obj_get_user_data(row));
    if (index >= self->musicEntries_.size()) return;
    self->enqueue(RadioAction::SelectMusic, index);
    if (!self->musicEntries_[index].directory) lv_obj_set_hidden(self->musicOverlay_, true);
}
