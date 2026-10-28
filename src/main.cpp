#include <Arduino.h>
#include <WiFi.h>
#include <Audio.h>
#include "RadioDisplay.h"
#include "StationFavorites.h"
#include "SdDiagnostics.h"
#include "SdStationImport.h"
#include "RadioWeb.h"
#include "RadioWifi.h"
#include "RadioDiscovery.h"
#include "RadioOta.h"
#include "SdMusicFiles.h"
#include "PlaybackResume.h"
#include "RadioLanguage.h"
#include <SD.h>
#include <esp_system.h>
#include <math.h>


constexpr uint8_t I2S_BCLK = 0;
constexpr uint8_t I2S_LRC = 18;
constexpr uint8_t I2S_DOUT = 17;

// Default volume; saved preferences are applied during startup.
uint8_t volume = 11;
bool muted = false;

StationStore stations;
StationFavorites stationFavorites;
size_t currentStation = 0;
bool audioReady = false;
RadioSettings settings;
RadioSettings previewSettings;
bool settingsEditing = false;
uint8_t volumeBeforeSettings = 11;
bool mutedBeforeSettings = false;
bool playbackDirty = false;
uint32_t playbackChangedAt = 0;

void markPlaybackChanged() {
    playbackDirty = true;
    playbackChangedAt = millis();
}

void persistPlayback() {
    if (!playbackDirty || settingsEditing || millis() - playbackChangedAt < 2000) return;
    if (saveLastPlayback(volume, currentStation)) playbackDirty = false;
    else playbackChangedAt = millis();
}

RadioDisplay screen;
Audio audio;
// Audio::loop invokes this synchronously on the main loop task. No UI, allocation,
// clock reads or floating point in the sample callback. Audio samples stay intact.
static uint16_t vuSamplePeak = 0;
static bool vuCaptureEnabled = false;
void audio_process_i2s(uint32_t* sample, bool* continueI2S) {
    *continueI2S = true; // Required: otherwise the library skips the I2S write.
    if (!vuCaptureEnabled) return;
    const int32_t left = static_cast<int16_t>(*sample & 0xFFFF);
    const int32_t right = static_cast<int16_t>(*sample >> 16);
    const uint16_t a = left < 0 ? -left : left;
    const uint16_t b = right < 0 ? -right : right;
    const uint16_t peak = a > b ? a : b;
    if (peak > vuSamplePeak) vuSamplePeak = peak;
}

void serviceVu() {
    static uint32_t updatedAt = 0, peakAt = 0;
    static uint16_t level = 0, heldPeak = 0;
    const uint32_t now = millis();
    vuCaptureEnabled = screen.vuActive();
    if (!vuCaptureEnabled) {
        vuSamplePeak = 0;
        level = heldPeak = 0;
        peakAt = updatedAt = now;
        return; // No measurement calculations or VU drawing while disabled/off.
    }
    if (now - updatedAt < 100) return;
    updatedAt = now;
    const uint16_t amplitude = vuSamplePeak;
    vuSamplePeak = 0;
    // Log scale (-48..0 dBFS) keeps quiet passages visible. Prototype peak meter,
    // not a calibrated VU instrument. Only ten logarithm evaluations per second.
    uint16_t target = 0;
    if (amplitude && audio.isRunning()) {
        const float scaled = (20.0f * log10f(amplitude / 32768.0f) + 48.0f) * (1000.0f / 48.0f);
        if (scaled > 0) target = scaled >= 1000 ? 1000 : static_cast<uint16_t>(scaled);
    }
    level = target >= level ? target : (level > 100 ? level - 100 : 0);
    if (level < target) level = target;
    if (level >= heldPeak) {
        heldPeak = level;
        peakAt = now;
    } else if (now - peakAt >= 600) {
        heldPeak = heldPeak > 60 ? heldPeak - 60 : 0;
        if (heldPeak < level) heldPeak = level;
    }
    screen.updateVu(level, heldPeak);
}

RadioWeb radioWeb;
RadioOta radioOta;
bool otaMode = false;
RadioWifi network;
RadioDiscovery radioDiscovery;
bool lastWifiConnected = false;
bool sdMode = false, sdActive = false, sdPaused = false, sdEofPending = false;
std::vector<String> musicFiles;
std::vector<SdMusicEntry> musicEntries;
String musicFolder = "/";
PlaybackResume sourceResume;
bool restoringPlayback = false, resumeSavePending = false;
uint32_t resumeSaveAt = 0;
void persistSourceState(bool force);
void updateSdTime(bool force);
size_t currentMusic = 0;
String sdTitle, sdArtist;
uint32_t sdCardCheckAt = 0, sdProgressAt = 0, sdLastPosition = UINT32_MAX;
enum class SdRepeat : uint8_t { Off, One, All };
SdRepeat sdRepeat = SdRepeat::Off;
bool sdShuffle = false;
std::vector<size_t> sdOrder;
size_t sdOrderPosition = 0;
uint32_t sdElapsed = 0, sdDuration = 0, sdTimeCheckAt = 0;

bool radioStarted = false;
bool playbackRequested = false;
bool streamReady = false;
bool httpFailurePending = false;
bool httpFailureRetryable = false;
uint16_t httpFailureCode = 0;
uint8_t retryAttempt = 0;
bool retryPending = false;
uint32_t retryAt = 0;
uint32_t connectionStartedAt = 0;
uint32_t streamReadyAt = 0;
constexpr uint8_t MAX_STREAM_RETRIES = 3;
bool screenReady = false;
bool screenDirty = true;
bool stationDeletePending = false;
String stationDeleteUrl;
uint32_t stationDeleteIdentity = 0;
bool failedStationPromptPending = false;
String failedStationPromptUrl;
void requestStationDeletion(size_t index, bool unavailable);

String stationName = "Groove Salad";
String trackTitle;
bool trackIsStatus = false;
String statusText = "Wi-Fi bağlanıyor...";
String streamGenre;
String streamFormat;
String broadcasterGenre;
String broadcasterStationName;
uint32_t lastStreamDetailsAt = 0;

String metadataStationKey(String text) {
    // Match common display/header spelling differences, e.g. Süper FM / SUPER_FM.
    text.replace("Ç", "c"); text.replace("ç", "c");
    text.replace("Ğ", "g"); text.replace("ğ", "g");
    text.replace("İ", "i"); text.replace("ı", "i");
    text.replace("Ö", "o"); text.replace("ö", "o");
    text.replace("Ş", "s"); text.replace("ş", "s");
    text.replace("Ü", "u"); text.replace("ü", "u");
    text.toLowerCase();
    String key;
    key.reserve(text.length());
    for (size_t i = 0; i < text.length(); ++i) {
        const uint8_t c = static_cast<uint8_t>(text[i]);
        if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c >= 0x80)
            key += static_cast<char>(c);
    }
    return key;
}

void refreshStreamGenre() {
    String genre = broadcasterGenre;
    const String key = metadataStationKey(genre);
    if (!key.length() || key == metadataStationKey(stations[currentStation].name) ||
        (broadcasterStationName.length() && key == metadataStationKey(broadcasterStationName))) {
        genre = "";
    }
    if (streamGenre != genre) {
        streamGenre = genre;
        screenDirty = true;
    }
}

void clearStreamDetails() {
    streamGenre = "";
    streamFormat = "";
    broadcasterGenre = "";
    broadcasterStationName = "";
    lastStreamDetailsAt = millis() - 500;
    screenDirty = true;
}

// Numeric codec values match the vendored Audio 2.0.7 enum. Avoid getCodecname(),
// whose table has no entry for that version's CODEC_AACP value (9).
const char* streamCodecLabel(int codec) {
    switch (codec) {
    case 1: return "WAV";
    case 2: return "MP3";
    case 3: return "AAC";
    case 4: return "M4A";
    case 5: return "FLAC";
    case 6: return "OGG";
    case 7: return "FLAC";
    case 8: return "OPUS";
    case 9: return "AAC+";
    default: return "";
    }
}

void serviceStreamDetails() {
    if (!audioReady || !audio.isRunning()) return;
    if (!sdMode && (!radioStarted || !streamReady)) return;
    const uint32_t now = millis();
    if (now - lastStreamDetailsAt < 500) return;
    lastStreamDetailsAt = now;
    const char* detected = streamCodecLabel(audio.getCodec());
    if (streamFormat != detected) {
        streamFormat = detected;
        screenDirty = true;
    }
}

void drawScreen() {
	if (!screenReady) {
		return;
	}
	screen.setMusicState(sdMode, sdMode ? sdActive : (playbackRequested || radioStarted), sdPaused, currentMusic, musicFiles.size(),
	                     sdElapsed, sdDuration, static_cast<uint8_t>(sdRepeat), sdShuffle,
	                     currentMusic < musicFiles.size() ? musicFiles[currentMusic] : sourceResume.path,
	                     sdActive && sdDuration > 0 && audio.getBitRate(true) > 0);
	screen.update(stationName, trackTitle, statusText, streamGenre, streamFormat,
	              WiFi.status() == WL_CONNECTED,
	              volume, muted, currentStation, trackIsStatus);
	screenDirty = false;
}

// Bildirimlerde yalnızca metinleri güncelliyoruz.
// Ekrana çizim ana döngüde yapılıyor.
void audio_info(const char* info) {
    if (!info) return;
    Serial.printf("[SES] %s\n", info);
    if (sdMode) return;
    if (strstr(info, "stream ready") != nullptr) {
        if (!streamReady) streamReadyAt = millis();
        streamReady = true;
        statusText = "Yayın hazır";
        screenDirty = true;
    }
}

void audio_showstation(const char* info) {
    if (sdMode) return;
	Serial.printf("[ISTASYON] %s\n", info);

	if (info && info[0]) {
		broadcasterStationName = info;
		refreshStreamGenre(); // icy-name may arrive after icy-genre.
		stationName = stations[currentStation].name;
		screenDirty = true;
	}
}

void audio_icygenre(const char* info) {
    if (sdMode) return;
    String genre = info ? info : "";
    genre.replace('\r', ' ');
    genre.replace('\n', ' ');
    genre.replace('\t', ' ');
    genre.trim();
    broadcasterGenre = genre;
    Serial.printf("[TUR] %s\n", genre.c_str());
    refreshStreamGenre();
}

// Audio 2.0.7 reports HTTP failures through the stream-title callback.
// Keep protocol status out of song metadata and handle it after audio.loop().
int httpStatusCode(const char* line) {
    if (!line || strncmp(line, "HTTP/", 5) != 0) return 0;
    const char* code = strchr(line, ' ');
    if (!code) return 0;
    while (*code == ' ') ++code;
    if (strlen(code) < 3 || code[0] < '1' || code[0] > '5' ||
        code[1] < '0' || code[1] > '9' || code[2] < '0' || code[2] > '9') return 0;
    return (code[0] - '0') * 100 + (code[1] - '0') * 10 + code[2] - '0';
}

void audio_showstreamtitle(const char* info) {
    if (sdMode) return;
    const int code = httpStatusCode(info);
    if (code) {
        Serial.printf("[HTTP] %s\n", info);
        if (code >= 400) {
            httpFailureCode = code;
            String reason(info);
            reason.toLowerCase();
            httpFailureRetryable = code == 408 || code == 429 || code >= 500 ||
                (code == 403 && reason.indexOf("account already in use") >= 0);
            httpFailurePending = true;
            statusText = "HTTP " + String(code) + ": yayın isteği reddedildi";
            trackTitle = "Yayın başlatılamadı"; trackIsStatus = true;
            screenDirty = true;
        }
        return;
    }
    String title = info ? info : "";
    title.trim();
    if (title == "-") title = ""; // Empty artist/title separator is not song data.
    if (trackTitle == title && !trackIsStatus) return;
    Serial.printf("[PARCA] %s\n", title.c_str());
    trackTitle = title; trackIsStatus = false;
    screenDirty = true;
}

void audio_id3data(const char* info) {
    if (!sdMode || !sdActive || !info) return;
    String value;
    if (strncmp(info, "Title: ", 7) == 0) {
        value = info + 7; value.trim();
        if (value.length()) sdTitle = value;
    } else if (strncmp(info, "Artist: ", 8) == 0) {
        value = info + 8; value.trim(); sdArtist = value;
    } else return;
    value = sdArtist.length() ? sdArtist + " - " + sdTitle : sdTitle;
    if (trackTitle != value) { trackTitle = value; trackIsStatus = false; screenDirty = true; }
}

void audio_eof_mp3(const char*) {
    if (sdMode && sdActive) {
        sdElapsed = audio.getAudioCurrentTime();
        sdEofPending = true; screenDirty = true;
    }
}

void scheduleStreamRetry(const String& reason, bool retryable) {
    clearStreamDetails();
    radioStarted = false;
    streamReady = false;
    httpFailurePending = false;
    // stopSong alone leaves network clients open in 2.0.7. This closes them too.
    if (audioReady) audio.disconnectStream();
    if (!retryable || retryAttempt >= MAX_STREAM_RETRIES) {
        retryPending = false;
        playbackRequested = false;
        statusText = reason + "; listeden yeniden seçin";
        trackTitle = "Yayın başlatılamadı"; trackIsStatus = true;
        if (!sdMode && WiFi.status() == WL_CONNECTED && currentStation < stations.count()) {
            failedStationPromptUrl = stations[currentStation].url;
            failedStationPromptPending = true;
        }
    } else {
        const uint32_t delays[] = {3000, 10000, 30000};
        const uint32_t waitMs = delays[retryAttempt];
        retryAt = millis() + waitMs;
        retryPending = true;
        statusText = reason + ": " + String(waitMs / 1000) + " sn sonra tekrar";
        trackTitle = "Yeniden bağlantı " + String(retryAttempt + 1) + "/" + String(MAX_STREAM_RETRIES); trackIsStatus = true;
    }
    screenDirty = true;
}

// Network operations are performed outside LVGL event callbacks.
void startStation(size_t index, bool automaticRetry = false) {
	if (index >= stations.count()) return;
	if (sdMode) {
		updateSdTime(true);
		if (audioReady) audio.disconnectStream();
		sdMode = sdActive = sdPaused = sdEofPending = false;
	}
	if (!automaticRetry && index == currentStation && radioStarted &&
	    audio.isRunning() && !httpFailurePending) return;
	if (audioReady) audio.disconnectStream();
	if (!automaticRetry) {
		failedStationPromptPending = false;
		retryAttempt = 0;
		markPlaybackChanged();
	}
	playbackRequested = true;
	retryPending = false;
	httpFailurePending = false;
	httpFailureCode = 0;
	httpFailureRetryable = false;
	streamReady = false;
	clearStreamDetails();
	currentStation = index;
	stationName = stations[index].name;
	if (!automaticRetry) persistSourceState(true);
	trackTitle = "";
	screenDirty = true;
	if (!audioReady || WiFi.status() != WL_CONNECTED) {
		radioStarted = false;
		if (audioReady) scheduleStreamRetry("Wi-Fi bekleniyor", true);
		return;
	}

	radioStarted = false;
	statusText = "Yayın açılıyor...";
	drawScreen();
	screen.present();
	radioStarted = audio.connecttohost(stations[index].url);
	connectionStartedAt = millis();
	audio.setVolume(muted ? 0 : volume);
	if (!radioStarted) {
		Serial.println("HATA: Radyo baglantisi baslatilamadi.");
		scheduleStreamRetry("Yayın bağlantısı kurulamadı", true);
	}
	screenDirty = true;
}

void serviceStreamConnection() {
    if (stationDeletePending) return;
    if (sdMode || !audioReady || !playbackRequested) return;
    if (radioStarted) {
        if (httpFailurePending) {
            scheduleStreamRetry("HTTP " + String(httpFailureCode), httpFailureRetryable);
        } else if (!audio.isRunning()) {
            scheduleStreamRetry("Yayın bağlantısı kesildi", true);
        } else if (!streamReady && millis() - connectionStartedAt >= 20000) {
            scheduleStreamRetry("Yayın yanıt vermedi", true);
        } else if (streamReady && millis() - streamReadyAt >= 30000) {
            // Reset the budget only after stable playback, not merely TCP success.
            retryAttempt = 0;
        }
    }
    if (retryPending && static_cast<int32_t>(millis() - retryAt) >= 0 &&
        WiFi.status() == WL_CONNECTED) {
        retryPending = false;
        ++retryAttempt;
        Serial.printf("[YAYIN] Yeniden baglanti %u/%u\n", retryAttempt, MAX_STREAM_RETRIES);
        startStation(currentStation, true);
    }
}

std::vector<String> stationFiles;
std::vector<StationRecord> pendingStations;
String pendingStationFile;
bool importPrepared = false;

void serviceSdAudio() {
    if (radioStarted || (sdMode && sdActive)) audio.loop();
}

void updateSdTime(bool force = false) {
    if (!sdMode || !sdActive || sdEofPending) return;
    if (!force && (sdPaused || millis() - sdTimeCheckAt < 250)) return;
    sdTimeCheckAt = millis();
    const uint32_t pending = audio.getPendingFilePosition();
    const uint32_t elapsed = pending ? sdElapsed : audio.getAudioCurrentTime();
    const uint32_t duration = audio.getAudioFileDuration();
    if (elapsed != sdElapsed || (duration && duration != sdDuration)) {
        sdElapsed = elapsed;
        if (duration) sdDuration = duration; // Getter returns zero after EOF closes the file.
        screenDirty = true;
    }
}

void persistSourceState(bool force) {
    if (!audioReady || restoringPlayback) return;
    // No periodic checkpoint: only retry a failed source/track save after 10 s.
    if (!force && (!resumeSavePending || millis() - resumeSaveAt < 10000)) return;
    PlaybackResume next = sourceResume;
    next.sd = sdMode;
    if (sdMode && currentMusic < musicFiles.size()) {
        next.path = musicFiles[currentMusic];
    }
    sourceResume = next; resumeSaveAt = millis();
    resumeSavePending = !savePlaybackResume(next);
    if (resumeSavePending) Serial.println("[PLAYBACK] Son kaynak/parca kaydedilemedi; yeniden denenecek.");
}

void stopSdMusic(const String& reason) {
    updateSdTime(true);
    if (audioReady) audio.disconnectStream();
    sdActive = sdPaused = sdEofPending = false;
    statusText = reason; screenDirty = true;
}

bool sdCardReadable() {
    if (SD.cardType() == CARD_NONE) return false;
    uint8_t sector[512];
    return SD.readRAW(sector, 0); // Read-only card presence check; never formats/writes.
}

void makeSdOrder(bool anchorCurrent) {
    sdOrder.clear(); sdOrderPosition = 0;
    if (!sdShuffle || musicFiles.empty()) return;
    for (size_t i = 0; i < musicFiles.size(); ++i) sdOrder.push_back(i);
    for (size_t remaining = sdOrder.size(); remaining > 1; --remaining) {
        const size_t other = esp_random() % remaining;
        const size_t temporary = sdOrder[remaining - 1];
        sdOrder[remaining - 1] = sdOrder[other]; sdOrder[other] = temporary;
    }
    if (anchorCurrent) {
        for (size_t i = 0; i < sdOrder.size(); ++i) {
            if (sdOrder[i] == currentMusic) {
                const size_t temporary = sdOrder[0]; sdOrder[0] = sdOrder[i]; sdOrder[i] = temporary;
                break;
            }
        }
    } else if (sdOrder.size() > 1 && sdOrder[0] == currentMusic) {
        const size_t other = 1 + esp_random() % (sdOrder.size() - 1);
        const size_t temporary = sdOrder[0]; sdOrder[0] = sdOrder[other]; sdOrder[other] = temporary;
    }
}

void startSdMusic(size_t index, bool preserveOrder = false) {
    if (!audioReady || index >= musicFiles.size()) return;
    File probe = SD.open(musicFiles[index], FILE_READ);
    if (!probe || probe.isDirectory() || probe.size() == 0) {
        screen.showMusicFolder(musicFolder, musicEntries, "Dosya açılamadı veya boş. SD kartı kontrol edin."); return;
    }
    probe.close();
    audio.disconnectStream();
    radioStarted = streamReady = playbackRequested = false;
    retryPending = httpFailurePending = false;
    sdMode = true; sdActive = true; sdPaused = sdEofPending = false;
    currentMusic = index;
    if (!preserveOrder) makeSdOrder(true);
    sdElapsed = sdDuration = 0; sdTimeCheckAt = millis();
    clearStreamDetails(); streamFormat = "MP3";
    sdTitle = musicFiles[index].substring(musicFiles[index].lastIndexOf('/') + 1);
    sdTitle.remove(sdTitle.length() - 4); sdArtist = "";
    trackTitle = sdTitle; trackIsStatus = false; statusText = "MP3 açılıyor...";
    screenDirty = true; drawScreen(); screen.present();
    sdCardCheckAt = sdProgressAt = millis(); sdLastPosition = UINT32_MAX;
    const bool opened = audio.connecttoFS(SD, musicFiles[index].c_str());
    audio.setVolume(muted ? 0 : volume);
    if (opened) {
        statusText = "SD müzik çalıyor";
        persistSourceState(true);
    }
    else stopSdMusic("MP3 açılamadı; dosyayı veya kartı kontrol edin.");
    screenDirty = true;
}

void advanceSdMusic(bool backwards, bool automatic = false) {
    if (musicFiles.empty()) return;
    if (automatic && sdRepeat == SdRepeat::One) { startSdMusic(currentMusic, true); return; }
    size_t next = currentMusic;
    if (sdShuffle) {
        if (sdOrder.size() != musicFiles.size() || sdOrderPosition >= sdOrder.size() || sdOrder[sdOrderPosition] != currentMusic)
            makeSdOrder(true);
        if (backwards) sdOrderPosition = sdOrderPosition ? sdOrderPosition - 1 : sdOrder.size() - 1;
        else if (sdOrderPosition + 1 < sdOrder.size()) ++sdOrderPosition;
        else if (automatic && sdRepeat != SdRepeat::All) { stopSdMusic("Liste tamamlandı"); return; }
        else makeSdOrder(false);
        next = sdOrder[sdOrderPosition];
    } else if (backwards) next = (currentMusic + musicFiles.size() - 1) % musicFiles.size();
    else if (currentMusic + 1 < musicFiles.size()) next = currentMusic + 1;
    else if (automatic && sdRepeat != SdRepeat::All) { stopSdMusic("Liste tamamlandı"); return; }
    else next = 0;
    startSdMusic(next, true);
}

void toggleSdPause() {
    if (!sdActive) { if (!musicFiles.empty()) startSdMusic(currentMusic); return; }
    if (sdEofPending) return;
    updateSdTime(true);
    if (audio.pauseResume()) {
        sdPaused = !sdPaused;
        sdProgressAt = millis();
        statusText = sdPaused ? "Duraklatıldı" : "SD müzik çalıyor";
        screenDirty = true;
    }
}

void serviceSdMusic() {
	if (!sdMode || !sdActive) return;
	updateSdTime();
	const uint32_t now = millis();
	if (!sdPaused && !sdEofPending && audio.isRunning()) {
		const uint32_t position = audio.getFilePos();
		if (position != sdLastPosition) {
			sdLastPosition = position;
			sdProgressAt = sdCardCheckAt = now;
			return;
		}
	}
	if (now - sdCardCheckAt >= 2000 || sdEofPending) {
		sdCardCheckAt = now;
		if (!sdCardReadable()) { stopSdMusic("SD kart çıkarıldı veya okunamadı."); return; }
	}
	if (sdPaused) return;
	if (sdEofPending) {
		sdEofPending = false;
		advanceSdMusic(false, true);
		return;
	}
	if (!audio.isRunning()) { stopSdMusic("MP3 çalınamadı; başka dosya seçin."); return; }
	if (now - sdProgressAt >= 30000) stopSdMusic("Dosya okunması ilerlemiyor; SD kartı kontrol edin.");
}

void browseMusicFolder(const String& folder) {
    screen.showMusicFolder(musicFolder, {}, "SD kart taranıyor...");
    screen.present();
    std::vector<SdMusicEntry> candidate; String error;
    if (!listSdMusicFolder(folder, candidate, error, !(sdMode && sdActive), serviceSdAudio)) {
        screen.showMusicFolder(musicFolder, musicEntries, error); return;
    }
    musicFolder = folder; musicEntries.swap(candidate);
    screen.showMusicFolder(musicFolder, musicEntries, musicEntries.empty() ?
        "Bu klasörde MP3 veya alt klasör yok." : "Klasöre girmek veya çalmak için dokunun.");
}

void openMusicFiles() { browseMusicFolder(musicFolder); }

void selectMusicPath(const String& path) {
    for (const auto& entry : musicEntries) {
        if (entry.path != path) continue;
        if (entry.directory) { browseMusicFolder(path); return; }
        File probe = SD.open(path, FILE_READ);
        if (!probe || probe.isDirectory() || !probe.size()) {
            screen.showMusicFolder(musicFolder, musicEntries, "Dosya açılamadı veya boş."); return;
        }
        probe.close();
        std::vector<String> candidate;
        size_t selected = 0;
        for (const auto& item : musicEntries) if (!item.directory) {
            if (item.path == path) selected = candidate.size();
            candidate.push_back(item.path);
        }
        updateSdTime(true);
        audio.disconnectStream();
        sdActive = sdPaused = sdEofPending = false;
        radioStarted = streamReady = playbackRequested = false;
        retryPending = httpFailurePending = false;
        sdMode = true;
        musicFiles.swap(candidate); currentMusic = selected;
        startSdMusic(selected);
        return;
    }
}

void seekSdMusic(int delta) {
    if (!sdMode || !sdActive || !sdDuration || sdEofPending) return;
    updateSdTime(true);
    int64_t target = int64_t(sdElapsed) + delta;
    if (target < 0) target = 0;
    if (target >= sdDuration) target = sdDuration - 1;
    if (!audio.setAudioPlayPosition(uint32_t(target))) return;
    sdElapsed = uint32_t(target);
    sdProgressAt = millis();
    screenDirty = true;
}

void restoreSdPlayback(bool sdMounted = false) {
	sdMode = true; streamFormat = "MP3";
	trackTitle = sourceResume.path.substring(sourceResume.path.lastIndexOf('/') + 1);
	trackIsStatus = false;
	if (trackTitle.endsWith(".mp3") || trackTitle.endsWith(".MP3")) trackTitle.remove(trackTitle.length() - 4);
	musicFolder = sdParentFolder(sourceResume.path);
	String error;
	if (!listSdMusicFolder(musicFolder, musicEntries, error, !sdMounted, serviceSdAudio)) {
		statusText = "Son MP3 açılamadı; SD kartı kontrol edin.";
		screenDirty = true; return;
	}
	bool found = false;
	for (const auto& entry : musicEntries) if (!entry.directory) {
		if (entry.path == sourceResume.path) { currentMusic = musicFiles.size(); found = true; }
		musicFiles.push_back(entry.path);
	}
	if (!found) {
		musicFiles.clear(); statusText = "Son MP3 bulunamadı; listeden dosya seçin.";
		screenDirty = true; return;
	}
	File probe = SD.open(sourceResume.path, FILE_READ);
	const bool readable = probe && !probe.isDirectory() && probe.size();
	probe.close();
	if (!readable) {
		musicFiles.clear(); statusText = "Son MP3 okunamadı; dosya seçin.";
		screenDirty = true; return;
	}
	restoringPlayback = true;
	startSdMusic(currentMusic);
	restoringPlayback = false;
	if (sdActive) persistSourceState(true);
}

// Only recalculate filters when a band changes; unrelated UI previews leave DSP alone.
void applyEqualizer(const RadioSettings& value) {
    static int8_t bass = 127, mid = 127, treble = 127;
    if (!audioReady) return;
    if (bass == value.bass && mid == value.mid && treble == value.treble) return;
    audio.setTone(value.bass, value.mid, value.treble);
    bass = value.bass;
    mid = value.mid;
    treble = value.treble;
}

void pauseForWifi() {
    if (sdMode) { lastWifiConnected = false; return; }
    if (audioReady) audio.disconnectStream();
    radioStarted = streamReady = false;
    retryPending = httpFailurePending = false;
    clearStreamDetails(); trackTitle = "";
    statusText = "Wi-Fi bağlantısı bekleniyor...";
    lastWifiConnected = false;
    screenDirty = true;
}

void openWifiSetup() {
    if (network.setupOpen()) return;
    network.openSetup();
    screen.openWifi(network.savedSsid());
    network.scan();
    screen.updateWifi(network.message(), network.busy(), network.scanning());
}

void requestStationDeletion(size_t index, bool unavailable) {
    if (index >= stations.count() || stationDeletePending || settingsEditing ||
        network.setupOpen() || otaMode || sdMode) return;
    stationDeletePending = true;
    stationDeleteUrl = stations[index].url;
    stationDeleteIdentity = stations.identity();
    failedStationPromptPending = false;
    screen.showStationDelete(stations[index].name, unavailable, stations.count() > 1);
}

void serviceFailedStationPrompt() {
    if (!failedStationPromptPending) return;
    if (sdMode || WiFi.status() != WL_CONNECTED || currentStation >= stations.count() ||
        failedStationPromptUrl != stations[currentStation].url) {
        failedStationPromptPending = false;
        return;
    }
    if (!settingsEditing && !network.setupOpen() && !otaMode && !stationDeletePending)
        requestStationDeletion(currentStation, true);
}

void confirmStationDeletion() {
    if (!stationDeletePending) return;
    if (stations.identity() != stationDeleteIdentity) {
        screen.showStationDeleteError("İstasyon listesi değişti. Vazgeç ile kapatıp yeniden seçin.");
        return;
    }
    if (stations.count() <= 1) {
        screen.showStationDeleteError("Son istasyon silinemez. Önce başka bir istasyon ekleyin.");
        return;
    }
    size_t removing = stations.count();
    for (size_t i = 0; i < stations.count(); ++i)
        if (stationDeleteUrl == stations[i].url) { removing = i; break; }
    if (removing == stations.count()) {
        screen.showStationDeleteError("İstasyon listesi değişti. Vazgeç ile kapatıp yeniden seçin.");
        return;
    }
    const std::string playingUrl = stations[currentStation].url;
    const std::string bootUrl = stations[settings.bootStation].url;
    const bool removedCurrent = stationDeleteUrl == playingUrl.c_str();
    std::vector<StationRecord> candidate;
    for (size_t i = 0; i < stations.count(); ++i)
        if (i != removing) candidate.push_back({stations[i].name, stations[i].url});
    size_t selected = 0, boot = 0;
    for (size_t i = 0; i < candidate.size(); ++i) {
        if (candidate[i].url == playingUrl) selected = i;
        if (candidate[i].url == bootUrl) boot = i;
    }
    if (removedCurrent) selected = removing < candidate.size() ? removing : candidate.size() - 1;
    String error;
    serviceSdAudio();
    if (!stations.replace(candidate, error)) {
        screen.showStationDeleteError(error);
        return; // Keep the confirmation and current playback intact on failure.
    }
    currentStation = selected;
    if (removedCurrent) {
        if (audioReady) audio.disconnectStream();
        playbackRequested = radioStarted = streamReady = false;
        retryPending = httpFailurePending = false;
        clearStreamDetails();
        trackTitle = ""; trackIsStatus = false;
        statusText = "İstasyon silindi; başka bir istasyon seçin.";
    }
    stationName = stations[currentStation].name;
    refreshStreamGenre();
    settings.bootStation = boot;
    previewSettings = settings;
    setStationSettingsIdentity(stations.identity());
    const bool configSaved = saveRadioSettings(settings);
    const bool playbackSaved = saveLastPlayback(volume, currentStation);
    playbackDirty = !playbackSaved; playbackChangedAt = millis();
    screen.setStations(stations.data(), stations.count());
    screen.setFavorites(stationFavorites.mask(stations.data(), stations.count()));
    screen.configureSettings(settings);
    stationDeletePending = failedStationPromptPending = false;
    stationDeleteUrl = "";
    screen.closeStationDelete(true);
    if (!configSaved || !playbackSaved)
        statusText = "Liste kaydedildi; açılış tercihleri tamamen kaydedilemedi. Yeniden açılışta ilk istasyon kullanılabilir.";
    screenDirty = true;
    drawScreen();
}

// Both touch and HTTP commands enter here, always on the main loop.
void processCommand(const RadioCommand& command) {
    if (stationDeletePending && command.action != RadioAction::ConfirmDeleteStation &&
        command.action != RadioAction::CancelDeleteStation) return;
    bool changed = false;
        switch (command.action) {
        case RadioAction::RequestDeleteStation:
            requestStationDeletion(command.stationIndex, false);
            break;
        case RadioAction::CancelDeleteStation:
            stationDeletePending = false;
            stationDeleteUrl = "";
            screen.closeStationDelete();
            break;
        case RadioAction::ConfirmDeleteStation:
            confirmStationDeletion();
            break;
        case RadioAction::ToggleRepeatOne:
            if (sdMode) { sdRepeat = sdRepeat == SdRepeat::One ? SdRepeat::Off : SdRepeat::One; screenDirty = true; }
            break;
        case RadioAction::ToggleRepeatAll:
            if (sdMode) { sdRepeat = sdRepeat == SdRepeat::All ? SdRepeat::Off : SdRepeat::All; screenDirty = true; }
            break;
        case RadioAction::ToggleShuffle:
            if (sdMode) { sdShuffle = !sdShuffle; makeSdOrder(true); screenDirty = true; }
            break;
        case RadioAction::OpenMusic:
        case RadioAction::ScanMusic:
            openMusicFiles();
            break;
        case RadioAction::SelectMusic:
            selectMusicPath(command.musicPath);
            changed = true;
            break;
        case RadioAction::MusicParent:
            browseMusicFolder(sdParentFolder(musicFolder));
            break;
		case RadioAction::MusicSeekBack:
			seekSdMusic(-10);
			break;
		case RadioAction::MusicSeekForward:
			seekSdMusic(10);
			break;
		case RadioAction::ToggleMusicPause:
			if (sdMode) {
				toggleSdPause();
			} else if (playbackRequested || radioStarted) {
				if (audioReady) audio.disconnectStream();
				playbackRequested = radioStarted = streamReady = false;
				retryPending = httpFailurePending = false;
				clearStreamDetails();
				trackTitle = "";
				statusText = "Yayın durduruldu";
			} else {
				startStation(currentStation);
			}
			changed = true;
			break;
		case RadioAction::SwitchRadio:
            startStation(currentStation);
            changed = true;
            break;
        case RadioAction::OpenWifi:
            openWifiSetup();
            break;
        case RadioAction::CloseWifi:
            network.closeSetup();
            screen.closeWifi();
            break;
        case RadioAction::ScanWifi:
            network.scan();
            screen.updateWifi(network.message(), network.busy(), network.scanning());
            break;
        case RadioAction::ConnectWifi:
            if (network.connect(command.wifiSsid, command.wifiPassword)) {
                pauseForWifi();
                screen.clearWifiPassword();
            }
            screen.updateWifi(network.message(), network.busy(), network.scanning());
            break;
        case RadioAction::VolumeDown:
            if (volume > 0) --volume;
            muted = false;
            changed = true;
            break;
        case RadioAction::ToggleFavorite: {
            if (sdMode || currentStation >= stations.count()) break;
            String error;
            if (stationFavorites.toggle(stations.data(), stations.count(), currentStation, error)) {
                screen.setFavorites(stationFavorites.mask(stations.data(), stations.count()));
            } else {
                statusText = error;
            }
            // No volume/source change or secondary playback/settings write.
            screenDirty = true;
            drawScreen();
            break;
        }
        case RadioAction::VolumeUp:
            if (volume < (settingsEditing ? previewSettings.maxVolume : settings.maxVolume)) ++volume;
            muted = false;
            changed = true;
            break;
        case RadioAction::ToggleMute:
            muted = !muted;
            changed = true;
            break;
        case RadioAction::OpenImport: {
            if (sdMode) { screen.showImportMessage("CSV yüklemek için önce Radyo kaynağına dönün."); break; }
            importPrepared = false;
            pendingStations.clear();
            pendingStationFile = "";
            screen.showImportMessage("SD kart taranıyor...");
            screen.present();
            String error;
            if (listSdStationFiles(stationFiles, error, serviceSdAudio))
                screen.showImportFiles(stationFiles, "Yüklemek istediğiniz CSV dosyasını seçin.\nYalnızca SD kartın kök dizini listelenir.");
            else screen.showImportMessage(error);
            break;
        }
        case RadioAction::SelectImportFile: {
            if (command.stationIndex >= stationFiles.size()) break;
            importPrepared = false; pendingStations.clear();
            pendingStationFile = stationFiles[command.stationIndex];
            String error;
            screen.showImportMessage("Dosya okunuyor ve doğrulanıyor...");
            screen.present();
            if (readSdStationFile(pendingStationFile, pendingStations, error, serviceSdAudio)) {
                importPrepared = true;
                screen.showImportPreview(pendingStationFile, pendingStations.size());
            } else screen.showImportFiles(stationFiles, error + "\nMevcut liste korundu.");
            break;
        }
        case RadioAction::CancelImport:
            importPrepared = false; pendingStations.clear(); stationFiles.clear();
            screen.closeImport();
            break;
        case RadioAction::ConfirmImport: {
            if (!importPrepared) break;
            importPrepared = false; // Prevent queued double taps from saving twice.
            const bool continuePlayback = playbackRequested;
            String error;
            serviceSdAudio();
            if (!stations.replace(pendingStations, error)) {
                pendingStations.clear();
                screen.showImportMessage(error);
                break;
            }
            // The catalog is committed. Indices from the previous catalog are invalid.
            if (audioReady) audio.disconnectStream();
            radioStarted = false; streamReady = false;
            clearStreamDetails();
            retryPending = false; httpFailurePending = false; playbackRequested = false;
            currentStation = 0;
            settings.bootStation = 0; previewSettings.bootStation = 0;
            setStationSettingsIdentity(stations.identity());
            const bool settingsSaved = saveRadioSettings(settings);
            const bool playbackSaved = saveLastPlayback(settingsEditing ? volumeBeforeSettings : volume, currentStation);
            playbackDirty = !playbackSaved; playbackChangedAt = millis();
            screen.setStations(stations.data(), stations.count());
            screen.setFavorites(stationFavorites.mask(stations.data(), stations.count()));
            screen.configureSettings(settings);
            screen.setVolumeLimit(settingsEditing ? previewSettings.maxVolume : settings.maxVolume);
            stationName = stations[0].name;
            trackTitle = "";
            statusText = "Yeni liste hazır; istasyon seçin";
            pendingStations.clear(); stationFiles.clear();
            String result = String(stations.count()) + " istasyon kalıcı belleğe kaydedildi.\nSD kartı çıkarabilirsiniz.\nYeni listede ilk istasyon seçildi.";
            if (!settingsSaved || !playbackSaved) result += "\nAçılış seçimi kaydedilemedi; ilk istasyon kullanılır.";
            screen.showImportMessage(result);
            screenDirty = true;
            drawScreen();
            if (continuePlayback) startStation(0);
            break;
        }
        case RadioAction::BeginSettings:
            volumeBeforeSettings = volume;
            mutedBeforeSettings = muted;
            previewSettings = settings;
            settingsEditing = true;
            break;
        case RadioAction::PreviewSettings: {
            RadioSettings next = command.settings;
            next.validate(stations.count());
            const uint8_t oldVolume = volume;
            const bool oldMuted = muted;
            if (next.bootVolume != previewSettings.bootVolume) {
                volume = next.bootVolume;
                muted = false;
            }
            previewSettings = next;
            applyEqualizer(next);
            if (volume > next.maxVolume) volume = next.maxVolume;
            screen.applyScreenSettings(next);
            screen.setVolumeLimit(next.maxVolume);
            // The settings widgets already redraw themselves. Brightness-only
            // previews must not redraw the hidden home screen or reset audio gain.
            changed = changed || volume != oldVolume || muted != oldMuted;
            break;
        }
        case RadioAction::SaveSettings: {
            RadioSettings next = command.settings;
            next.validate(stations.count());
            if (saveRadioSettings(next)) {
                settings = next;
                applyEqualizer(settings);
                settingsEditing = false;
                if (volume > settings.maxVolume) volume = settings.maxVolume;
                screen.configureSettings(settings);
                screen.applyScreenSettings(settings);
                // Commit the previewed playback value on an explicit Save too.
                saveLastPlayback(volume, currentStation);
                screen.finishSettings(true);
                changed = true;
            } else {
                screen.finishSettings(false, "Kaydedilemedi. Tekrar deneyin veya Vazgeç.");
            }
            break;
        }
        case RadioAction::CancelSettings:
            applyEqualizer(settings);
            if (settingsEditing) {
                volume = volumeBeforeSettings;
                muted = mutedBeforeSettings;
            }
            settingsEditing = false;
            screen.configureSettings(settings);
            screen.applyScreenSettings(settings);
            screen.finishSettings(true);
            changed = true;
            break;
        case RadioAction::PreviousStation:
            if (sdMode) {
                advanceSdMusic(true);
            } else if (stations.count() > 1) startStation((currentStation + stations.count() - 1) % stations.count());
            changed = true;
            break;
        case RadioAction::NextStation:
            if (sdMode) {
                advanceSdMusic(false);
            } else if (stations.count() > 1) startStation((currentStation + 1) % stations.count());
            changed = true;
            break;
        case RadioAction::SelectStation:
            startStation(command.stationIndex);
            changed = true;
            break;
        case RadioAction::StartPlayback:
            if (sdMode) { if (!sdActive || sdPaused) toggleSdPause(); }
            else startStation(currentStation);
            changed = true;
            break;
        case RadioAction::PausePlayback:
            if (sdMode) {
                if (sdActive && !sdPaused) { toggleSdPause(); changed = true; }
                break;
            }
            // A live radio cannot retain a paused position; pause closes it.
            // Fall through to StopPlayback without starting an idle station.
        case RadioAction::StopPlayback:
            failedStationPromptPending = false;
            if (sdMode) { stopSdMusic("Durduruldu"); changed = true; break; }
            if (audioReady) audio.disconnectStream();
            playbackRequested = radioStarted = streamReady = false;
            retryPending = httpFailurePending = false;
            clearStreamDetails();
            trackTitle = "";
            statusText = "Yayın durduruldu";
            changed = true;
            break;
        default:
            break;
        }
    if (changed) {
        if (!settingsEditing) markPlaybackChanged();
        if (audioReady) audio.setVolume(muted ? 0 : volume);
        screenDirty = true;
        drawScreen();
    }
}

void processCommands() {
    RadioCommand command;
    while (screen.takeCommand(command)) if (!otaMode) processCommand(command);
}

std::vector<StationRecord> webStationRecords() {
    std::vector<StationRecord> records;
    for (size_t i = 0; i < stations.count(); ++i)
        records.push_back({stations[i].name, stations[i].url});
    return records;
}

void webMessage(RadioWeb& web, int status, const std::string& message, const char* code = "") {
    web.reply(status, "application/json; charset=utf-8",
        "{\"message\":" + radioJson(message) + ",\"code\":" + radioJson(code) + "}");
}

void finishOtaFailure(const char* reason = nullptr) {
	otaMode = false;
	trackTitle = "";
	trackIsStatus = false;
	statusText = reason ? String(reason) : (radioOta.error().empty() ?
		String("Güncelleme kesildi. Yeniden deneyin; oynatmayı Başlat ile sürdürün.") : String(radioOta.error().c_str()));
	screen.endOta();
	screenDirty = true;
	drawScreen();
}

bool handleFirmwareUpload(RadioWeb& web, RadioWeb::Upload phase, const uint8_t* data, size_t size) {
	if (phase == RadioWeb::Upload::Begin) {
		if (otaMode || settingsEditing || network.setupOpen() || stationDeletePending) {
			webMessage(web, 409, "Güncelleme başlamadı. Cihazdaki ayarları kapatın veya süren güncellemeyi bekleyin.");
			return false;
		}
		if (web.contentLength() < 288 || web.contentLength() > radioOta.capacity()) {
			webMessage(web, 400, "Firmware boyutu uygun değil veya OTA alanı bulunamadı.");
			return false;
		}
		audio.disconnectStream();
		radioStarted = streamReady = playbackRequested = false;
		sdActive = sdPaused = sdEofPending = false;
		retryPending = httpFailurePending = false;
		otaMode = true;
		trackTitle = "Firmware güncellemesi"; trackIsStatus = true;
		statusText = "Firmware yükleniyor: %0";
		screen.showOtaNotice(false);
		web.deferUpload(3000); // Return to the main loop; TCP naturally back-pressures.
		return true;
	}
	if (phase == RadioWeb::Upload::Abort) {
		radioOta.abort();
		if (otaMode && !radioOta.pendingRestart())
			finishOtaFailure("Güncelleme kesildi. Yeniden deneyin; oynatmayı Başlat ile sürdürün.");
		return false;
	}
	if (phase == RadioWeb::Upload::Data && !radioOta.active()) {
		// The notice has been visible for three seconds. All flash operations
		// start only after the backlight is off, including NVS persistence.
		screen.beginOta();
		if (!radioOta.begin(web.contentLength())) {
			webMessage(web, 400, radioOta.error());
			finishOtaFailure();
			return false;
		}
		persistSourceState(true);
		saveLastPlayback(volume, currentStation);
	}
	const bool ok = phase == RadioWeb::Upload::Data ? radioOta.write(data, size) : radioOta.finish();
	if (!ok) { webMessage(web, 400, radioOta.error()); finishOtaFailure(); return false; }
	if (phase == RadioWeb::Upload::End) {
		statusText = "Güncelleme doğrulandı. Yeniden başlatılıyor...";
		screen.showOtaNotice(true); // esp_ota_end/set_boot_partition have finished.
		webMessage(web, 200, "Firmware doğrulandı. Cihaz yeniden başlatılıyor.");
	}
	return true;
}

// Saved indices are remapped by URL; an unchanged playing stream stays open.
void saveWebStations(RadioWeb& web, const std::vector<StationRecord>& candidate) {
    if (web.revision != std::to_string(stations.identity())) {
        webMessage(web, 409, "Cihazdaki liste değişti; taslak kaydedilmedi. Değişikliklerinizi not aldıktan sonra Değişiklikleri geri al ile güncel listeyi alın.");
        return;
    }
    const std::string oldUrl = stations[currentStation].url;
    const std::string bootUrl = stations[settings.bootStation].url;
    size_t selected = 0, boot = 0;
    bool retained = false;
    for (size_t i = 0; i < candidate.size(); ++i) {
        if (candidate[i].url == oldUrl) { selected = i; retained = true; }
        if (candidate[i].url == bootUrl) boot = i;
    }
    if (playbackRequested && !retained && web.confirm != "stop") {
        webMessage(web, 409, "Çalan veya bağlantısı beklenen istasyon yeni listede yok. Liste kaydedilecek ve yayın duracak. Devam edilsin mi?", "playing_removed");
        return;
    }
    if (writeStationCsv(candidate) == writeStationCsv(webStationRecords())) {
        webMessage(web, 200, "Liste zaten kayıtlı; değişiklik yok."); return;
    }
    String error;
    serviceSdAudio();
    if (!stations.replace(candidate, error)) { webMessage(web, 500, error.c_str()); return; }
    currentStation = selected;
    if (!retained) {
        if (audioReady) audio.disconnectStream();
        playbackRequested = radioStarted = streamReady = false;
        retryPending = httpFailurePending = false;
        clearStreamDetails(); trackTitle = "";
        statusText = "Yeni liste hazır; istasyon seçin";
    }
    stationName = stations[currentStation].name;
    refreshStreamGenre();
    settings.bootStation = boot;
    previewSettings = settings;
    setStationSettingsIdentity(stations.identity());
    const bool configSaved = saveRadioSettings(settings);
    const bool playbackSaved = saveLastPlayback(volume, currentStation);
    playbackDirty = !playbackSaved; playbackChangedAt = millis();
    screen.setStations(stations.data(), stations.count());
    screen.setFavorites(stationFavorites.mask(stations.data(), stations.count()));
    screen.configureSettings(settings);
    screenDirty = true; drawScreen();
    webMessage(web, 200, configSaved && playbackSaved ? "Liste cihaza kaydedildi." :
        "Liste kaydedildi; açılış tercihleri tamamen kaydedilemedi. Yeniden açılışta ilk istasyon kullanılabilir.");
}

void handleWebRequest(RadioWeb& web) {
	if (web.method == "GET" && web.path == "/api/firmware") {
		web.reply(200, "application/json; charset=utf-8",
			"{\"version\":" + radioJson(RADIO_VERSION) + ",\"product\":" + radioJson(RADIO_FIRMWARE_ID) +
			",\"maxSize\":" + std::to_string(radioOta.capacity()) + "}"); return;
	}
	if (web.method == "GET" && web.path == "/") { web.replyPage(); return; }
	if (web.method == "GET" && web.path == "/api/state") {
		std::string result = "{\"version\":" + radioJson(RADIO_VERSION) +
			",\"deviceId\":" + radioJson(radioDiscovery.id().c_str()) +
			",\"deviceName\":" + radioJson(radioDiscovery.name().c_str()) +
			",\"token\":" + radioJson(web.token().c_str()) +
			",\"revision\":" + radioJson(std::to_string(stations.identity())) +
			",\"source\":" + radioJson(sdMode ? "sd" : "radio") +
			",\"station\":" + radioJson(sdMode ? "SD KART" : stationName.c_str()) +
			",\"track\":" + radioJson(trackTitle.c_str()) +
			",\"status\":" + radioJson(statusText.c_str()) +
			",\"genre\":" + radioJson(streamGenre.c_str()) +
			",\"format\":" + radioJson(streamFormat.c_str()) +
			",\"volume\":" + std::to_string(volume) +
			",\"maxVolume\":" + std::to_string(settings.maxVolume) +
			",\"muted\":" + (muted ? "true" : "false") +
			",\"playing\":" + ((sdMode ? sdActive && !sdPaused : radioStarted) ? "true" : "false") +
			",\"editing\":" + ((settingsEditing || network.setupOpen() || otaMode || stationDeletePending) ? "true" : "false") +
			",\"current\":" + std::to_string(currentStation) + ",\"stations\":[";
		for (size_t i = 0; i < stations.count(); ++i) {
			if (i) result += ',';
			result += radioJson(stations[i].name);
		}
		web.reply(200, "application/json; charset=utf-8", result + "]}"); return;
	}
	if (web.method == "GET" && web.path == "/api/stations") {
		web.reply(200, "application/json; charset=utf-8",
			"{\"revision\":" + radioJson(std::to_string(stations.identity())) +
			",\"csv\":" + radioJson(writeStationCsv(webStationRecords())) + "}"); return;
	}
	if (web.method != "POST") { webMessage(web, 404, "Adres bulunamadı."); return; }
	if (stationDeletePending) {
		webMessage(web, 409, "Cihazda silme onayı açık. Önce Sil veya Vazgeç seçin."); return;
	}
	if (otaMode) { webMessage(web, 409, "Firmware güncelleniyor. Cihazın yeniden başlamasını bekleyin."); return; }
	if (settingsEditing || network.setupOpen()) {
		webMessage(web, 409, "Cihazda ayarlar açık. Önce cihazdaki Kaydet veya Vazgeç ile menüyü kapatın."); return;
	}
	if (web.path == "/api/validate" || web.path == "/api/stations") {
		if (sdMode && web.path == "/api/stations") {
			webMessage(web, 409, "SD müzik modunda istasyon listesi kaydedilemez. Önce Radyo kaynağına dönün."); return;
		}
		std::vector<StationRecord> candidate; std::string error;
		if (!parseStationCsv(web.body, candidate, error)) { webMessage(web, 400, error); return; }
		if (web.path == "/api/validate")
			web.reply(200, "application/json; charset=utf-8", "{\"csv\":" + radioJson(writeStationCsv(candidate)) + "}");
		else saveWebStations(web, candidate);
		return;
	}
	if (web.path == "/api/command") {
		RadioCommand command;
		if (web.body == "volumeDown") command.action = RadioAction::VolumeDown;
		else if (web.body == "volumeUp") command.action = RadioAction::VolumeUp;
		else if (web.body == "mute") command.action = RadioAction::ToggleMute;
		else if (web.body == "previous") command.action = RadioAction::PreviousStation;
		else if (web.body == "next") command.action = RadioAction::NextStation;
		else if (web.body == "play") command.action = RadioAction::StartPlayback;
		else if (web.body == "pause") command.action = RadioAction::PausePlayback;
		else if (web.body == "toggle") command.action = RadioAction::ToggleMusicPause;
		else if (web.body == "stop") command.action = RadioAction::StopPlayback;
		else if (web.body == "radio") command.action = RadioAction::SwitchRadio;
		else if (web.body.compare(0, 7, "select:") == 0) {
			if (web.revision != std::to_string(stations.identity())) {
				webMessage(web, 409, "İstasyon listesi değişti; sayfanın yenilenmesini bekleyin."); return;
			}
			const std::string index = web.body.substr(7);
			if (index.empty() || index.size() > 2) { webMessage(web, 400, "Geçersiz istasyon."); return; }
			size_t selected = 0;
			for (char c : index) {
				if (c < '0' || c > '9') { webMessage(web, 400, "Geçersiz istasyon."); return; }
				selected = selected * 10 + c - '0';
			}
			if (selected >= stations.count()) { webMessage(web, 400, "İstasyon bulunamadı."); return; }
			command.action = RadioAction::SelectStation; command.stationIndex = selected;
		} else { webMessage(web, 400, "Bilinmeyen kumanda komutu."); return; }
		processCommand(command);
		webMessage(web, 200, "Komut uygulandı."); return;
	}
	webMessage(web, 404, "Adres bulunamadı.");
}

void serviceWifi() {
    network.loop();
    // Keep a pending setup request until the deletion confirmation is closed.
    if (!stationDeletePending && network.takeSetupRequest() && !sdMode && !otaMode) openWifiSetup();
    std::vector<String> names;
    if (network.takeScanResults(names)) screen.showWifiNetworks(names);
    const bool connected = network.connected();
    if (connected && !lastWifiConnected) {
        lastWifiConnected = true;
        radioWeb.begin(handleWebRequest, handleFirmwareUpload);
        Serial.print("Wi-Fi baglandi. IP: "); Serial.println(WiFi.localIP());
        if (!sdMode) {
            if (playbackRequested) startStation(currentStation);
            else { statusText = "Başlatmak için istasyon seçin"; screenDirty = true; }
        }
    } else if (!connected && lastWifiConnected) pauseForWifi();
    radioDiscovery.loop(connected);
    if (network.setupOpen()) screen.updateWifi(network.message(), network.busy(), network.scanning());
}

void setup() {
	Serial.begin(115200);
	delay(200);

	Serial.println("\n--- Internet Radyo: Ekran ve Ses ---");
	Serial.printf("PSRAM: %u bayt\n",
		(unsigned int)ESP.getPsramSize());

	if (!psramFound()) {
		Serial.println("HATA: PSRAM bulunamadi.");
		return;
	}

	stations.load();
	stationFavorites.load();
	setStationSettingsIdentity(stations.identity());
	loadRadioSettings(settings, stations.count());
	setRadioLanguage(settings.language);
	uint8_t lastVolume = settings.bootVolume;
	size_t lastStation = settings.bootStation;
	loadLastPlayback(lastVolume, lastStation, stations.count());
	loadPlaybackResume(sourceResume);
	sdMode = sourceResume.sd;
	volume = settings.rememberVolume ? lastVolume : settings.bootVolume;
	if (volume > settings.maxVolume) volume = settings.maxVolume;
	currentStation = settings.resumeStation ? lastStation : settings.bootStation;
	stationName = stations[currentStation].name;

	Serial.println("Ekran baslatiliyor...");

	screenReady = screen.init(stations.data(), stations.count());

	if (!screenReady) {
		Serial.println("HATA: Ekran baslatilamadi.");
		return;
	}

	screen.configureSettings(settings);
	screen.applyScreenSettings(settings);
	screen.setFavorites(stationFavorites.mask(stations.data(), stations.count()));
	drawScreen();

	Serial.println("Ekran baslatildi.");

	// Check optional storage before starting the stream; missing media is allowed.
	screen.present();
	const bool sdMounted = runSdDiagnostics();

	if (!audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT)) {
		Serial.println("HATA: I2S ses pinleri baslatilamadi.");
		return; // Keep audioReady false so an unconfirmed OTA can roll back.
	}
	audio.forceMono(true);
	audio.setVolume(muted ? 0 : volume);
	audioReady = true;
	applyEqualizer(settingsEditing ? previewSettings : settings);
	playbackRequested = !sdMode && settings.autoPlay;
	if (sdMode) restoreSdPlayback(sdMounted);
	network.begin();
	if (!sdMode) {
		statusText = network.message();
		trackTitle = settings.autoPlay ? "" : "Yayın bekliyor";
		trackIsStatus = true;
	}
	screenDirty = true;
	drawScreen();
	serviceWifi();
}

void loop() {
	radioOta.checkBoot(screenReady && audioReady);
	if (!screenReady || !audioReady) { delay(1); return; }
	if (otaMode) {
		// Keep network alive during OTA while avoiding continuous LVGL timer redraws.
		processCommands();
		serviceWifi();
		radioWeb.loop();
		if (radioOta.restartReady()) ESP.restart();
		delay(1);
		return;
	}
	// Feed the audio DMA before servicing touch and potentially expensive drawing.
	if (radioStarted || (sdMode && sdActive)) audio.loop();
	screen.loop();
	serviceVu(); // Observe backlight-off/wake changes before the next audio feed.
	processCommands();
	serviceWifi();
	radioWeb.loop();
	if (otaMode) { delay(1); return; }
	serviceSdMusic();
	persistSourceState(false);
	serviceStreamConnection();
	serviceFailedStationPrompt();
	serviceStreamDetails();
	persistPlayback();
	if (radioStarted || (sdMode && sdActive)) {
		audio.loop();
	}

	// Yalnızca bilgiler değiştiğinde ekranı yenile.
	static uint32_t lastDraw = 0;

	if (screenDirty && millis() - lastDraw >= 200) {
		drawScreen();
		lastDraw = millis();
	}

	delay(1);
}
