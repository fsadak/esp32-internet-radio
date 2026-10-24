# İstasyon listesi örnekleri

- `groove-salad.csv`: tek istasyon, asıl Groove Salad adı.
- `sd-test.csv`: tek istasyon, yüklemeyi ayırt etmek için Groove Salad (SD Test) adı.
- `test-listesi.csv`: Groove Salad, Drone Zone ve Secret Agent; üç istasyonla
  liste seçimi, önceki/sonraki, listenin sonunda başa dönme ve açılış seçimi testleri için.

Dosyaları SD kartın kök dizinine kopyalayın. Yükleme Ayarlar → İstasyonlar menüsünde
dosya seçimi ve ayrı onay gerektirir; bu klasörde dosya hazırlanması cihazdaki listeyi değiştirmez.

Üçlü listedeki adresler 20 Ekim 2026 tarihinde SomaFM'nin resmi MP3 128 kbps
bağlantı sayfalarından doğrulandı:

- [Groove Salad](https://somafm.com/groovesalad/directstreamlinks.html)
- [Drone Zone](https://somafm.com/dronezone/directstreamlinks.html)
- [Secret Agent](https://somafm.com/secretagent/directstreamlinks.html)

Drone Zone ve Secret Agent için bilgisayardan kısa süreli bağlantı kontrolünde HTTP 200,
audio/mpeg ve ses verisi alındı; bağlantılar hemen kapatıldı. Groove Salad zaten cihazda
çalıştığı doğrulanan adrestir. Bu kontroller yeni iki yayının ESP32'de çaldığını göstermez;
cihaz testi ayrıca yapılacak. Sonuç kaydı: `display-test/multi-station-stream-check.json`.

Dosya biçimi ve sınırlar: `../STATION-LISTS.md`.
