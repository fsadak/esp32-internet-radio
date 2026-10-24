#include "SdStationImport.h"
#include "SdMusicFiles.h"
#include <SPI.h>
#include <SD.h>
#include <algorithm>

bool listSdStationFiles(std::vector<String>& files, String& error, SdService service) {
    files.clear();
    // Reopen the card on each request so insertion after boot is supported.
    if (service) service();
    if (!mountRadioSd(error)) return false;
    File root = SD.open("/", FILE_READ);
    if (!root || !root.isDirectory()) { error = "SD kartın kök dizini açılamadı."; return false; }
    size_t entries = 0;
    for (;;) {
        if (service) service();
        File entry = root.openNextFile(FILE_READ);
        if (!entry) break;
        if (++entries > 512) { error = "Kök dizinde çok fazla kayıt var (en fazla 512)."; files.clear(); return false; }
        String name = entry.name();
        if (name.startsWith("/")) name.remove(0, 1);
        String lower = name; lower.toLowerCase();
        if (!entry.isDirectory() && lower.endsWith(".csv")) {
            if (files.size() == 64) { error = "En fazla 64 CSV dosyası listelenebilir."; files.clear(); return false; }
            files.push_back("/" + name);
        }
        entry.close();
    }
    root.close();
    std::sort(files.begin(), files.end(), [](const String& a, const String& b) { return a.compareTo(b) < 0; });
    if (files.empty()) { error = "Kök dizinde .csv dosyası bulunamadı."; return false; }
    return true;
}

bool readSdStationFile(const String& path, std::vector<StationRecord>& result,
                       String& error, SdService service) {
    if (!path.startsWith("/") || path.indexOf('/', 1) >= 0 || path.indexOf("..") >= 0) {
        error = "Geçersiz dosya yolu."; return false;
    }
    File file = SD.open(path, FILE_READ);
    if (!file || file.isDirectory()) { error = "Dosya açılamadı. SD kartı kontrol edin."; return false; }
    const size_t expected = file.size();
    if (expected > MAX_STATION_CSV_BYTES) { error = "Dosya 12 KB sınırını aşıyor."; return false; }
    std::string csv; csv.reserve(expected);
    while (csv.size() < expected) {
        if (service) service();
        uint8_t buffer[256];
        const size_t wanted = std::min(sizeof(buffer), expected - csv.size());
        const int count = file.read(buffer, wanted);
        if (count <= 0) { error = "Dosya tam okunamadı. Mevcut liste korundu."; return false; }
        csv.append(reinterpret_cast<const char*>(buffer), count);
    }
    file.close();
    std::string detail;
    if (!parseStationCsv(csv, result, detail)) { error = detail.c_str(); return false; }
    return true;
}
