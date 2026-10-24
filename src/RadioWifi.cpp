#include "RadioWifi.h"
#include <WiFi.h>
#include <Preferences.h>
#include <esp_wifi.h>
#include <cstring>

namespace {
struct WifiCredentials {
    uint32_t version = 1;
    char ssid[33] = {};
    char password[65] = {};
};
bool validPassword(const String& password) {
    if (password.isEmpty()) return true; // Open network.
    if (password.length() >= 8 && password.length() <= 63) return true;
    if (password.length() != 64) return false;
    for (size_t i = 0; i < password.length(); ++i) {
        const char c = password[i];
        if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))) return false;
    }
    return true;
}
}

void RadioWifi::begin() {
    Preferences prefs;
    if (prefs.begin("radioWifi", false)) {
        WifiCredentials credentials;
        if (prefs.isKey("network") && prefs.getBytesLength("network") == sizeof(credentials) &&
            prefs.getBytes("network", &credentials, sizeof(credentials)) == sizeof(credentials) &&
            credentials.version == 1 && memchr(credentials.ssid, 0, sizeof(credentials.ssid)) &&
            memchr(credentials.password, 0, sizeof(credentials.password))) {
            savedSsid_ = credentials.ssid;
            savedPassword_ = credentials.password;
            if (!validPassword(savedPassword_)) { savedSsid_ = ""; savedPassword_ = ""; }
        }
        prefs.end();
    }
    // NVS above is the single source of truth; avoid driver auto-saving a failed trial.
    WiFi.onEvent([](arduino_event_id_t event, arduino_event_info_t info) {
        // Runs on Arduino's event task: numeric diagnostics only, no UI or secrets.
        if (event == ARDUINO_EVENT_WIFI_SCAN_DONE) {
            Serial.printf("[WIFI] SCAN_DONE t=%lu durum=%lu sonuc=%u id=%u\n",
                (unsigned long)millis(), (unsigned long)info.wifi_scan_done.status,
                (unsigned)info.wifi_scan_done.number, (unsigned)info.wifi_scan_done.scan_id);
        } else if (event == ARDUINO_EVENT_WIFI_STA_START || event == ARDUINO_EVENT_WIFI_STA_STOP) {
            Serial.printf("[WIFI] STA olayi t=%lu kod=%d\n", (unsigned long)millis(), (int)event);
        } else if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {
            Serial.printf("[WIFI] Baglanti kesildi t=%lu neden=%u\n",
                (unsigned long)millis(), (unsigned)info.wifi_sta_disconnected.reason);
        }
    });
    WiFi.persistent(false);
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false);
    WiFi.setAutoReconnect(false);
    WiFi.disconnect(false, false);
    if (savedSsid_.length()) startSaved();
    else { message_ = "İlk kullanım: ağınızı seçip şifrenizi girin."; setupRequested_ = true; }
}

void RadioWifi::startSaved(Phase phase) {
    if (savedSsid_.isEmpty()) return;
    stopScan();
    WiFi.disconnect(false, false);
    phase_ = phase;
    begun_ = false;
    startedAt_ = millis();
    if (phase == Phase::Saved) message_ = "Kayıtlı ağa bağlanılıyor...";
}

void RadioWifi::stopScan() {
    if (scanning_) esp_wifi_scan_stop();
    scanning_ = false;
    scanPending_ = false;
    WiFi.scanDelete();
}

void RadioWifi::openSetup() {
    setupOpen_ = true;
    // Allow the user to correct a saved network without waiting for its timeout.
    if (phase_ == Phase::Saved) {
        WiFi.disconnect(false, false); phase_ = Phase::Idle;
    }
    if (phase_ == Phase::Idle) message_ = "Ağ seçin veya ağ adını elle girin. Şifreyi yeniden yazın.";
}

void RadioWifi::closeSetup() {
    stopScan();
    setupOpen_ = false;
    if (phase_ == Phase::Candidate) {
        candidateSsid_ = ""; candidatePassword_ = "";
        WiFi.disconnect(false, false); phase_ = Phase::Idle;
    }
    if (phase_ == Phase::Idle && WiFi.status() != WL_CONNECTED && savedSsid_.length()) startSaved();
}

void RadioWifi::scan() {
    if (busy()) { message_ = "Önce bağlantı denemesinin tamamlanmasını bekleyin."; return; }
    stopScan(); names_.clear(); scanReady_ = false;
    // Queue one asynchronous scan; retries are explicitly requested with Tara.
    scanPending_ = true; scanAt_ = millis();
    message_ = "Ağlar taranıyor...";
}

void RadioWifi::failScan(int code) {
    uint16_t driverCount = 0;
    const esp_err_t driverResult = esp_wifi_scan_get_ap_num(&driverCount);
    Serial.printf("[WIFI] Tarama hatasi: %d sure=%lu ms durum=%d driver_kod=%d driver_ag=%u heap=%u\n",
        code, (unsigned long)(millis() - scanStartedAt_), (int)WiFi.status(),
        (int)driverResult, (unsigned)driverCount, (unsigned)ESP.getFreeHeap());
    stopScan(); names_.clear();
    scanReady_ = true;
    message_ = "Tarama tamamlanamadı. Tara'ya dokunun veya ağ adını yazın.";
}

bool RadioWifi::connect(const String& ssid, const String& password) {
    if (busy()) { message_ = "Bir bağlantı denemesi sürüyor."; return false; }
    if (ssid.isEmpty() || ssid.length() > 32 || strchr(ssid.c_str(), '\n') || strchr(ssid.c_str(), '\r')) {
        message_ = "Ağ adı boş olamaz ve en fazla 32 bayt olabilir."; return false;
    }
    if (!validPassword(password)) {
        message_ = "Şifre 8–63 karakter veya 64 onaltılık hane olmalı. Açık ağda boş bırakın."; return false;
    }
    stopScan();
    candidateSsid_ = ssid; candidatePassword_ = password;
    phase_ = Phase::Candidate; startedAt_ = millis();
    begun_ = false;
    message_ = "Bağlanılıyor... Başarılı olursa kaydedilecek.";
    WiFi.disconnect(false, false);
    return true;
}

bool RadioWifi::saveCandidate() {
    WifiCredentials credentials;
    candidateSsid_.toCharArray(credentials.ssid, sizeof(credentials.ssid));
    candidatePassword_.toCharArray(credentials.password, sizeof(credentials.password));
    Preferences prefs;
    if (!prefs.begin("radioWifi", false)) return false;
    const bool saved = prefs.putBytes("network", &credentials, sizeof(credentials)) == sizeof(credentials);
    prefs.end();
    memset(&credentials, 0, sizeof(credentials));
    return saved;
}

void RadioWifi::failCandidate(const char* reason) {
    message_ = reason;
    candidateSsid_ = ""; candidatePassword_ = "";
    WiFi.disconnect(false, false); phase_ = Phase::Idle;
    if (savedSsid_.length()) {
        message_ += " Önceki kayıt korundu; eski ağa dönülüyor.";
        startSaved(Phase::Rollback);
    }
}

void RadioWifi::loop() {
    if (scanPending_ && static_cast<int32_t>(millis() - scanAt_) >= 0) {
        scanPending_ = false;
        scanStartedAt_ = millis();
        // Arduino default: 300 ms per channel, 6 s scan timeout. No shortened RF dwell.
        const int result = WiFi.scanNetworks(true);
        Serial.printf("[WIFI] Tarama baslatma: %d t=%lu mod=%d durum=%d heap=%u\n",
            result, (unsigned long)scanStartedAt_, (int)WiFi.getMode(), (int)WiFi.status(),
            (unsigned)ESP.getFreeHeap());
        if (result == WIFI_SCAN_RUNNING || result >= 0) scanning_ = true;
        else failScan(result);
    }
    if (scanning_) {
        const int count = WiFi.scanComplete();
        if (count < 0 && count != WIFI_SCAN_RUNNING) failScan(count);
        else if (count >= 0) {
            Serial.printf("[WIFI] Tarama tamamlandi: %d ag sure=%lu ms\n", count,
                (unsigned long)(millis() - scanStartedAt_));
            scanning_ = false; names_.clear();
            for (int i = 0; i < count && names_.size() < 32; ++i) {
                const String name = WiFi.SSID(i);
                if (name.isEmpty() || name.indexOf('\n') >= 0 || name.indexOf('\r') >= 0) continue;
                bool duplicate = false;
                for (const auto& previous : names_) if (previous == name) duplicate = true;
                if (!duplicate) names_.push_back(name);
            }
            WiFi.scanDelete(); scanReady_ = true;
            message_ = names_.empty() ? "Ağ bulunamadı. Yeniden tarayın veya ağ adını yazın." :
                "Ağ seçin, şifreyi girin; Bağlan ve kaydet'e dokunun.";
        }
    }
    if (phase_ != Phase::Idle) {
        const String& target = phase_ == Phase::Candidate ? candidateSsid_ : savedSsid_;
        if (!begun_) {
            // Wait for the old link to go down before testing the new credentials.
            // Otherwise stale WL_CONNECTED + a newly set SSID could look like success.
            if (WiFi.status() == WL_CONNECTED) {
                if (millis() - startedAt_ < 3000) return;
                if (phase_ == Phase::Candidate) failCandidate("Eski bağlantı kapatılamadı; tekrar deneyin.");
                else {
                    phase_ = Phase::Idle; retryAt_ = millis() + 10000;
                    message_ = "Ağ bağlantısı yenilenemedi. Wi-Fi ayarlarını kontrol edin.";
                    if (!setupOpen_) setupRequested_ = true;
                }
                return;
            }
            const String& password = phase_ == Phase::Candidate ? candidatePassword_ : savedPassword_;
            WiFi.begin(target.c_str(), password.isEmpty() ? nullptr : password.c_str());
            begun_ = true; startedAt_ = millis();
            return;
        }
        if (WiFi.status() == WL_CONNECTED && WiFi.SSID() == target) {
            if (phase_ == Phase::Candidate) {
                if (!saveCandidate()) { failCandidate("Bağlantı kuruldu ama bilgiler kaydedilemedi."); return; }
                savedSsid_ = candidateSsid_; savedPassword_ = candidatePassword_;
                candidateSsid_ = ""; candidatePassword_ = "";
                message_ = "Bağlandı ve kaydedildi. Kapat ile radyoya dönebilirsiniz.";
            } else if (phase_ == Phase::Rollback) message_ += " Eski ağa bağlandı.";
            else message_ = "Kayıtlı ağa bağlandı.";
            phase_ = Phase::Idle;
        } else if (millis() - startedAt_ >= 30000) {
            if (phase_ == Phase::Candidate) failCandidate("Bağlanılamadı. Ağ adını ve şifreyi kontrol edin.");
            else {
                const bool rollback = phase_ == Phase::Rollback;
                WiFi.disconnect(false, false); phase_ = Phase::Idle; retryAt_ = millis() + 10000;
                message_ = rollback ? "Önceki ağ da erişilebilir değil; kayıt korundu. Yeni bilgiler deneyebilirsiniz." :
                    "Kayıtlı ağa bağlanılamadı. Ağı ve şifreyi kontrol edin.";
                if (!setupOpen_) setupRequested_ = true;
            }
        }
    } else if (!setupOpen_ && savedSsid_.length() && WiFi.status() != WL_CONNECTED &&
               static_cast<int32_t>(millis() - retryAt_) >= 0) startSaved();
}

bool RadioWifi::takeSetupRequest() {
    const bool requested = setupRequested_; setupRequested_ = false; return requested;
}
bool RadioWifi::connected() const {
    return phase_ == Phase::Idle && savedSsid_.length() && WiFi.status() == WL_CONNECTED && WiFi.SSID() == savedSsid_;
}
bool RadioWifi::takeScanResults(std::vector<String>& names) {
    if (!scanReady_) return false;
    names = names_; scanReady_ = false; return true;
}
