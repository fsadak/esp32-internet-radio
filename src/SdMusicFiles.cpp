#include "SdMusicFiles.h"
#include <SPI.h>
#include <SD.h>
#include <algorithm>

bool mountRadioSd(String& error) {
    SD.end(); SPI.end();
    pinMode(TF_CS, OUTPUT); digitalWrite(TF_CS, HIGH);
    SPI.begin(TF_SPI_SCLK, TF_SPI_MISO, TF_SPI_MOSI, TF_CS);
    if (!SD.begin(TF_CS, SPI, 1000000, "/sd", 5, false) || SD.cardType() == CARD_NONE) {
        SD.end(); SPI.end(); error = "SD kart yok veya okunamadı. Kartı takıp tekrar deneyin.";
        return false;
    }
    return true;
}

bool validSdMusicPath(const String& path) {
    if (!path.startsWith("/") || path.length() > 240 || path.indexOf('\\') >= 0 || path.indexOf("//") >= 0) return false;
    if (path != "/" && path.endsWith("/")) return false;
    for (size_t i = 0; i < path.length(); ++i) if ((uint8_t)path[i] < 32 || path[i] == 127) return false;
    size_t start = 1;
    while (start < path.length()) {
        int end = path.indexOf('/', start);
        if (end < 0) end = path.length();
        const String part = path.substring(start, end);
        if (part == "." || part == "..") return false;
        start = end + 1;
    }
    return true;
}

String sdParentFolder(const String& path) {
    const int slash = path.lastIndexOf('/');
    return slash <= 0 ? String("/") : path.substring(0, slash);
}

bool listSdMusicFolder(const String& folder, std::vector<SdMusicEntry>& entries,
                       String& error, bool remount, SdMusicService service) {
    if (!validSdMusicPath(folder)) { error = "Geçersiz klasör yolu."; return false; }
    if (service) service();
    if (remount && !mountRadioSd(error)) return false;
    std::vector<SdMusicEntry> candidate;
    {
        File directory = SD.open(folder, FILE_READ);
        if (!directory || !directory.isDirectory()) {
            error = "SD dizini açılamadı. Kartı kontrol edin."; return false;
        }
        size_t entries = 0;
        for (;;) {
            if (service) service();
            File entry = directory.openNextFile(FILE_READ);
            if (!entry) break;
            if (++entries > 512) { error = "Bir dizinde en fazla 512 kayıt taranabilir."; return false; }
            String name = entry.name();
            name = name.substring(name.lastIndexOf('/') + 1);
            String lower = name; lower.toLowerCase();
            const bool isDirectory = entry.isDirectory();
            if (name.length() && name != "." && name != ".." && (isDirectory || lower.endsWith(".mp3"))) {
                String path = folder == "/" ? "/" + name : folder + "/" + name;
                if (!validSdMusicPath(path)) { error = "Dosya/klasör yolu geçersiz veya 240 bayttan uzun."; return false; }
                if (candidate.size() == 128) { error = "Bir klasörde en fazla 128 MP3/alt klasör listelenebilir."; return false; }
                candidate.push_back({path, isDirectory});
            }
            entry.close();
        }
    }
    std::sort(candidate.begin(), candidate.end(), [](const SdMusicEntry& a, const SdMusicEntry& b){
        if (a.directory != b.directory) return a.directory;
        return a.path.compareTo(b.path) < 0;
    });
    entries.swap(candidate);
    return true;
}
