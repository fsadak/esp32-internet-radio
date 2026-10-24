#pragma once
#include <Arduino.h>
#include <vector>

class RadioWifi {
public:
    void begin();
    void loop();
    void openSetup();
    void closeSetup();
    void scan();
    bool connect(const String& ssid, const String& password);
    bool takeSetupRequest();
    bool takeScanResults(std::vector<String>& names);
    bool setupOpen() const { return setupOpen_; }
    bool busy() const { return phase_ != Phase::Idle; }
    bool scanning() const { return scanning_ || scanPending_; }
    bool connected() const;
    const String& savedSsid() const { return savedSsid_; }
    const String& message() const { return message_; }
private:
    enum class Phase { Idle, Saved, Candidate, Rollback };
    void startSaved(Phase phase = Phase::Saved);
    void stopScan();
    void failScan(int code);
    void failCandidate(const char* reason);
    bool saveCandidate();
    Phase phase_ = Phase::Idle;
    String savedSsid_, savedPassword_, candidateSsid_, candidatePassword_;
    String message_;
    std::vector<String> names_;
    uint32_t startedAt_ = 0, retryAt_ = 0;
    bool setupOpen_ = false, setupRequested_ = false;
    bool scanning_ = false, scanReady_ = false;
    bool begun_ = false;
    bool scanPending_ = false;
    uint32_t scanAt_ = 0;
    uint32_t scanStartedAt_ = 0;
};
