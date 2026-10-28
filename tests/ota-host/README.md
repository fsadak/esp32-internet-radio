# OTA kontrolleri

Bu testler gerçek `RadioOta.cpp`, `RadioWeb.cpp` ve `RadioFirmwareImage.h` kaynaklarını sahte Wi-Fi/flash/OTA işlevleriyle bilgisayarda çalıştırır. Gerçek flash, ESP32 bootloader'ı, SHA-256 doğrulama uygulaması, radyo yayını veya fiziksel ağ test edilmez. SDK'nın doğrulama hatası vermesi durumunda yeni alanın etkinleştirilmediği denetlenir.

C++17 derleyicisiyle proje kökünden çalıştırın:

```sh
g++ -std=c++17 -Wall -Wextra -I tests/ota-host -I include tests/ota-host/test_ota.cpp src/RadioOta.cpp src/RadioWeb.cpp -o /tmp/radio-ota-check
/tmp/radio-ota-check
node tests/test_ota_page.cjs
```

Başlık ve proje işaretinin farklı paket sınırlarında alınması, yanlış çip/dosya, boyut sınırı, eksik aktarım, yazma/doğrulama hataları, Host/Origin/token/onay/Content-Type kontrolleri, yinelenen Content-Length, chunked aktarım reddi, ayar kilidi, bağlantı kesilmesi, zaman aşımı, en fazla 4096 baytlık yazmalar, normal API, gecikmeli yeniden başlama ve açılış onayı kontrol edilir. Tarayıcı betiği için başarılı yükleme, hatalı başlık, büyük dosya, kullanıcı vazgeçmesi, sunucu reddi ve bağlantı hatası senaryoları çalıştırılır.

Gerçek kartta ilk kez USB ile 0.12.0 yükleyin. Ardından aynı sürümün `firmware.bin` dosyasını webden yükleyerek ekranın açıldığını, ekran/I2S başlatmasından sonra ana döngü en az 10 saniye çalışınca OTA onaylandığını, Wi-Fi/istasyon ayarlarının korunduğunu ve tekrar güncelleme yapılabildiğini doğrulayın. Geliştirme sırasında bu fiziksel test yapılmamıştır.

Geç başlatma sonrasında tam 10 saniyelik onay aralığı için kontrol eklendi; güncellenen test kaynağı henüz çalıştırılmadı.
