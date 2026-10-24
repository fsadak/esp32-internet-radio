#include "SdDiagnostics.h"
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

bool runSdDiagnostics() {
    Serial.println("\n[SD] Kart kontrolu basliyor...");
    pinMode(TF_CS, OUTPUT);
    digitalWrite(TF_CS, HIGH);
    SPI.begin(TF_SPI_SCLK, TF_SPI_MISO, TF_SPI_MOSI, TF_CS);
    // Same 1 MHz SPI speed as the supplied working hardware demo.
    // Explicitly disable formatting; this diagnostic never creates/writes files.
    if (!SD.begin(TF_CS, SPI, 1000000, "/sd", 5, false)) {
        Serial.println("[SD] Kart yok veya okunamadi. Radyo devam edecek.");
        SD.end();
        SPI.end();
        return false;
    }
    const uint8_t type = SD.cardType();
    if (type == CARD_NONE) {
        Serial.println("[SD] Kart algilanamadi. Radyo devam edecek.");
        SD.end();
        SPI.end();
        return false;
    }
    const char* typeName = type == CARD_MMC ? "MMC" :
                           type == CARD_SD ? "SD" :
                           type == CARD_SDHC ? "SDHC" : "Bilinmeyen";
    Serial.printf("[SD] Tur: %s, kart kapasitesi: %llu MB\n", typeName,
                  static_cast<unsigned long long>(SD.cardSize() / (1024 * 1024)));
    File root = SD.open("/", FILE_READ);
    if (!root || !root.isDirectory()) {
        Serial.println("[SD] Kok dizin acilamadi. Radyo devam edecek.");
        root.close();
        SD.end();
        SPI.end();
        return false;
    }
    Serial.println("[SD] Kok dizin (yalnizca ad ve boyut):");
    constexpr size_t MAX_ENTRIES = 40;
    size_t count = 0;
    while (count < MAX_ENTRIES) {
        File entry = root.openNextFile(FILE_READ);
        if (!entry) break;
        if (entry.isDirectory()) {
            Serial.printf("[SD] DIZIN  %s\n", entry.name());
        } else {
            Serial.printf("[SD] DOSYA  %s (%lu bayt)\n", entry.name(),
                          static_cast<unsigned long>(entry.size()));
        }
        entry.close();
        ++count;
    }
    root.close();
    if (count == MAX_ENTRIES) Serial.println("[SD] Liste 40 kayitla sinirlandi.");
    Serial.printf("[SD] Kontrol tamamlandi; %u kayit listelendi.\n",
                  static_cast<unsigned int>(count));
    return true;
}
