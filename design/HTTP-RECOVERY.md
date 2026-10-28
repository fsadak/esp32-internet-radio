# Yayın bağlantısı ve HTTP hataları

ESP32-audioI2S 2.0.7 yerel kopyası boş hesap bilgileri için Authorization başlığı
göndermez. HTTP hata satırları parça adı yerine bağlantı durumunda gösterilir.
TCP/TLS bağlantısı ile yayının hazır olması ayrı izlenir; istemciler yeniden
bağlantıdan önce disconnectStream ile kapatılır.

Geçici hatalar, 408, 429, 5xx ve 403 Account already in use için en fazla üç ek
deneme 3/10/30 saniye sonra yapılır. Genel 401/403 hataları sınırsız denenmez.
20 saniyede hazır olmayan yayın zaman aşımına uğrar. Tekrar bütçesi 30 saniye
kararlı yayın sonrası yenilenir. TCP/TLS bağlantı çağrısı senkrondur.
Denemeler bittiğinde aynı istasyonu yeniden seçebilirsiniz. 0.14.1 erişilemeyen
yayın için silme önerisi de sunar; bu, yayının kalıcı kapandığını kanıtlamaz.
