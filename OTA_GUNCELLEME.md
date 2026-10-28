# Web üzerinden firmware güncelleme — 0.12.0

## İlk kurulum

OTA özelliğini getiren **0.12.0** sürümünü bir kez USB üzerinden yükleyin. Proje kökünde `platformio.ini` dosyasının bulunduğu klasörü açın. PlatformIO'da **Upload** işlemini kullanın. Komut satırında aynı işlem:

```text
pio run -e esp32_radio -t upload
```

Bölüm düzeni değiştirilmemiştir: `default_16MB.csv`, iki adet 0x640000 baytlık uygulama alanı ve OTA verisi içerir. Ayar/NVS alanı silinmez. Normal yükleme işlemini kullanın; tüm flash belleğini silmeyin.

## Sonraki güncellemeler

1. Yeni sürümü derleyin: `pio run -e esp32_radio`.
2. Tarayıcıda radyonuzun mevcut IP adresini açın. Eski sayfa açık kaldıysa yenileyin.
3. Sayfanın altındaki **Firmware güncelleme** bölümünden `.pio\build\esp32_radio\firmware.bin` dosyasını seçin.
4. **Güncellemeyi yükle** düğmesine basın ve dosya onayını verin. Cihazdaki ayar menüsü açıksa önce kapatın.
5. İlerleme ve yeniden başlama tamamlanana kadar sayfayı açık, beslemeyi bağlı tutun. Radyo veya SD oynatma durur. Cihazda 3 saniye “Güncelleme başlatıldı” ve gücü kesmeme uyarısı gösterilir; ardından flash yazımı başlamadan arka ışık kapatılır. Aktarım ilerlemesini tarayıcıdan izleyin. Başarılı doğrulama ve açılış bölümü seçimi sonrasında ekran açılıp “Güncelleme tamamlandı. Yeniden başlatılıyor...” mesajı gösterilir. Yaklaşık 2 saniye sonra cihaz yeniden başlar; normal açılış ayarları uygulanır.

Yalnızca bu projenin OTA destekli uygulama `.bin` dosyasını kullanın. `bootloader.bin`, `partitions.bin`, dosya sistemi imajı veya birleştirilmiş flash imajı seçmeyin. 0.11.0 ve daha eski dosyalar OTA proje işareti taşımadığından bu ekrandan yüklenmez. Bölüm tablosu/bootloader değişiklikleri USB yüklemesi gerektirir. APK güncellemesi gerekmez.

## Hata durumları

Yeni dosya kullanılmayan uygulama alanına, en fazla 4096 baytlık parçalarla yazılır. HTTP dosya boyutu OTA alanını aşamaz. Yanlış ESP32-S3 biçimi/proje işareti, eksik aktarım ve SDK bütünlük doğrulaması hatalarında yeni alan etkinleştirilmez. Kesinti sırasında mevcut çalışan firmware alanına yazılmaz. Dosya aktarımı yeniden denenmez; cihazın durumunu kontrol ettikten sonra yeniden deneyin.

Başarısız veya yarım kalan güncelleme sonrasında ekran ayarlı normal parlaklığa döner ve hata durumu gösterilir. Oynatma kendiliğinden başlamaz; **Başlat** düğmesini kullanın. Wi-Fi kopmasıyla yarım kalan aktarım 15 saniyelik hareketsizlik zaman aşımında bırakılır. Toplam aktarım sınırı 180 saniyedir. Doğrulama başarılı olursa cihaz yanıtı gönderdikten yaklaşık 2 saniye sonra yeniden başlatılır. Web sayfası aynı cihazın yeni açılışını 75 saniyeye kadar bekler; bağlantı gelmezse cihaz ekranını/IP adresini kontrol edip sayfayı yeniden açın.

SDK'nın rollback desteği kullanılır. Arduino'nun erken OTA onayı ertelenir; yeni firmware ekran başlatmasını ve I2S pin kurulumunu başarıyla tamamladıktan sonra ana döngü en az 10 saniye çalıştığında onaylanır. Ekran veya I2S pin kurulumu başarısızsa önceki uygulamaya dönüş istenir; onaydan önce yeniden başlama/crash olursa bootloader önceki uygulamaya dönebilir. Bu kontrol internet yayınının veya yeni sürümdeki bütün işlevlerin kusursuz çalıştığını garanti etmez.

Web işlemi mevcut Host/Origin ve oturum token kontrollerini kullanır. Ayrı kullanıcı/parola veya imzalı firmware doğrulaması eklenmemiştir; proje işareti uyumluluk kontrolüdür. Bu arayüz güvenilen yerel ağ içindir.

## Doğrulama durumu

2026-10-28 tarihinde başarılı OTA akışı fiziksel kartta doğrulanmıştır: 3 saniyelik
başlangıç/gücü kesmeme uyarısı, yazım sırasında karanlık ekran ve tarayıcı ilerlemesi,
sonlandırma sonrası tamamlanma mesajı ve normal yeniden başlatma. Bu doğrulama güç
kesintisi, yarım aktarım, hata sonrası ekran geri dönüşü veya rollback senaryolarını
kapsamaz; bunlar ayrıca doğrulanmalıdır.

SDK davranışı için [Espressif ESP-IDF 4.4 OTA belgesini](https://docs.espressif.com/projects/esp-idf/en/v4.4/esp32/api-reference/system/ota.html) inceleyebilirsiniz.

## RGB görüntü sorunu ve önceki deneme

OTA sırasında yatay kayma, titreme ve iz bırakma gözlenmiştir. ESP-IDF 4.4 sürücüsü
VSYNC sonunda LCD/DMA aktarımını zaten yeniden başlatır. Uygulama callback'indeki
ikinci, bağımsız DMA/FIFO sıfırlaması kaldırılmıştır. PCLK, RGB pinleri ve bölüm
tablosu değiştirilmemiştir. Bu değişiklik sonrasında yapılan ardışık OTA denemelerinde
görüntü sorunu devam etmiştir; kayma/titreme yalnızca aktarım sırasında görülmüş,
yeniden başlatmadan sonra ana ekran normal çalışmıştır. Çift sıfırlamanın kaldırılması
bu sorun için başarılı düzeltme değildir.

Yeni OTA ekran davranışını sınamak için güncel sürüm cihazda çalışırken bir sonraki
web yüklemesini değerlendirin. Aktarım sırasında dosyanın içindeki yeni kod değil,
cihazda o anda çalışan firmware kullanılır.

## Güncel ekran davranışı

Başlangıç uyarısı 3 saniye boyunca sabit gösterilir; bu süre ana döngüyü kilitlemez.
OTA bölümünü açma, NVS kayıtları ve flash yazımı bu uyarıdan sonra, arka ışık kapalıyken
başlar. Yazım sırasında cihazda ilerleme çizilmez; ilerleme tarayıcıdadır. Başarılı
sonlandırma ve açılış bölümü seçimi tamamlanınca sabit başarı mesajı gösterilir.
Yeniden başlatma yaklaşık 2 saniye sonra gerçekleşir.

Bu davranış flash/PSRAM kısıtını ortadan kaldırmaz; ekranda flash işlemleri sırasında
çizim yapmaktan kaçınır. Başlangıç reddi, yazma hatası, aktarım kesilmesi veya zaman
aşımında ekran normale dönecek şekilde tasarlanmıştır; bu hata yolları ayrıca
sınanmalıdır. Başlangıç/başarı mesajları dahil başarılı akış fiziksel kartta doğrulanmıştır.
