#include "RadioDiscovery.h"
#include "RadioSettings.h"
#include <WiFi.h>
#include <ESPmDNS.h>

void RadioDiscovery::identify() {
    if (id_.length()) return;
    id_ = WiFi.macAddress();
    id_.replace(":", "");
    id_.toLowerCase();
    const String suffix = id_.substring(6);
    hostname_ = "esp32-radio-" + id_;
    name_ = "ESP32 Radyo " + suffix;
}
const String& RadioDiscovery::id() { identify(); return id_; }
const String& RadioDiscovery::name() { identify(); return name_; }

void RadioDiscovery::loop(bool connected) {
    if (!connected) {
        if (running_) MDNS.end();
        running_ = false;
        retryAt_ = 0;
        return;
    }
    const IPAddress current = WiFi.localIP();
    if (running_ && current != address_) { MDNS.end(); running_ = false; retryAt_ = 0; }
    if (running_ || (retryAt_ && static_cast<int32_t>(millis() - retryAt_) < 0)) return;
    identify();
    retryAt_ = millis() + 30000;
    if (!MDNS.begin(hostname_.c_str())) {
        Serial.println("[KESIF] mDNS baslatilamadi; radyo calismaya devam ediyor.");
        return;
    }
    MDNS.setInstanceName(name_);
    if (!MDNS.addService("esp32-radio", "tcp", 80)) {
        MDNS.end();
        Serial.println("[KESIF] Servis kaydi basarisiz; 30 sn sonra yeniden denenecek.");
        return;
    }
    MDNS.addServiceTxt("esp32-radio", "tcp", "app", "esp32-internet-radio");
    MDNS.addServiceTxt("esp32-radio", "tcp", "id", id_.c_str());
    MDNS.addServiceTxt("esp32-radio", "tcp", "model", "Sunton ESP32-8048S070");
    MDNS.addServiceTxt("esp32-radio", "tcp", "fw", RADIO_VERSION);
    MDNS.addServiceTxt("esp32-radio", "tcp", "api", "1");
    address_ = current;
    running_ = true;
    Serial.printf("[KESIF] %s / %s.local\n", name_.c_str(), hostname_.c_str());
}
