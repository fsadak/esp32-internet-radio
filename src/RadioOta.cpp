#include "RadioOta.h"

// Arduino normally confirms OTA before setup(). Defer until the UI/audio initialise.
extern "C" bool verifyRollbackLater() { return true; }

void RadioOta::checkBoot(bool healthy) {
    static bool checked = false, pending = false;
    static bool observing = false;
    static uint32_t healthySince = 0;
    if (!checked) {
        checked = true;
        esp_ota_img_states_t state;
        pending = esp_ota_get_state_partition(esp_ota_get_running_partition(), &state) == ESP_OK &&
            state == ESP_OTA_IMG_PENDING_VERIFY;
    }
    if (!pending) return;
    if (!healthy) {
        observing = false;
        esp_ota_mark_app_invalid_rollback_and_reboot();
        return;
    }
    if (!observing) { healthySince = millis(); observing = true; }
    // Require ten seconds of servicing the main loop after setup completes,
    // rather than allowing slow Wi-Fi/SD setup to consume the entire grace time.
    if (millis() - healthySince >= 10000 && esp_ota_mark_app_valid_cancel_rollback() == ESP_OK)
        pending = false;
}

size_t RadioOta::capacity() const {
    const auto* next = esp_ota_get_next_update_partition(nullptr);
    return next ? next->size : 0;
}
bool RadioOta::fail(const char* message) { error_ = message; abort(); return false; }
void RadioOta::abort() {
    if (active_) esp_ota_abort(handle_);
    active_ = false; handle_ = 0;
}
bool RadioOta::begin(size_t size) {
    if (active_ || pendingRestart_) { error_ = "Başka bir güncelleme sürüyor."; return false; }
    error_.clear(); image_.reset(); total_ = size; received_ = 0;
    partition_ = esp_ota_get_next_update_partition(nullptr);
    if (!partition_ || partition_ == esp_ota_get_running_partition() || size < 288 || size > partition_->size)
        return fail("Firmware boyutu uygun değil veya OTA alanı bulunamadı.");
    if (esp_ota_begin(partition_, OTA_WITH_SEQUENTIAL_WRITES, &handle_) != ESP_OK)
        return fail("Güncelleme alanı açılamadı.");
    active_ = true;
    return true;
}
bool RadioOta::write(const uint8_t* data, size_t size) {
    if (!active_ || size > total_ - received_) return fail("Güncelleme verisi beklenen boyutta değil.");
    if (!image_.feed(data, size)) return fail("Bu dosya uygun ESP32-S3 uygulama firmware'i değil. firmware.bin dosyasını seçin.");
    if (esp_ota_write(handle_, data, size) != ESP_OK) return fail("Firmware yazılamadı. Mevcut sürüm korunuyor.");
    received_ += size;
    return true;
}
bool RadioOta::finish() {
    if (!active_ || received_ != total_ || !image_.compatible())
        return fail("Dosya eksik veya bu radyo projesine ait değil.");
    const esp_err_t verified = esp_ota_end(handle_);
    active_ = false; handle_ = 0; // esp_ota_end frees the handle even on failure.
    if (verified != ESP_OK) return fail("Firmware bütünlük doğrulaması başarısız. Mevcut sürüm korunuyor.");
    if (esp_ota_set_boot_partition(partition_) != ESP_OK)
        return fail("Yeni firmware etkinleştirilemedi. Mevcut sürüm korunuyor.");
    pendingRestart_ = true; finishedAt_ = millis();
    return true;
}
