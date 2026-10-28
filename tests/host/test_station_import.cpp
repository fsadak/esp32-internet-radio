#include "StationCsv.h"
#include "StationStore.h"
#include "RadioSettings.h"
#include "Preferences.h"
#include <iostream>
#include <cstdlib>

int checks = 0;
void check(bool pass, const char* message) {
    ++checks;
    if (!pass) { std::cerr << "FAIL: " << message << '\n'; std::exit(1); }
}
void invalid(const std::string& csv) {
    std::vector<StationRecord> result = {{"Keep", "https://keep.example/live"}};
    std::string error;
    check(!parseStationCsv(csv, result, error), "malformed input rejected");
    check(result.size() == 1 && result[0].name == "Keep", "failed parse leaves current list intact");
    check(!error.empty(), "validation error supplied");
}
int main() {
    std::vector<StationRecord> result; std::string error;
    check(parseStationCsv("name;url\nGroove Salad;https://ice5.somafm.com/groovesalad-128-mp3", result, error), "LF and no final newline");
    check(result.size() == 1 && result[0].name == "Groove Salad", "basic values");
    check(parseStationCsv("\xef\xbb\xbf\r\nname;url\r\n\"T\xc3\xbc rk; \"\"Radyo\"\"\";https://example.org/live?a=1;b=2\r\n", result, error) == false, "unquoted extra separator rejected");
    check(parseStationCsv("\xef\xbb\xbf\r\nname;url\r\n\"T\xc3\xbcrk; \"\"Radyo\"\"\";\"https://example.org/live?a=1;b=2\"\r\n\r\n", result, error), "BOM CRLF Turkish quoted separator and quote");
    check(result[0].name == "T\xc3\xbcrk; \"Radyo\"", "UTF-8 and escaped quote retained");
    auto original = result;
    check(parseStationCsv(writeStationCsv(original), result, error) && result[0].name == original[0].name && result[0].url == original[0].url, "web export canonical round trip");
    check(parseStationCsv("name;url\r A ; http://example.org:8000/live \r\t\r", result, error), "CR only whitespace and HTTP");
    invalid(""); invalid("name;url\n"); invalid("name,url\nA,http://a");
    invalid("name;url\n;http://example.org"); invalid("name;url\nA;");
    invalid("name;url\nA;ftp://example.org"); invalid("name;url\nA;https://");
    invalid("name;url\nA;https:///live"); invalid("name;url\nA;https://:80/live");
    invalid("name;url\nA;https://host/a b"); invalid("name;url\nA;https://user:pass@host/live");
    invalid("name;url\nA;https://host/live\nB;https://host/live");
    invalid("name;url\n\"A;https://host/live"); invalid("name;url\n\"A\"x;https://host/live");
    invalid("name;url\nA;https://host/live;extra");
    invalid("name;url\nA\xc0\xaf;https://host/live");
    invalid("name;url\nA\xed\xa0\x80;https://host/live");
    invalid("name;url\nA\xf4\x90\x80\x80;https://host/live");
    invalid(std::string("name;url\nA") + '\0' + "B;https://host/live");
    invalid("name;url\n" + std::string(97, 'a') + ";https://host/live");
    invalid("name;url\nA;https://host/" + std::string(513, 'a'));
    invalid(std::string(MAX_STATION_CSV_BYTES + 1, ' '));
    std::string many = "name;url\n";
    for (size_t i = 0; i < MAX_STATIONS; ++i) many += "Station " + std::to_string(i) + ";http://host/" + std::to_string(i) + "\n";
    check(parseStationCsv(many, result, error) && result.size() == MAX_STATIONS, "maximum list accepted");
    invalid(many + "Extra;http://host/extra\n");

    Preferences::storage.clear();
    StationStore store;
    const uint32_t defaultIdentity = store.identity();
    check(!store.load() && store.count() == 1, "first boot built-in station");
    String saveError;
    check(store.replace(result, saveError) && store.count() == MAX_STATIONS, "replace entire list");
    check(std::string(store[MAX_STATIONS - 1].name) == "Station " + std::to_string(MAX_STATIONS - 1), "view lifetime after swap");
    StationStore reboot;
    check(reboot.load() && reboot.count() == MAX_STATIONS && reboot.identity() == store.identity(), "catalog survives restart without SD");
    auto oldIdentity = store.identity();
    Preferences::failAfter = 0;
    check(!store.replace(original, saveError) && store.count() == MAX_STATIONS && store.identity() == oldIdentity, "NVS write failure preserves active catalog");
    Preferences::failAfter = -1;
    StationStore rebootAfterFailure;
    check(rebootAfterFailure.load() && rebootAfterFailure.count() == MAX_STATIONS, "NVS failure preserves stored catalog");
    check(!store.replace({}, saveError) && store.count() == MAX_STATIONS, "empty replacement rejected");
    setStationSettingsIdentity(store.identity());
    RadioSettings settings; settings.bootStation = MAX_STATIONS - 1; settings.brightness = 99;
    check(saveRadioSettings(settings) && saveLastPlayback(3, MAX_STATIONS - 1), "settings and playback saved");
    RadioSettings loaded;
    loadRadioSettings(loaded, MAX_STATIONS);
    size_t station = 0; uint8_t volume = 11;
    loadLastPlayback(volume, station, MAX_STATIONS);
    check(loaded.bootStation == MAX_STATIONS - 1 && station == MAX_STATIONS - 1 && volume == 3, "same catalog restores indices and volume");
    setStationSettingsIdentity(defaultIdentity);
    loadRadioSettings(loaded, MAX_STATIONS); station = 0;
    loadLastPlayback(volume, station, MAX_STATIONS);
    check(loaded.bootStation == 0 && station == 0 && loaded.brightness == 99 && volume == 3, "changed catalog resets only station selection");
    Preferences::storage["stations/csv1"] = {'b','a','d'};
    StationStore corrupt;
    check(!corrupt.load() && corrupt.count() == 1, "corrupt saved CSV falls back to built-in station");
    std::cout << "PASS: " << checks << " checks (firmware CSV parser, catalog persistence and settings identity)\n";
}
