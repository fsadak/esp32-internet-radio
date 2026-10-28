#pragma once
#include <Arduino.h>
#include <IPAddress.h>

// DNS-SD advertisement only; audio and HTTP keep working if mDNS cannot start.
class RadioDiscovery {
public:
    void loop(bool connected);
    const String& id();
    const String& name();
private:
    void identify();
    String id_, name_, hostname_;
    bool running_ = false;
    IPAddress address_;
    uint32_t retryAt_ = 0;
};
