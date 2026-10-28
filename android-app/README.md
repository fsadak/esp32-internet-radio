# Radyo Kumandası — Android kaynak projesi

Java ile hazırlanmış, ek arayüz kütüphanesi kullanmayan Android uygulaması. Paket: tr.fsadak.radyokumanda, sürüm 0.2.0. minSdk 26, compileSdk/targetSdk 36, Android Gradle Plugin 8.10.0, Gradle 8.13, JDK 17.

## Kullanım ve keşif

Telefonunuz ve radyolarınız aynı yerel ağdayken **Radyoları bul** düğmesine basın ve cihazınızı seçin. İlk açılışta arama penceresi kendiliğinden açılır. Arama 15 saniye sürer; cihazlar bulundukça listelenir. **Tekrar ara** ile yenileyin. Bir radyoya basılı tutarak telefonunuzda kullanılacak adı kaydedin. Ad değişikliği firmware'e yazılmaz.

Son cihazın kimliği ve IP adresi saklanır. Uygulama tekrar öne geldiğinde son IP denenir ve aynı cihaz kimliği yeniden aranır. Yeni IP bulunursa bağlantı güncellenir. Aynı IP başka bir cihaza verilmişse kimlik uyuşmazlığı komut gönderimini engeller. **IP ile bağlan** seçeneğini açarak elle bağlantı da kurabilirsiniz.

Otomatik keşif için radyoya firmware **0.11.0** yükleyin. Önceki sürümde elle IP bağlantısı çalışır. Servis türü `_esp32-radio._tcp.`, port 80, TXT `app=esp32-internet-radio` ve `id=<12 haneli MAC kimliği>` şeklindedir. Uygulama yalnızca uygun TXT/kimlik ve yerel IPv4 adreslerini kabul eder. mDNS engellenen veya istemci yalıtımı bulunan ağlarda arama sonuç vermeyebilir.

## Derleme
Android Studio'da bu klasörü açın ve SDK 36'yı seçin. local.properties dosyasını kendi SDK konumunuzla oluşturun; bu dosya pakete dahil değildir.

Test imzasını yerel olarak üretin (proje kökünde private klasörünü oluşturun):

```text
keytool -genkeypair -keystore private/radio-test.jks -storepass android -keypass android -alias radiotest -keyalg RSA -keysize 2048 -validity 10000 -dname "CN=Radyo Kumandasi Local Test"
```

Ardından:

```text
gradlew.bat :app:assembleDebug :app:lintDebug
```

APK, app/build/outputs/apk/debug/app-debug.apk konumunda oluşur. Bu bir yerel test imzasıdır; mağaza yayını için imza yönetimi ayrıca ele alınmalıdır. Sağlanan APK'nın özel imza anahtarı kaynak ZIP'e konulmadı. Buradaki komutla üreteceğiniz yeni anahtar farklı olacağından kendi derlemenizi önceki APK üzerine güncellemek yerine kaldırıp yeniden kurmanız gerekebilir; kayıtlı IP de silinebilir.

## Kaynaklar
- MainActivity.java: Yerel Android arayüzü, izinler, yaşam döngüsü ve konuşma hizmeti.
- RadioApi.java: Seri HTTP istekleri, token/liste kimliği ve hata işleme.
- VoiceRules.java: Türkçe komutlar; belirsiz eşleşmeler işlem yapmaz.
- RadioAddress.java: Yalnızca yerel IPv4 adreslerini kabul eder.
- RadioDiscovery.java: Android NSD ile 15 saniyelik mDNS/DNS-SD araması ve sıralı çözümleme.
- RadioDevices.java: Cihaz kimliğiyle tekilleştirilmiş, en fazla 32 cihazlık liste.

Firmware 0.11.0'ın GET /api/state ve POST /api/command uçları kullanılır. POST istekleri Content-Length, X-Radio-Token ve X-Radio-Revision başlıklarını taşır. HTTP 403'te durum/token bir kez yenilenir. Liste kimliği değişmişse eski sıra numarasıyla istasyon seçilmez. Belirsiz ağ hatasında POST kendiliğinden tekrarlanmaz; durum okuması bir kez yeniden denenebilir.

## Testler
Java komut/adres testi proje kökünden:

```text
javac -encoding UTF-8 -d tests/classes app/src/main/java/tr/fsadak/radyokumanda/VoiceRules.java app/src/main/java/tr/fsadak/radyokumanda/RadioAddress.java app/src/main/java/tr/fsadak/radyokumanda/RadioDevices.java tests/VoiceRulesTest.java tests/RadioDevicesTest.java
java -cp tests/classes VoiceRulesTest
java -cp tests/classes RadioDevicesTest
```

Komut/adres için 42, cihaz listesi/kimliği için 10 kontrolü kapsar. Android bağlantı testi için tests/mock_radio.py, bilgisayarın yalnızca 127.0.0.1:80 adresinde bir benzetim sunucusu açar. Bu port boş olmalıdır. Test gerçek radyoda çalıştırılmaz. Android Emulator host gateway adresi 10.0.2.2 kullanılır.

```text
python tests/mock_radio.py
gradlew.bat :app:assembleDebug :app:assembleDebugAndroidTest
adb install -r app/build/outputs/apk/debug/app-debug.apk
adb install -r app/build/outputs/apk/androidTest/debug/app-debug-androidTest.apk
adb shell am instrument -w tr.fsadak.radyokumanda.test/tr.fsadak.radyokumanda.RadioSmoke
adb shell am instrument -w -e discovery true tr.fsadak.radyokumanda.test/tr.fsadak.radyokumanda.RadioSmoke
```

Kaynak ZIP, test raporu/özel anahtar/derleme önbelleği içermez. Gradle ve SDK indirmeleri ilk derlemede internet gerektirebilir. Telefonun konuşma hizmetinin Türkçe performansı fiziksel telefonda ayrıca değerlendirilmelidir.

API testi 21 kontrol içerir; yanlış cihaz kimliği, token yenileme, eski liste kimliği, komutlar ve uygulama açılışını denetler. Keşif testi Android NSD üzerinden iki deneme servisi kaydeder, kimlikleriyle çözümlendiklerini ve gerçek seçim penceresinde listelendiklerini denetler. Bu test, ESP32 donanımı veya ev yönlendiriciniz üzerinden yapılmış bir doğrulama değildir. Testi temiz uygulama verileriyle çalıştırın.

0.2.0 APK, önceki 0.1.0 APK ile aynı yerel anahtarla imzalanmıştır. Mevcut `private/radio-test.jks` dosyanızı koruyun; başka bir anahtar üretirseniz önceki kurulumun üzerine güncelleme yapılamaz.
