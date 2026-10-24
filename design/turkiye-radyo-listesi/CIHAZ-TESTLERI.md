# Türkiye listesi — cihaz testleri

Kullanıcı tarafından Sunton ESP32-8048S070 / firmware 0.4.0 üzerinde doğrulandı:

- `01_tr_secme_32.csv`: 32 istasyon önizlemesi ve onayla yükleme başarılı.
- Radyo 7 — 1/32: yayın çalıyor.
- Süper FM — 2/32: Sonraki ile geçiş ve yayın başarılı.
- TRT FM — 4/32: listeden doğrudan seçim ve AAC yayını başarılı.
- Radyo Spor — 32/32: 4. liste sayfasına geçiş, son satırdan seçim ve yayın başarılı.
- Radyo Spor’dan Sonraki ile Radyo 7 — 1/32’ye sarma ve yayın başarılı.

Bu, seçme listedeki diğer 28 yayının veya katalogdaki tüm 222 kanalın cihazda çaldığı anlamına gelmez.
HLS grubunun cihaz testi yapılmadı. 32 istasyonlu listede kart çıkarılıp yeniden açılış testi
henüz yapılmadı; önceki tek istasyonlu listede bu kalıcılık testi geçmişti.

İstasyon CSV dosyaları ve firmware değiştirilmedi. Yalnızca doğrulama kayıtları güncellendi.
