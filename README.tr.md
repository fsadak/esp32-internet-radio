# ESP32 İnternet Radyosu ve SD MP3 Oynatıcı

ESP32-S3 üzerinde çalışan; siyah-turuncu dokunmatik arayüzü, Türkçe/İngilizce cihaz menüleri, çevrimdışı SD MP3 oynatıcısı ve yerel web kumandası bulunan internet radyosu.

**Proje kurucusu ve sürdürücüsü: fsadak** · **Firmware: 0.14.1** · **PlatformIO / Arduino**

[English](README.md) | **Türkçe**

![Siyah-turuncu radyo arayüzü tasarım önizlemesi](design/tanitim/ana-ekran.png)

*Önceki arayüz tasarımının temsili önizlemesidir; cihaz fotoğrafı veya cihazdan alınmış ekran görüntüsü değildir. Güncel firmware'de ayrıca SD MP3 kontrolleri ve dil seçimi bulunur.*

0.14.1 sürümünde istasyon üst sınırı, favoriler dahil 128'e çıkarılmıştır.
CSV dosyası sınırı 12 KiB olarak korunmuştur; uzun ad ve adreslerle bu sınıra
128 kayıttan önce ulaşılabilir. Mevcut 20 KiB NVS alanı ayarlar, Wi-Fi ve favorilerle
paylaşılır; alan dolarsa kayıt reddedilir ve önceki liste korunur. Bölüm tablosu
ve kayıt biçimi değiştirilmemiştir. Cihaz doğrulaması beklenmektedir.

## 0.14.1 — batarya entegrasyonu öncesi son SD/MP3 sürümü

Bu sürüm SD/MP3 işlevlerini korur. Batarya ölçümü ve şarj durum ekranı uygulanmış
değildir. Radyo/batarya çalışmaları ayrı sürümde sürdürülecek; v0.14.1 sabit bir
etiket olarak kalacaktır. Genel cihaz kullanımı doğrulanmıştır. Bu yayın işlemi
kapsamında yeni derleme, yükleme veya cihaz testi yapılmamıştır.

Bilinen sınırlar: katalog 12 KiB içinde 128 kayıt kabul eder, ancak web istasyon
seçimi iki basamaklı sıfır tabanlı indekslerle (0–99) sınırlıdır. Android 0.2.0
kaynakları en fazla 32 istasyon kabul eder. Korunan sürümde bu davranışlar
değiştirilmemiştir; sonraki indeksler için cihaz ekranını kullanın. Güç kesintisi
ve OTA rollback koşulları ayrı doğrulama gerektirir.

## Özellikler

| Alan | Kullanılabilir işlevler |
| --- | --- |
| İnternet radyosu | Wi-Fi üzerinden yayın, istasyon listesi, önceki/sonraki istasyon, yayıncı sağladığında parça başlığı, tür ve codec bilgisi |
| SD MP3 | Klasör gezintisi, oynat/duraklat, önceki/sonraki parça, −10/+10 saniye sarma, geçen/toplam süre |
| Çalma modları | Tek parça tekrarı, tüm liste tekrarı ve karışık çalma |
| Ses | Ses −/+, iki düğme arasında sayısal seviye, sessize alma, bas/orta/tiz ekolayzer |
| Dil | Türkçe ve İngilizce cihaz arayüzü; varsayılan İngilizce; ayarlardan kalıcı seçim |
| Wi-Fi kurulumu | Ekrandan ağ tarama, elle SSID girişi, maskeli şifre girişi ve ağ kaydı |
| İstasyon yönetimi | Web ve çevrimdışı SD içe aktarma için ortak CSV biçimi |
| Yerel web arayüzü | Radyo kumandası, istasyon düzenleme/sıralama, CSV içe/dışa aktarma ve cihaz durumu |
| Ekran | Parlaklık ayarı, otomatik karartma, arka ışığı kapatma ve dokunarak uyandırma |
| VU metre | Yatay dijital seviye çubuğu ve tepe işareti; ayarlardan kapatma; arka ışık kapalıyken ölçüm ve çizimi durdurma |
| Favoriler | Kalp ile kalıcı seçim; 4 × 2 kutulu, sayfalı ekran; radyo simgesi ve tek satırda en fazla 20 karakterlik ad. Yeni özellik; cihaz doğrulaması bekleniyor. |
| Açılış | Son radyo/MP3 kaynağını ve son seçilen MP3 dosyasını hatırlama |

İstasyon satırlarında onaylı silme düğmesi bulunur. Başarısız yayın bağlantısında yeniden denemeler sonrasında silme önerilebilir; geçici erişim sorunu kalıcı kapanma sayılmaz. Son istasyon korunur. Yeni cihaz akışları için doğrulama beklenmektedir; [istasyon yönetimi](design/STATION-LISTS.md) belgesini inceleyin.

Başlangıç istasyonu **SomaFM Groove Salad**'dır. İstasyon listesini kendi listenizle değiştirebilirsiniz.

## Android kumanda ve otomatik keşif

`android-app/` klasöründeki Radyo Kumandası 0.2.0 uygulamasında **Radyoları bul** düğmesiyle aynı yerel ağdaki bir veya birden fazla radyoyu listeleyip seçin. Listede bir cihaza basılı tutarak telefonunuzda kullanacağınız adı kaydedin. Son seçilen cihaz hatırlanır; uygulama tekrar öne geldiğinde kimliğiyle yeniden aranır. **IP ile bağlan** seçeneği de kullanılabilir.

Firmware 0.11.0, `_esp32-radio._tcp` DNS-SD servisini TCP 80 üzerinde duyurur. TXT kayıtları `app`, `id`, `model`, `fw` ve `api` alanlarını içerir. Cihaz kimliği Wi-Fi MAC adresinden üretilen 12 haneli değerdir; `/api/state` yanıtında `deviceId` ve `deviceName` alanları bulunur. Host denetimi korunur; uygulama keşfedilen yerel IPv4 adresine bağlanır. Wi-Fi yeniden bağlandığında veya IP değiştiğinde servis yeniden kaydedilir.

Keşif için firmware 0.11.0 veya sonrasını radyonuza yükleyin. Önceki firmware, elle IP girilerek kumanda edilebilir. Telefon ve radyo aynı yerel ağda bulunmalı; misafir ağı/istemci yalıtımı veya mDNS engeli keşfi önleyebilir. Yeni firmware derlenmiştir; fiziksel cihazda keşif ve yayın sırasında çalışma ayrıca doğrulanmalıdır. Kurulum ve test ayrıntıları için [Android kaynak açıklamasını](android-app/README.md) inceleyin.

## Web üzerinden firmware güncelleme

Firmware **0.12.0** ile sayfanın altındaki **Firmware güncelleme** alanından uygulama dosyası yükleyebilirsiniz. Bu ilk OTA sürümünü bir kez USB ile yükleyin; sonraki sürümlerde .pio/build/esp32_radio/firmware.bin dosyasını webden seçip onaylayın. Yükleme sırasında oynatma durur; dosya kullanılmayan uygulama alanına yazılıp doğrulanır ve cihaz yeniden başlar. İlerleme ve yeniden bağlantı web sayfasında gösterilir. Bölüm/NVS düzeni korunmuştur; APK güncellemesi gerekmez. Ayrıntılar için [OTA yönergelerini](OTA_GUNCELLEME.md) inceleyin.

OTA başında 3 saniyelik sabit uyarı gösterilir; flash yazımı boyunca arka ışık kapalı tutulur ve ilerleme tarayıcıda izlenir. Başarılı doğrulama ve açılış bölümü seçimi sonrasında tamamlanma mesajı gösterilir; ardından cihaz yeniden başlar. Hata durumunda ekran normal parlaklığa döner. Başlangıç/başarı mesajları dahil başarılı OTA akışı fiziksel kartta doğrulanmıştır; kesinti ve rollback senaryoları ayrıca doğrulanmalıdır.

## Donanım

Proje **Sunton ESP32-8048S070, PCB V1.3** üzerinde test edilmiştir.

| Bileşen | Yapılandırma |
| --- | --- |
| İşlemci | ESP32-S3 |
| Flash / PSRAM | 16 MB flash / 8 MB OPI PSRAM |
| Ekran | 7 inç, 800 × 480 RGB dokunmatik ekran |
| Ses | MAX98357 I²S amplifikatör, mono çıkış |
| Geliştirmede kullanılan hoparlör | 4 Ω / 3 W |
| Besleme | 5 V USB |
| SD kart | Yerel MP3 oynatma ve SD'den istasyon listesi yükleme için gerekli |

Çalıştığı doğrulanan pinler:

| Sinyal | GPIO |
| --- | ---: |
| I²S BCLK | 0 |
| I²S LRC / WS | 18 |
| I²S DOUT | 17 |
| SD CS | 10 |
| SD SCK | 12 |
| SD MOSI | 11 |
| SD MISO | 13 |

Özel kart tanımı çalışan RGB ekran ayarlarını korur. Dokunmatik sorgulama yöntemiyle okunur; GPIO18 sese ayrıldığı için GT911 kesme pini kullanılmaz. Farklı kart revizyonlarında ayarların uyarlanması gerekebilir.

### Gözlenen güç tüketimi

Geliştirme sırasında USB girişinden ölçülerek bildirilen değerler:

| Ekran durumu | Güç | Akım |
| --- | ---: | ---: |
| Tam parlaklık | 1,83 W | 0,37 A |
| Arka ışık kapalı | 0,85 W | 0,17 A |

Bunlar çalışma ölçümleridir; azami tüketim veya uyku akımı değildir. Gerçek tüketim oynatma, ses seviyesi ve ağ etkinliğine göre değişir.

## Derleme ve yükleme

Projedeki yapılandırmayla [PlatformIO](https://platformio.org/) kullanılır. Firmware yüklemek için USB veri bağlantısı gerekir.

| Ayar | Sürüm / değer |
| --- | --- |
| Platform | `espressif32@6.10.0` |
| Framework | Arduino |
| Ortam | `esp32_radio` |
| Kart | `sunton-8048s070-radio` — tanım `boards/` içinde |
| Flash / bölüm tablosu | 16 MB / `default_16MB.csv` |
| Bellek yapılandırması | `qio_opi`, OPI PSRAM |
| LVGL | `9.6.0` |
| ESP32-audioI2S | Yerel düzeltmelerle projeye dahil edilmiş `2.0.7` |
| esp32_smartdisplay | Yerel düzeltmelerle projeye dahil edilmiş `2.1.2` |
| Seri monitör | 115200 baud |

1. Projeyi indirin veya klonlayın; `platformio.ini` dosyasının bulunduğu klasörü açın.
2. PlatformIO'nun sabitlenmiş platform ve bağımlılıkları kurmasını bekleyin.
3. Kartı bağlayın. Otomatik bulunamazsa kartın gerçek seri portunu seçin.
4. PlatformIO üzerinden önce **Build**, ardından **Upload** çalıştırın.
5. Gerekirse seri monitörü açarak tanılama mesajlarını izleyin.

PlatformIO CLI karşılıkları:

```sh
pio run -e esp32_radio
pio run -e esp32_radio -t upload
pio device monitor -b 115200
```

Projedeki `lib/` klasörlerini koruyun: bu firmware'in ihtiyaç duyduğu yerel değişiklikler burada bulunur. Bunları değiştirilmemiş upstream sürümleriyle değiştirmek önceki sorunları geri getirebilir. Web sayfası firmware içinde olduğundan ayrıca web dosya sistemi yüklemesi gerekmez.

## İlk kullanım

1. Kayıtlı ağ yoksa Wi-Fi kurulum ekranı açılır. **2,4 GHz** ağınızı seçin, şifrenizi girin ve **Connect and save** düğmesine basın.
2. Ana ekrandan istasyon seçin. Ses seviyesini **− / sayısal değer / +** kontrolleriyle ayarlayın.
3. Türkçe için **Settings → Device → Dil / Language** bölümünde **Türkçe** seçip **Save** düğmesine basın.
4. Dil değişikliği cihaz yeniden başlatılmadan uygulanır. **Cancel/Vazgeç**, önceki dili korur. Diğer ayarlar da Kaydet/Vazgeç akışını kullanır.

Ağ daha sonra **Ayarlar → Cihaz → Wi-Fi ağını ayarla / değiştir** yolundan değiştirilebilir. Tek ağ profili saklanır. Wi-Fi bilgileri kaynak koda yazılarak derlenmez; cihaz ekranından girilir.

Çevrimdışı MP3 için açıksa Wi-Fi kurulumunu kapatın, SD kartı takın ve **SD MP3** seçin. Yerel müzik için internet bağlantısı gerekmez.

## SD MP3 oynatma

- MP3 dosyalarını SD kartın kök dizinine veya alt klasörlere yerleştirin.
- **SD MP3** listesinden klasöre girip dosya seçin. O klasördeki MP3 dosyaları çalma listesi olur.
- Başka klasörlerde gezinmek, yeni dosya seçilene kadar çalan parçayı ve çalma listesini değiştirmez.
- Ana ekrandan **Oynat/Duraklat**, **Önceki/Sonraki**, **−10/+10**, tekrar ve karışık çalma kontrollerini kullanın.
- Varsa ID3 başlık/sanatçı bilgisi kullanılır; yoksa dosya adı gösterilir.
- Süre alanı geçen/toplam zamanı gösterir. Çözücü yeterli zaman bilgisi oluşturduğunda sarma etkinleşir; VBR dosyalarda zaman tahminidir.
- Tekrar kapalıysa sonraki parçaya otomatik geçilir ve son parça bitince durulur.
- **Radyo / Radyoya dön** ile radyo kaynağına geçilir.

**Yeniden açılışta son MP3 en baştan çalar.** Konum ve duraklatma durumu bilerek kalıcı kaydedilmez. Önceki periyodik konum kayıtları RGB ekranda kaymaya yol açmıştı; bu yazımların kaldırılması gözlenen periyodik ve sarma kaynaklı kaymaları giderdi. Kaynak/dosya değişimleri kaydedilmeye devam eder.

Gezgin, klasör başına en fazla **128 MP3/alt klasör**, tarama başına en fazla **512 dizin kaydı** ve **240 bayt** dosya yolu destekler. `.mp3` uzantısı büyük/küçük harfe duyarsız listelenir. M4A, WebM ve diğer yerel biçimler bu oynatıcının mevcut kapsamı dışındadır.

## İstasyon listeleri: ortak CSV biçimi

Web arayüzü ve SD içe aktarma **UTF-8, noktalı virgülle ayrılmış CSV** kullanır:

```csv
name;url
Groove Salad;https://ice5.somafm.com/groovesalad-128-mp3
```

| Sınır | Değer |
| --- | ---: |
| Cihazda saklanan istasyon | CSV boyut sınırı içinde en fazla 128 |
| CSV dosyası | 12 KiB |
| İstasyon adı | 96 bayt |
| Yayın adresi | 512 bayt |

Adresler geçerli HTTP/HTTPS yayın adresi olmalıdır. Bir radyonun normal web sayfası veya oynatıcı sayfası doğrudan yayın adresi olmayabilir. UTF-8 bayt sınırları karakter sayısı değildir.

Çevrimdışı yükleme:

1. CSV dosyalarını **SD kartın kök dizinine** kopyalayın.
2. Radyo modunda **Ayarlar → İstasyonlar → SD karttan liste yükle** yolunu açın.
3. Dosyayı seçip onay ekranını inceleyin.
4. Cihazdaki tüm istasyonları değiştirmek için **Listeyi değiştir**, mevcut listeyi korumak için **Vazgeç** seçin.

Yüklenen liste dahili NVS belleğinde saklanır; SD kart çıkarılsa da korunur. İçe aktarma ayrı bir onaydır: doğrulanıp yüklenen liste, çevresindeki ayar menüsünde Vazgeç seçilerek geri alınmaz. Firmware SD karta yazmaz veya kartı biçimlendirmez. Listeleri birleştirme/sona ekleme henüz yoktur.

Örnekler: [design/sd-lists](design/sd-lists) ve [Türkiye radyo koleksiyonu](design/turkiye-radyo-listesi). Büyük koleksiyon cihaz sınırına uygun dosyalara bölünmüştür; tüm yayınlar doğrulanmış değildir ve adresler zamanla değişebilir.

## Yerel web arayüzü

Telefonunuzu veya bilgisayarınızı cihazla aynı ağa bağlayın. **Ayarlar → Cihaz** bölümündeki IP adresini kullanarak aşağıdaki adresi açın:

```text
http://DEVICE_IP/
```

Mevcut web arayüzü radyo kumandası, ses/sessize alma, istasyon seçimi, istasyon düzenleme/sıralama ve CSV içe/dışa aktarma sağlar. İstasyon değişiklikleri cihaza kaydedilene kadar tarayıcıda taslak olarak kalır.

**Mevcut kapsam sınırları:**

- Web arayüzü şu anda **yalnızca Türkçe**; cihaz dili seçimi web sayfasını değiştirmez.
- Cihaz ayarları/Wi-Fi kurulumu açıkken webden değişiklik engellenir. SD MP3 kaynağında uzaktan oynatma/ses kumandası ve Radyo kaynağına geçiş kullanılabilir; istasyon listesi kaydı ise engellenir.
- Webden MP3 klasör gezintisi ve MP3 dosyası yükleme henüz yoktur.
- Sunucu yerel HTTP kullanır; giriş hesabı/şifre ve TLS yoktur. Güvenilen yerel ağ için tasarlanmıştır; internete açma ve port yönlendirme desteklenen kurulumun dışındadır.

## Ekran ve ses ayarları

- **Parlaklık:** ayarlanabilir; kayıtlı seviye açılışta geri yüklenir.
- **Tek boşta kalma süresi:** önce ayarlanan parlaklığın %50'sine düşer, **15 saniye sonra** arka ışık kapanır. **Kapalı** seçimi iki aşamayı da devre dışı bırakır.
- **Uyandırma:** ilk dokunuş yalnızca ekranı uyandırır; altındaki kontrolü çalıştırmaz.
- **VU metre:** başlığın altında turuncu çubuk ve beyaz tepe işareti; 100 ms güncelleme ve 600 ms tepe tutma. Ayarlar → Ekran bölümünden kapatabilirsiniz. Kaydet/Vazgeç akışına uyar; arka ışık tamamen kapalıyken ölçüm ve çizim durur, uyanınca tercih açıksa sürer. Kalibre edilmiş akustik ses ölçümü değildir.
- **Ekolayzer:** bas, orta ve tiz; −12 ile +6 dB aralığı ve Düz seçeneği.
- **Açılış:** azami ses, açılış sesi, son sesi hatırlama, otomatik radyo ve istasyon tercihleri.

Son radyo/MP3 kaynağı hatırlanır. Radyonun açılışı otomatik yayın/istasyon tercihlerine uyar. Son MP3 veya kart bulunamazsa radyo kendiliğinden başlamaz; MP3 modunda mesaj gösterilir.

## Bilinen sınırlamalar ve tasarım kararları

- Parlaklık değiştirilirken hafif ses paraziti oluşabilir; kayıtlı ve kabul edilen bir sınırlamadır.
- Kaynak/parça değişimi veya ayar kaydı gibi flash yazımlarında kısa RGB görüntü kayması hâlâ mümkün. Periyodik MP3 konum yazımları kaldırılmıştır.
- MP3 başlıkları **sabit ve üç noktayla kısaltılmış** gösterilir. Önceden uzun başlıkların kayması seste kesinti oluşturuyordu; bildirilen sorun sabit gösterimle çözüldü.
- Yayın ve kayıtların ses yüksekliği farklı olabilir. Otomatik normalizasyon/AGC yoktur; manuel ses ve ekolayzer kullanılabilir.
- Bluetooth ses bu projenin kapsamı dışındadır. ESP32-S3, Bluetooth Classic/A2DP desteği sunmaz.
- Yayın erişimi, codec ve parça bilgileri yayıncıya bağlıdır. Bazı MP3/AAC yayınları test edilmiştir; tüm codec/HLS yayınlarıyla uyumluluk iddia edilmez.
- Fontlar Türkçe/Latin metin desteği sağlar; tüm Unicode karakterlerini kapsamaz. Yayın bilgileri, dosya ve ağ adları çevrilmeden korunur.
- Temel işlevler tek donanım düzeninde test edilmiştir. Bu, her SD kartın, ağın, kart revizyonunun veya yayının doğrulandığı anlamına gelmez.

## Yol haritası

- İngilizce desteği ve daha fazla cihaz kumandasıyla web arayüzünü yeniden tasarlamak.
- Tüm listeyi değiştirmenin yanında birleştirme/sona ekleme sunmak.
- Ağ, depolama, hata koşulları, uzun süreli kullanım ve fiziksel cihazda web OTA güncelleme/rollback testlerini sürdürmek.
- Sonraki donanım aşaması: **1S2P düzende iki adet 3200 mAh 18650**, UPS besleme ve harici durum LED'leri. Bu bir plandır; kurulmuş veya test edilmiş batarya sistemi değildir. Ekranda pil göstergesi şimdilik kapsam dışıdır.

Klasör gezintisi, ±10 saniye sarma, son kaynak/dosya kaydı, Türkçe/İngilizce cihaz menüleri ve yerel web üzerinden OTA firmware yükleme uygulanmıştır. MP3'ün kayıtlı zaman konumundan devam etmesi açık iş değildir; bu gereksinim iptal edilmiştir.

## Proje yapısı ve belgeler

```text
boards/          Özel Sunton kart tanımı
include/         Uygulama başlıkları, UI ayarları ve gömülü web sayfası
src/             Radyo, MP3, ayarlar, dil desteği ve üretilmiş font kaynakları
lib/             Yerel ses/ekran kütüphaneleri ve değişiklik notları
design/          Tasarım notları, istasyon örnekleri ve font lisansı
tests/host/      Bilgisayar test kaynakları ve benzetimleri
platformio.ini   Sabitlenmiş PlatformIO yapılandırması
```

Ayrıntılı mühendislik notları ağırlıklı olarak Türkçedir:

- [Dil desteği](design/LANGUAGES.md)
- [SD MP3 davranışı](design/SD-MUSIC.md)
- [CSV istasyon listeleri](design/STATION-LISTS.md)
- [Wi-Fi kurulumu](design/WIFI-SETUP.md)
- [Ayarlar](design/SETTINGS.md)
- [Web arayüzü](design/WEB-INTERFACE.md)
- [Batarya planı — ertelenen donanım işi](design/BATTERY-POWER.md)
- [Favoriler](design/FAVORITES.md)
- [Yol haritası](design/TODO.md)

Üretilmiş font kaynakları projededir; yeniden üretim notları [design/fonts](design/fonts) altındadır.

## Katkı sahipleri ve lisans

Projenin yönlendirmesi, bütünleştirilmesi ve sürdürülmesi: **fsadak**. Firmware başka geliştiricilerin çalışmalarını kullanır; bu bileşenlerin bildirim ve lisansları geçerliliğini korur.

| Bileşen | Kaynak / lisans notu |
| --- | --- |
| ESP32-audioI2S 2.0.7 | [esphome/ESP32-audioI2S](https://github.com/esphome/ESP32-audioI2S), ilk geliştirici schreibfaul1; [projede bulunan GPLv3 lisansı](lib/ESP32-audioI2S/LICENSE) |
| esp32_smartdisplay 2.1.2 | [rzeldent/esp32-smartdisplay](https://github.com/rzeldent/esp32-smartdisplay), René Zeldenthuis; upstream metadata MIT derken LICENSE dosyası GPLv3 içeriyor; inceleme açık |
| LVGL 9.6.0 | [LVGL](https://github.com/lvgl/lvgl), MIT; telif/lisans bildirimleri korunmalıdır |
| Espressif bileşenleri | Projedeki ilgili dosyalarda Apache-2.0 bildirimleri bulunur; bildirimler ve geçerli lisans koşulları korunmalıdır |
| Montserrat font verisi | [Montserrat Project](https://github.com/JulietaUla/Montserrat); [SIL Open Font License 1.1](design/fonts/Montserrat-OFL.txt) |

Yerel değişiklikler: [ses kütüphanesi notları](lib/ESP32-audioI2S/LOCAL-CHANGES.md) ve [ekran kütüphanesi notları](lib/esp32_smartdisplay/LOCAL-CHANGES.md).

**Lisans:** Projenin özgün katkıları [GNU GPLv3](LICENSE) kapsamında sunulur; kapsamı [AUTHORS.md](AUTHORS.md) dosyasında açıklanmıştır. Üçüncü taraf hakları ayrıdır. Helix kökenli çözücülerin lisans zinciri henüz netleşmemiştir; açıklama [upstream issue #23](https://github.com/esphome/ESP32-audioI2S/issues/23) üzerinden istenmiştir. Korunan bildirimler ve açık sorular için [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md) dosyasını inceleyin. Yayınlama bu soruları çözmez veya ek bir lisans izni oluşturmaz.

## Katkıda bulunma

Proje **fsadak** tarafından sürdürülür. Depo yayımlandığında öneriler issue ve pull request üzerinden iletilebilir; değişiklikler sürdürücü incelemesinden sonra resmî sürüme alınır.

Hata bildirirken firmware sürümünü, kart/PCB revizyonunu, tekrar adımlarını, beklenen/gerçekleşen davranışı ve ilgili logları ekleyin. Paylaşmadan önce şifreleri, token'ları ve kişisel ağ bilgilerini çıkarın. Flash/NVS dökümlerini veya özel yedekleri paylaşmayın.

Cihazda doğrulanmış sonuçları öneri, benzetim ve tasarım önizlemelerinden ayırarak belirtin.
