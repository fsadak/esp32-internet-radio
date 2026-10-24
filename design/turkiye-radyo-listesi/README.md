# Türkiye radyo listeleri

**222 farklı kanal:** 190 doğrudan MP3/AAC ve 32 ayrı HLS deneme kanalı. Kontrol: 2026-10-20.

İstasyonların tamamı, bağlantı adresleri ve ayrıntılar: [KATALOG.md](KATALOG.md).

## İlk kullanım

1. `sd-listeleri/01_tr_secme_32.csv` dosyasını SD kartın kök dizinine kopyala.
2. Cihazda Ayarlar → İstasyonlar → SD karttan liste yükle yolundan dosyayı seç.
3. 32 istasyonu kontrol edip onaylarsan mevcut listenin tamamı değiştirilir.

Diğer CSV dosyaları da aynı şekilde seçilebilir. Dosyalar birbirine eklenmez. Cihaz sınırı 32 istasyon / 12 KB’dır; her hazır CSV bu sınırlara ve firmware ayrıştırıcısına göre kontrol edildi. CSV başlığı `name;url`, ayırıcı noktalı virgül, kodlama UTF-8 BOM’dur.

`sd-listeleri.zip`: doğrudan yayınların SD’ye kopyalanacak dosyaları. `hls-deneme/`: HLS listesi ayrı tutulur; önce cihazda denenmelidir. `bilgi-katalog.csv` ve `katalog.json` ayrıntılı arşivdir, cihazın iki sütunlu istasyon dosyası değildir.

Bağlantı kontrolleri bilgisayarda kısa örneklerle yapıldı. ESP32’de bütün yayınlar denenmedi. HLS dosyası özellikle ayrı bir cihaz testi gerektirir. Her CSV yüklemesi mevcut listeyi değiştirir; bu hazırlık cihazda yükleme yapmaz.

| Hazır dosya | İstasyon |
|---|---:|
| sd-listeleri/01_tr_secme_32.csv | 32 |
| sd-listeleri/02_tr_pop_slow_1.csv | 32 |
| sd-listeleri/02_tr_pop_slow_2.csv | 5 |
| sd-listeleri/03_tr_turku_arabesk.csv | 29 |
| sd-listeleri/04_tr_alternatif_caz.csv | 26 |
| sd-listeleri/05_tr_haber_spor.csv | 15 |
| sd-listeleri/06_tr_dini.csv | 17 |
| sd-listeleri/07_tr_yerel_universite_1.csv | 32 |
| sd-listeleri/07_tr_yerel_universite_2.csv | 15 |
| sd-listeleri/08_tr_power.csv | 19 |
| hls-deneme/90_tr_hls_deneme.csv | 32 |

Cihazda doğrulananlar: Radyo 7, Süper FM, TRT FM ve Radyo Spor; ayrıca 32 istasyonlu liste, 4. sayfa ve 32→1 dönüşü. [Cihaz test kaydı](CIHAZ-TESTLERI.md). Diğer yayınlar cihazda henüz doğrulanmadı.
