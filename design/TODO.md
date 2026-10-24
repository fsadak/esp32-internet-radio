# Yapılacaklar

## GitHub yayın hazırlığı yeniden başlatıldı — 2026-10-24

- Kullanıcı, Helix lisans açıklaması beklenmeden GitHub push hazırlığına devam
  edilmesini istedi. Lisans sorusu çözülmüş sayılmadı; issue #23 ve mevcut
  üçüncü taraf bildirimleri korunur. Kaynak veya telif bildirimleri kaldırılmaz.
- Hedef `fsadak/esp32-internet-radio`. Güncel kaynaklar, VU kabul kayıtları ve
  iki dilli README güncellemeleri ayrı temiz yayın adayına alınacak.
- Eski 2026-10-22 raporları tarihsel kayıttır; lisans yanıtı bekleme kararı bu
  yeni kullanıcı talebiyle yayın hazırlığı için kaldırıldı.

## VU / tepe seviye göstergesi — temel testler tamamlandı (2026-10-24)

- Kullanıcı isteğiyle sol üst başlığın altına 340 × 10 piksel yatay turuncu
  çubuk ve beyaz tepe işareti eklendi. Tepe 600 ms tutulur, ardından düşer.
- I2S öncesi, ekolayzer/ses ayarından sonraki dijital örneklerden tepe ölçülür;
  ses örnekleri değiştirilmez. FFT, SD/flash kaydı veya yeni görev yoktur.
- Güncel görsel güncelleme 100 ms (10 Hz) aralıkla yapılır; yalnız değişen çubuk
  genişliği ve tepe konumu güncellenir. Ölçek -48..0 dBFS;
  kalibre edilmiş VU ölçümü değildir.
- İlk kısa kullanıcı gözleminde belirgin performans sorunu görülmedi. Daha sonra
  bildirilen küçük kesilmeler üzerine çizimsiz tanılama ve 200 ms (5 Hz) çizim
  denemesi hazırlandı. Kullanıcı karşılaştırmada farklı MP3 seçtiğini, kesilmelerin
  sorunlu dosyada bulunduğunu açıkladı. Önceki A/B karşılaştırması geçersizdir;
  VU çiziminin kesilmeye yol açtığı sonucu doğrulanmış sayılmaz.
- Kullanıcı isteğiyle ilk hareketli sürüme dönüldü: ölçüm ve çizim 100 ms,
  tepe tutma 600 ms. Çizimsiz tanılama değişkeni ve 5 Hz çizim sınırlaması kaldırıldı.
  Kullanıcı VU öncesi sorunsuz çalan iki MP3 ile 10 Hz sürümünde de kesinti
  olmadığını doğruladı ve göstergenin bu şekilde kalmasını onayladı.
  Bu doğrulama tüm yayın/dosya/uzun süre senaryolarını kapsamaz.
- Kullanıcı `upload_speed = 921600` ile yüklemenin başarıyla tamamlandığını ve
  hızlandığını bildirdi. Kullanıcının platformio.ini değişikliği korundu.
- Sonraki kullanıcı talebi kodlandı:
  Ayarlar → Ekran altında VU metre / Settings → Display altında VU meter anahtarı.
  Varsayılan açık; kapatılınca gösterge gizlenir, örnek ölçümü ve çizim durur.
  Kaydet kalıcılaştırır; Vazgeç/çarpı önceki seçimi geri getirir.
  Arka ışık tamamen kapalıyken ölçüm/hesap/çizim durur; uyanınca kayıtlı veya
  önizlenen tercih açıksa otomatik sürer. %50 kararma aşamasında çalışır.
  Ayar kaydı sürüm 4; sürüm 1/2/3 değerleri korunur, eski kayıtlara VU açık eklenir.
  Sürüm 3 dili korunur; sürüm 2 dolgu baytı dil tercihi sayılmaz.
  2026-10-24 kullanıcı cihaz doğrulaması: MP3 çalarken kapatma/Kaydet sonrası
  gösterge gizlendi ve ses sürdü; kapatıp açınca kapalı tercih korundu.
  Açma/Vazgeç sonrası gizli kaldı; açma/Kaydet sonrası çubuk ve tepe yeniden
  hareket etti, ses kesilmedi. 30 saniye sonunda yarı parlaklık, 15 saniye sonra
  arka ışık kapanması ve sesin devamı; tek dokunuşla uyanma ve VU'nun yeniden
  hareketi doğrulandı. Temel kullanıcı test grubu tamamlandı.
  İç ölçümün durması kod davranışıdır; CPU/güç ölçümü yapılmadı. Tüm eski kayıt
  sürümlerinin geçişi, çarpıyla iptal, açık tercihin yeniden açılışta kalıcılığı
  ve uzun süreli/radyo testleri bu onaylarla ayrıca doğrulanmış sayılmaz.
  Asistan derleme/yükleme/otomatik test yapmadı.
  Yedek: `display-test/backups/before-vu-settings-20261022-093330/`.
- Önceki dosyalar `display-test/backups/before-vu-prototype-20261022-084626/`
  altında yedeklendi. Yayın adayı kopyasına VU değişiklikleri henüz aktarılmadı.

## OTA güncelleme — sonraya bırakıldı (2026-10-22)

- Kullanıcı isteğiyle, yerel ağdaki web arayüzünden firmware güncelleme
  (OTA) gelecekte geliştirilecek özellik olarak kaydedildi; henüz uygulanmadı.
- Tasarım aşamasında bölüm tablosu/uygulama alanı, güncelleme dosyasının
  doğrulanması, erişim yetkisi ve başarısız veya kesilen güncellemeden kurtarma
  yöntemi değerlendirilecek. Ayarlar ve istasyon kayıtlarının korunması ele alınacak.
- yoRadio yalnızca karşılaştırma amacıyla incelendi. Projeye geçiş veya tümünü
  mevcut firmware'e uyarlama planı yoktur; bu kayıt yalnızca OTA geliştirmesidir.

## İki dilli cihaz arayüzü — 0.10.0 (2026-10-22)

- Kullanıcı GitHub hazırlığını beklemeye aldı; önce Türkçe/İngilizce cihaz dili.
- Varsayılan English. Settings → Device → Dil / Language altında seçim;
  Save/Kaydet ile uygulama ve kalıcılık, Cancel/Vazgeç ile mevcut dili koruma kodlandı.
- Ana ekran, ayarlar/ekolayzer, Wi-Fi, istasyon/dosya listeleri, CSV onayları ve
  cihaz durum/hataları merkezi çeviri tablosunu kullanır. Metadata/SSID/adresler
  çevrilmez. Ekran yeniden kurulmaz veya cihaz yeniden başlatılmaz.
- Ayar kayıt sürümü 3; eski sürüm 1/2 değerleri korunarak varsayılan dil eklenir.
  Wi-Fi, katalog ve son kaynak/parça kayıt biçimleri değiştirilmedi.
- Web çevirisi kapsamlı web tasarımına ertelendi; seri loglar kapsam dışı.
- 2026-10-22 kullanıcı doğrulaması: İngilizce açılış, Türkçeye geçiş ve oynatmanın
  sürmesi, Türkçe kalıcılığı, Vazgeç ile mevcut dili koruma, İngilizceye dönüş ve
  kalıcılık geçti. İngilizce MP3 klasör/oynatma, Wi-Fi tarama ve CSV onay/Cancel
  akışları da doğrulandı; dosya/ağ adları korundu. Temel test grubu tamamlandı.
- Ayrı doğrulanmayan hata/sınır/uzun süre testleri geçmiş sayılmaz.
  Ayrıntılı kullanıcı onayları ve kalan kontroller LANGUAGES.md dosyasında.
- GitHub hazırlığı kullanıcı yeniden başlatana kadar beklemede; yükleme yapılmadı.
- Yedek: display-test/backups/before-en-tr-language-20261022-015136.

## Güncel MP3 kapsamı — 2026-10-22

Kullanıcı isteğiyle 0.8.3 listesindeki aşağıdaki geliştirme grubu açık yapılacaklar
arasından çıkarıldı; yeniden geliştirme bekleyen özellik olarak gösterilmeyecek:

- Klasörler arasında gezinme: tamamlandı ve kullanıcı doğruladı.
- Parça içinde ileri/geri sarma: -10/+10 saniye olarak tamamlandı; duraklatma ve
  başlangıç/son sınırları dahil kullanıcı doğruladı.
- Yeniden açılışta son MP3 ve son radyo/MP3 modunu hatırlama: tamamlandı ve
  kullanıcı doğruladı. Kaldığı konumdan devam kapsamı kullanıcı kararıyla iptal
  edildi; son parça en baştan çalar. Konum kaydı yeniden yapılacak değildir.

Aşağıdaki eski sürüm notları geliştirme geçmişidir; bu üç madde için güncel açık
iş sayılmaz. Ayrı kaydedilmiş hata/sınır kontrolleri kendi durumlarını korur.

## Doğrulananlar

- Parlaklık kaydırılırken ses kesilmesi giderildi; kullanıcı onayladı.
- Ayarların temel ses, kalıcılık, karartma/uyandırma ve otomatik yayın testleri geçti.
- 0.3.1 ile kullanıcı birkaç reset sonrası yayının sorunsuz başladığını doğruladı.
- 0.3.2 SD kartlı tanılama başarılı; kullanıcı kart olmadan reset sonrası radyonun çaldığını doğruladı.

## SD kart — 0.4.0

- UTF-8, noktalı virgüllü `name;url` CSV biçimi kararlaştırıldı.
- Kök dizindeki CSV dosyalarından seçim, doğrulama ve ayrı onayla mevcut listenin
  tamamını değiştirme uygulandı; kullanıcı dosya seçimi ve değiştirmeyi doğruladı.
- Liste tek NVS kaydıyla saklanır; kart olmadan açılışta kullanılabilir.
- Hatalı dosyada veya kayıt başarısızlığında mevcut liste korunur.
- SD karta yazma ve biçimlendirme yapılmaz; açılışta otomatik içe aktarma yoktur.
- Mevcut listeye ekleme/birleştirme **sonraya bırakıldı**.
- Kullanım, sınırlar ve örnekler: `STATION-LISTS.md`, `sd-lists/`.
- Kullanıcı `sd-test.csv` seçimi, 1 istasyon önizlemesi, onayla değiştirme ve yayını doğruladı.
- Cihaz kapatılıp SD kart çıkarıldıktan sonra yeniden açılışta `Groove Salad (SD Test)`
  adı korundu ve yayın çaldı; kalıcılık kullanıcı tarafından doğrulandı.
- Kullanıcı boş URL içeren `sd-invalid.csv` dosyasının reddedildiğini, mevcut liste
  korundu mesajını ve menüden çıkınca istasyon adı ile yayının korunduğunu doğruladı.
- Kullanıcı `groove-salad.csv` önizlemesinden Vazgeç seçildiğinde mevcut
  `Groove Salad (SD Test)` adı ve yayının korunduğunu doğruladı.
- Kullanıcı üçlü listeyi yükledi; üç yayın da çaldı. Önceki/sonraki geçişleri ve
  iki yönde sarma (3→1 ve 1→3), listeden doğrudan Drone Zone seçimi doğrulandı.
- Otomatik yayın ve son istasyonu hatırlama açıkken reset sonrası Drone Zone (2/3)
  otomatik çaldı; kullanıcı doğruladı.
- Son istasyonu hatırlama kapalı, açılış istasyonu Secret Agent iken Groove Salad
  çalınıp reset yapıldı; Secret Agent (3/3) otomatik başladı. Kullanıcı doğruladı.
- SD istasyon yüklemenin temel kullanıcı akışları tamamlandı; kabul edilen sürüm 0.4.0.
- Türkiye seçme listesi 32 istasyon olarak yüklendi. Radyo 7 (1/32), Süper FM (2/32),
  TRT FM (4/32, AAC) ve Radyo Spor (32/32) çaldı; kullanıcı doğruladı.
- İstasyon listesinin 4. sayfasına erişim, son satırdan seçim ve Sonraki ile 32→1
  dönüşü doğrulandı. Diğer Türkiye yayınları ve HLS cihazda henüz denenmedi.
- 12 KB dosya, 64 dosya/512 dizin kaydı sınırları ile gerçek kayıt hatası/güç kesintisi
  donanım testleri henüz yapılmadı.

## Türkçe GUI — 0.4.1 hazırlığı

- Şarkı adı fontunun (28 px) Türkçe kapsamı eksikti; 14/24 px alanlar da tamamlandı.
- Tüm kullanılan font boyutları Türkçe/Latin eklerini ortak RadioFonts üzerinden alıyor.
- Menü, durum ve SD/CSV hata mesajları Türkçe harflerle yazıldı.
- Gerçek panelde menü ve Türkçe şarkı adı doğrulaması yüklemeden sonra yapılacak.
- Kodlama dönüşümünü değiştirecek bir kanıt olmadan ses kütüphanesi değiştirilmedi.

## SD karttan MP3 — sonraya bırakıldı

- Radyo / SD Müzik kaynak seçimi.
- SD dosya listesi, oynat/duraklat ve önceki/sonraki parça.
- Mevcut ses ve sessize alma kontrollerini ortak kullanma.
- İnternetsiz çalma; kaynak değişiminde eski yayını/dosyayı düzgün kapatma.
- Kart yok/çıkarılmış/okunamıyor ve desteklenmeyen dosya durumlarını ele alma.
- Bu işlevler henüz uygulanmayacak; kullanıcı MP3 işlemlerini açıkça yapılacaklara bıraktı.

## Batarya ve ekran güç yönetimi

- **2026-10-22 güncel karar — sonraki donanım iyileştirmelerine ertelendi:**
  1S2P paketin hazırlanması/koruması, UPS bağlantısı, USB besleme düzeni ve
  modülün harici LED bağlantıları aynı donanım aşamasında ele alınacak.
  Şimdiki aşamada montaj, bağlantı veya buna yönelik firmware geliştirmesi yok.
  Plan: USB adaptör → UPS IN; korumalı 1S2P paket → UPS B;
  UPS 5 V OUT → Sunton kartın ana 5 V/GND beslemesi.
  Bu topoloji planıdır; fiziksel uçlar, kutuplar, korumalar, bilgisayar USB'sinden
  geri besleme ve adaptör geçişi uygulamadan önce doğrulanacak.

- 2026-10-22 kullanıcı kararı: dört hücre yerine **2 × Orion 18650 3200 mAh**
  yeni hücre, **1S2P** paket (3,7 V nominal, 4,2 V tam dolu, 6400 mAh / 23,68 Wh).
- Şarj olurken radyo/MP3 çalışmaya devam etmeli. Bağımsız Motorobit 5 V/1 A UPS
  modülü üzerinden planlanıyor; bağlantı ve gerçek cihaz testleri henüz yapılmadı.
- Batarya seviyesi/durum takibi modülün LED göstergeleriyle yapılacak.
  GUI pil göstergesi ve buna yönelik ADC/fuel-gauge entegrasyonu **şimdilik iptal**;
  yeni kullanıcı talebi olmadan açık yazılım işi sayılmayacak.
- Ölçümler, seçimin gerekçesi ve bekleyen donanım doğrulamaları: BATTERY-POWER.md.

- Tek ayarlanabilir boşta kalma süresi: önce mevcut parlaklığın %50'sine düşme,
  ardından sabit 15 saniye sonra arka ışığı sıfıra indirme (0.8.4 kodlandı; test bekliyor).
- Arka ışığın sıfırlanması uygulanır; RGB panelin veya ekran beslemesinin tamamen
  kapatılması, kartın güç devresi ve dokunmatik bağlantısı incelendikten sonra değerlendirilir.
- Ses çalmaya devam ederken ekran kapalı kalabilmeli.
- İlk dokunuş yalnızca uyandırmalı; alttaki düğmeyi tetiklememeli.
- Mevcut karartma süresi kaydı kullanılır; ikinci süre ayarı yoktur. Kapalı seçimi
  iki aşamayı da devre dışı bırakır.
- Bataryalı kullanımda ölçümle tüketim ve uyanma güvenilirliği doğrulanmalı.
- Bu özellik ses kesilmesi düzeltmesinden ayrı uygulanacak.

## Sonraki özellikler

- Web arayüzü ve cihaz menüsünden erişim için gerçek adres/QR kodu.
- İstasyon ekleme/çıkarma ve toplu dosya yükleme.
- Wi-Fi ağı seçimi ve şifre girişi.

## Kabul edilen bilinen durum — parlaklık değişiminde ses

- Türkçe GUI düzeltmesinden sonraki kullanıcı testinde, parlaklık değiştirilirken seste
  hafif parazit duyulduğu bildirildi. Sürekli ses kesilmesi olarak raporlanmadı.
- Kullanıcı parlaklık ayarı seyrek değiştirildiğinden bu durumu kabul edilebilir buldu;
  yalnızca kayda alınmasını ve bu adımın geçilmesini istedi.
- Bu konu için düzeltme yapılmayacak; bu test adımı kapatıldı.
- Türkçe karakter sorununun düzeldiği, kullanıcının kendi derleme/yüklemesi sonrası doğrulandı.

## Alt sol yayın bilgileri — 0.4.2 hazırlığı

- Durum ve istasyon sayacı arasına tür/format satırı eklendi.
- Tür yayıncının `icy-genre` bilgisinden, format gerçek ses çözücüsünden alınır.
- Bilgi yokken `—` gösterilir; yayın değişiminde eski bilgiler temizlenir.
- Derleme/yükleme kullanıcı tarafından yapılacak; cihaz doğrulaması bekleniyor.

- 0.4.2 kullanıcı yüklemesi sonrası tür alanında kanal adı görüldü. 0.4.3: CSV veya
  sunucu istasyon adıyla eşleşen genre değeri gizlenir; `Tür: —` gösterilir.
  Başlıkların geliş sırası ve Süper FM / SUPER_FM gibi yazım farkları ele alındı.
  Tür için yeni bir kategori tahmini yapılmaz; kullanıcı derleme/yüklemesi bekleniyor.

- 0.4.4: Metadata göndermeyen yayında sürekli “Parça bilgisi bekleniyor” yerine şarkı
  ve sanatçı satırları boş bırakılır. Bilgi gelirse otomatik gösterilir; istasyon
  değişiminde önceki metin temizlenir. Kullanıcı derleme/yüklemesi bekleniyor.

- 0.4.4 sonrasında kullanıcı “OK sorun yok” bildirimiyle bu düzenlemeleri doğruladı.

## Ekolayzer — 0.5.0

- Ayarlar → Ses → Ekolayzer: bas, orta ve tiz için -12…+6 dB aralığında
  birer dB adımlı eksi/değer/artı kontrolleri; başlangıç 0 dB.
- Düz üç bandı sıfırlar. Geri, düzenlenen değerleri koruyarak Ses sekmesine döner.
- Değişiklikler ana döngüde Audio::setTone ile anında uygulanır.
  Yalnızca bant değerleri değiştiğinde filtreler yeniden hesaplanır.
- Kaydet kalıcı kaydeder; Vazgeç veya ayarları kapatma önceki sesi geri getirir.
- NVS config sürüm 2; sürüm 1 kayıtları tüm eski alanları korunarak okunur,
  ekolayzer sıfırdan başlar. Yeni biçim bir sonraki ayar kaydında yazılır.
- Yedek: display-test/backups/before-equalizer-20261020-130448.
- Derleme, yükleme ve cihaz testi yapılmadı; kullanıcı gerçekleştirecek.
- Bekleyen cihaz kontrolleri sırayla: ekran ve anlık bas etkisi; orta/tiz;
  sınırlar ve Düz; Vazgeç; Kaydet/reset kalıcılığı; istasyon değişiminde koruma;
  önceki parlaklık, ses ve açılış tercihlerinin korunması.

- 0.5.0 için kullanıcı ilk kullanım geri bildirimini “Harika oldu” olarak bildirdi.
  Bu, yukarıdaki her sınır/kalıcılık testinin ayrı ayrı yapıldığını göstermez.

## Web arayüzü — 0.6.0 hazırlığı

- Yerel IP adresinde siyah-turuncu sayfa; liste ekleme/düzenleme/silme/sıralama,
  CSV taslak yükleme ve kayıtlı CSV indirme kodlandı.
- Önceki/sonraki, istasyon seçimi, ses +/-, sessize alma ve yayın başlat/durdur eklendi.
- Ekran ile web aynı komut işleyicisini kullanır; durum webde 2 saniyede bir güncellenir.
- Aynı URL korunuyorsa çalan bağlantı korunur; çıkarılıyorsa ayrıca onayla durdurulur.
- Eski liste taslağı için kimlik kontrolü; cihazda ayarlar açıkken web yazma kilidi var.
- Kullanıcının isteği: normalde cihaz ekranından yapılan işlemlerin tamamı ileride
  webden de yapılabilmeli. Ekolayzer, ekran ayarları, açılış tercihleri ve ilerideki
  SD müzik kontrolleri bu ortak altyapıya eklenecek; ilk sürüm kapsamına alınmadı.
- Ayrıntı, API, sınırlar ve bekleyen adımlı testler: `WEB-INTERFACE.md`.
- Derleme/yükleme/test kullanıcı tarafından yapılacak; başarı henüz doğrulanmadı.

- Kullanıcı 0.6.0'ı derleyip yükledi. Temel web kumandası, iki yönlü ses durumu,
  taslak/geri alma, CSV indirme/yükleme önizlemesi, yayını koruyan ad/sıra değişimi,
  çalan istasyonu onayla çıkarma ve listeyi geri yükleme doğrulandı.
- Yerel ayar kilidi, reset sonrası liste/açılış davranışı ve iki sekmede eski
  taslağın reddi doğrulandı. Ayrıntı ve henüz denenmeyenler WEB-INTERFACE.md içinde.
- Boş URL'li CSV webde reddedildi; kullanıcı mevcut listenin ve kesintisiz yayının
  korunduğunu doğruladı. Web Önceki düğmesi ve iki ekranda istasyon uyumu da geçti.
  0.6.0 temel web kullanım testleri tamamlandı; sınır/yük testleri ayrı bekliyor.

## Wi-Fi kurulumu — 0.7.0 hazırlığı

- Kullanıcı web ayarlarından önce Wi-Fi ilk kurulum/değiştirmeyi önceliklendirdi.
- İlk kayıtsız açılışta ve kayıtlı ağa bağlanılamadığında dokunmatik kurulum açılır.
- Ayarlar → Cihaz üzerinden sonradan açma; ağ tarama/seçme, elle SSID, maskeli
  şifre ve ekran klavyesi kodlandı.
- Yeni ağ bağlantısı doğrulanmadan eski profil değiştirilmez; başarısız denemede
  veya iptalde eski kayıt korunur. Başarıdan sonra profil ayrı NVS kaydına yazılır.
- Sabit SSID/şifre kaynak kodundan kaldırıldı; ilk 0.7.0 yüklemesinde bir kez
  ekrandan ağ girmek gerekir. Diğer radyo ayarları ve liste korunur.
- Ayrıntı ve adımlı cihaz testleri `WIFI-SETUP.md` içinde. Derleme/yükleme ve
  cihaz testi henüz yapılmadı; kullanıcı gerçekleştirecek.
- Webden ekolayzer/ekran/açılış ayarları bekliyor; Wi-Fi sonrası devam edilecek.

- 0.7.0 ilk cihaz denemesinde ağ bulunamadı ve boş listede titreme/tepki yokluğu
  bildirildi. 0.7.1'de tarama varsayılan süreye döndürüldü, başlangıç ertelendi,
  başarısız tarama ayrıldı, bir yeniden deneme ve sayısal tanılama eklendi.
  Wi-Fi kontrol durumları yalnızca değiştiğinde uygulanıyor. Başarı henüz doğrulanmadı.

- 0.7.1 kullanıcı testinde de tarama -1 → -2 oldu; çözülmedi. 0.7.2 yalnızca
  eksik kayıt uyarısını ele alır ve süre/SCAN_DONE/sürücü/heap tanılaması ekler.
  Fayda sağlamayan başlangıç ertelemesi ve otomatik ikinci tarama kaldırıldı.
  Kök neden henüz doğrulanmadı; kullanıcı derleme/yükleme ve yeni log bekleniyor.

- Kullanıcı 0.7.2'de ağ tarama, listeleme ve seçimi doğruladı; şifre klavyesi
  görünmedi. 0.7.3'te LVGL alt hizalamasına eklenen +320 kaydırması kaldırıldı;
  klavye açıkça ekran altına hizalandı. Tarama/bağlantı kodu korunuyor.
  Klavye, ilk bağlantı ve kalıcılık testleri bekliyor.

- 0.7.3 kullanıcı testleri: klavye, ilk ağ bağlantısı/kayıt, radyo çalma, resetle
  şifresiz yeniden bağlantı ve açılış davranışı geçti. Kurulumu değişikliksiz açıp
  kapatma yayını korudu. Aynı ağda yanlış şifre sonrası eski ağa dönüş/yayın,
  resetle doğru kaydın korunması ve denemeyi erken iptal etme doğrulandı.
- Temel Wi-Fi akışları tamamlandı; farklı ağ/IP, gizli/açık ağ ve diğer özel
  koşullar ayrı bekliyor. Ayrıntılı kabul kaydı WIFI-SETUP.md dosyasında.

## SD MP3 — 0.8.0 hazırlığı

- Kullanıcı web işini sonraya bıraktı; daha önce ertelenen SD MP3 işini şimdi istedi.
- Ana ekranda kaynak seçimi, kök ve `/music` dosya listesi, dosyadan MP3 oynatma,
  duraklat/devam, önceki/sonraki ve radyo kaynağına dönüş kodlandı.
- Ortak ses/sessize alma/ekolayzer; ID3 veya dosya adı; parça sonunda sonraki dosya,
  listenin sonunda durma eklendi. Kart ve dosya hataları ayrı ele alınıyor.
- Wi-Fi geri geldiğinde SD dosyasını kesmeme; duraklamayı radyo hatasına karıştırmama;
  açık yerel dosyada kartı yeniden bağlamama kontrolleri eklendi.
- MP3 kaynağında CSV içe aktarma ve mevcut web yazma/kumandası kilitli. Kapsamlı
  web müzik yönetimi gelecekte tasarlanacak.
- O sürümde son MP3/konum kalıcılığı, klasör gezgini, sarma ve çalma modları ertelendi;
  sonraki sürümlerin güncel durumu aşağıdaki kayıtlardadır.
- Ayrıntılar `SD-MUSIC.md`; henüz derlenmedi/yüklenmedi, cihaz testleri kullanıcıda.

- 0.8.0 kullanıcı testinde MP3 listesi/seçimi, çalma, duraklat/devam, ortak ses,
  önceki/sonraki ve sayaç, listeden seçim, EOF'ta sonraki dosya ve son parçada durma,
  son radyo istasyonuna dönüş doğrulandı. İlk dosya bulunamadı bildirimi .m4a/.webm
  dosyaların MP3 sanılmasından kaynaklandı; gerçek MP3 ile listeleme çalıştı.
- 0.8.1: Tür yerine geçen/toplam süre, etiketli tek oynat/duraklat, tek parça ve
  tüm liste tekrarı ile karışık sıra eklendi. MP3 etiketi korundu. Modlar bu sürümde
  oturumluk; resetle kapalı başlar. Yeni özellikler henüz cihazda test edilmedi.

- 2026-10-21: 0.8.1'de kayan parça adıyla eşzamanlı MP3 kesintisi bildirildi.
  0.8.2'de yerel MP3 SD okuma parçası 16 KB'dan 2 KB'a indirildi ve LVGL dönüşünde
  ek ses beslemesi kondu. Kayan yazı korunuyor; düzeltme cihaz doğrulaması bekliyor.
  Bu hata kabul edilen parlaklık parazitinden ayrı tutuluyor.

- 0.8.2 kullanıcı testinde kesinti sürdü; kısa adlarda yok, kayan uzun adlarda var.
  Kullanıcı sabit/kısaltılmış başlık istedi. 0.8.3 MP3 başlığında kaydırmayı kapatır,
  sığmayan bölümü üç noktayla gösterir. Başarısız okuma-boyutu/ek besleme denemeleri
  geri alındı. Cihaz doğrulaması bekleniyor.

- 2026-10-21: Kullanıcı 0.8.3 sonrası “OK sorun çözüldü” bildirimiyle MP3 ses
  kesintisinin giderildiğini doğruladı. Çözüm: MP3 başlığının kaydırılması yerine
  sabit, sığmayan bölümde üç noktalı gösterim. Sonuç vermeyen 0.8.2 okuma-boyutu
  ve ek ses besleme denemeleri geri alındı. Bu hata kapatıldı; ayrıntılı belirti,
  deneme ve kabul kaydı SD-MUSIC.md içindedir. Diğer MP3 mod testleri ayrı bekliyor.

## Bluetooth — iptal edildi (2026-10-21)

- Kullanıcı kararı: Bluetooth özelliği donanım kısıtlamaları nedeniyle iptal edildi.
- Kullanılan ESP32-S3 yalnızca BLE destekler; Bluetooth Classic/A2DP ve LE Audio
  desteği yoktur. Mevcut kartın dahili Bluetooth'u ile standart Bluetooth ses
  alma veya kulaklık/hoparlöre ses gönderme hedefi gerçekleştirilemez.
- BLE üzerinden kumanda teknik olarak ayrı bir seçenek olsa da Bluetooth işi
  proje kapsamından çıkarıldı; BLE kumanda veya harici ses modülü için çalışma
  başlatılmayacak. Yeniden ele almak kullanıcının yeni talebine bağlıdır.
- Karar yalnızca kayda geçirildi; firmware kodu ve mevcut radyo/SD/web/Wi-Fi
  işlevleri değiştirilmedi.
- Teknik dayanak: https://documentation.espressif.com/projects/esp-adf/en/latest/solution-center/bluetooth-audio.html

## Otomatik ses dengeleme — iptal edildi (2026-10-21)

- Kullanıcı bazı radyo yayınları ve MP3 kayıtlarının aynı ses ayarında daha düşük
  duyulduğunu bildirdi. Kaynakların kayıt/yayın seviyelerinin farklı olması olası
  neden olarak değerlendirildi; sinyal ölçümüyle kesin neden doğrulanmadı.
- Cihazda AGC/ALC ve limiter ile otomatik dengeleme seçeneği görüşüldü;
  herhangi bir uygulama, performans ölçümü veya cihaz testi yapılmadı.
- Kullanıcı kararı: mevcut donanım/işlem yükü kısıtları gerekçesiyle otomatik ses
  dengeleme iptal edildi ve proje kapsamından çıkarıldı. Bu karar, ESP32-S3'te
  yazılımsal dengelemenin teknik olarak imkânsız olduğuna dair bir tespit değildir.
- AGC/ALC/limiter geliştirmesi başlatılmayacak; yeniden değerlendirme kullanıcının
  yeni talebine bağlıdır. Kaynaklar arasındaki seviye farkı bu aşamada manuel ses
  ayarıyla yönetilecek. MP3 dosyalarına toplu normalizasyon işlemi yapılmadı.
- Firmware ve mevcut ses/ekolayzer işlevleri değiştirilmedi.

## MP3 test güncellemesi — 2026-10-21

- Kullanıcı MP3 süre, tekrar ve karışık çalma testlerini kendisinin yaptığını
  bildirdi; bu test grubu tamamlandı ve kabul edildi olarak kaydedildi (0.8.3).
- Önceki kayıtlardaki bu üç gruba ait “test bekliyor” notları artık güncel değil.
- Ayrıntılı kabul kaydı SD-MUSIC.md içinde. Diğer bekleyen hata/sınır/uzun süre
  testlerinin durumu değiştirilmedi. Firmware koduna dokunulmadı.

## Tek süreyle iki aşamalı karartma — 0.8.4

- Kullanıcı iki ayrı süre istemedi: mevcut süre dolunca parlaklık yarıya iner;
  bu aşamadan sabit 15 saniye sonra arka ışık tamamen kapanır.
- Kapalı seçimi iki aşamayı da kapatır. İlk dokunuş yalnızca uyandırır ve yeni
  bekleme süresini başlatır. Radyo/MP3 oynatma akışı değiştirilmedi.
- Mevcut süre ve parlaklık kayıtları kullanılır; ek NVS alanı/migrasyon yoktur.
- Yedek: display-test/backups/before-two-stage-dimming-20261021-150354.
- Derleme/yükleme ve cihaz doğrulaması kullanıcıdan bekleniyor.

- 2026-10-21: Kullanıcı 0.8.4 için “OK test ettim sorunsuz çalışıyor” onayı verdi.
  Tek süre → yarı parlaklık → 15 saniye sonra arka ışığı kapatma akışı kabul edildi.
  Ayrıntılı onay SETTINGS.md içinde; ayrı ayrı raporlanmayan sınır/kalıcılık
  kontrolleri yapılmış sayılmadı. Kod değiştirilmedi.


## Klasör, ±10 saniye ve son kaynaktan devam — 0.9.0 (2026-10-21)

- Alt klasörlerde gezinme, üst klasöre dönüş ve klasöre göre MP3 çalma listesi kodlandı.
  Sadece gezinmek çalan kaynağı/listeyi değiştirmez; dosya seçmek değiştirir.
- MP3 ekranına -10/+10 saniye düğmeleri eklendi; başlangıç/son sınırı korunur.
  Süre/bit hızı henüz bilinmiyorsa sarma düğmeleri kapalıdır.
- Son radyo/MP3 modu, MP3 yolu, boyutu, byte/zaman konumu ve duraklama durumu
  ayrı NVS kaydında tutulur. Mevcut radyo ayarı/katalog kaydı değiştirilmedi.
- MP3 konumu 10 saniyede bir, duraklatma/sarma ve kaynak/parça değişiminde kaydedilir.
  Ani güç kesilmesinde son kayıttan sonraki yaklaşık 10 saniye tekrar çalabilir.
- Eksik kart/dosya halinde MP3 modunda hata verilir; radyo kendiliğinden açılmaz
  ve eski devam kaydı korunur. Dosya boyutu değişmişse eski konum kullanılmaz.
- Kayan MP3 başlığı kapalı kalır; başarısız 0.8.2 deneyleri yeniden uygulanmadı.
- Ses kütüphanesinde konum/zaman hesabı, bekleyen konum okuma ve sonlu MP3
  kare araması düzenlendi. 16 KB okuma boyutu ve ana ses besleme düzeni korunur.
- Yedek: display-test/backups/before-folder-seek-resume-20261021-151341.
- Durum: kod ve kaynak üzerinden inceleme tamamlandı; derleme/yükleme yapılmadı.
  Kullanıcı derleme/yükleme sonrası OK dediğinde cihaz testleri tek tek verilecek:
  klasör gezintisi, ±10/sınırlar/duraklama, MP3 reset/güç kesilmesi, radyo modu,
  eksik kart/dosya, boyutu değişen dosya ve önceki tekrar/karışık çalma davranışı.

## RGB kayması tanılaması — 0.9.1-diag (2026-10-21)

- Kullanıcı dört klasörü görme, klasörden dosya seçme, başka klasörde gezinirken
  çalan dosyanın korunması, yeni klasörün parça sayacı ve ±10 saniyeyi doğruladı.
- Yaklaşık 10 saniyede bir ve düğmelere basınca tüm görüntünün hafif sağa kayıp
  geri geldiğini bildirdi. Bu zamanlar yeni NVS devam kaydıyla örtüşüyor.
- Olası neden: flash yazımı sırasında PSRAM'dan RGB görüntü aktarımının aksaması.
  Bu henüz kontrollü cihaz testiyle kanıtlanmadı; kesin neden olarak kaydedilmedi.
- Tek değişkenli tanılama: PlaybackResume modülündeki flash yazıları geçici kapalı.
  Eski kayıt okunur ve korunur; yeni mod/dosya/konum yalnızca RAM'de güncellenir,
  yeniden açılışa kaydedilmez. Genel ayarlar/radyo kayıtları değiştirilmedi.
- Ekran saat/pin/zamanlamaları, ses besleme ve SD okuması değiştirilmedi.
- Kalıcı düzeltme değildir; derleme/yükleme ve kayma karşılaştırması kullanıcıda.
  Sonuçtan sonra kalıcılığı koruyan çözüm ayrıca uygulanmalı; bu bayrak kapalı
  bırakılarak özellik tamamlandı sayılmamalı.
- Yedek: display-test/backups/before-resume-flash-diagnostic-20261021-154200.
- Kaynak: https://docs.espressif.com/projects/esp-faq/en/latest/software-framework/peripherals/lcd.html

## Yalnız son parça ve mod kaydı — 0.9.2 (2026-10-21)

- Kullanıcı 0.9.1-diag ile 40 saniye bekleme ve ±10 düğmelerinde kaymanın tamamen
  kaybolduğunu doğruladı. Yeni devam kaydının flash yazılarıyla ilişki doğrulandı.
- Kullanıcı SD karta periyodik kayıt istemedi; konumdan devam özelliğini iptal
  ederek yeniden açılışta son şarkının baştan çalmasını istedi.
- Tanılama bayrağı kaldırıldı. Dahili NVS'ye yalnız mod/dosya bilgisi kaydedilir:
  ilk seçim, elle parça değiştirme veya doğal bitişte yeni parça açılması ve
  radyo/MP3 modu değişimi. Aynı kayıt tekrar yazılmaz. SD karta veri yazılmaz.
- Konum, süre ve duraklama durumu kaydedilmez; eski konum kayıtları yok sayılır.
  10 saniyelik düzenli kayıt, sarma/duraklatma/durdurma kayıtları kaldırıldı.
  Yalnız başarısız parça/mod kaydının 10 saniye arayla hata sonrası tekrarı vardır.
- Klasör gezgini, ±10 saniye, tekrar/karışık çalma ve eksik kart/dosya koruması korunur.
- Parça/mod geçişinde flash yazısına bağlı kısa kayma hâlâ mümkün; bu düzenleme
  flash kaynaklı her kaymayı giderdi iddiası taşımaz. Yeni sürüm cihaz testi bekler.
- Yedek: display-test/backups/before-track-only-resume-20261021-154845.
- Derleme/yükleme yapılmadı; kullanıcı yapacak ve OK sonrası testler tek tek verilecek.

## 0.9.2 kullanıcı doğrulaması — 2026-10-21

- Kullanıcı derleme/yükleme sonrasında aşağıdaki cihaz testlerini onayladı:
  - MP3 çalarken en az 40 saniyelik bekleme ve ±10 saniye düğmelerinde ekran sabit.
  - MP3 sırasında kapat/aç: aynı dosya MP3 modunda en baştan çalıyor.
  - Elle Sonraki ile seçilen yeni dosya kapat/aç sonrasında en baştan çalıyor.
  - Radyo moduna geçip kapat/aç: radyo modunda açılıyor ve yayın çalıyor.
  - Tekrar/karışık kapalıyken doğal parça sonundan otomatik geçilen yeni dosya,
    kapat/aç sonrasında en baştan çalıyor. Parça sonuna ±10 ile yaklaşıldı.
- Daha önce doğrulanan klasör gezintisi ve klasöre göre parça listesiyle birlikte
  bu temel geliştirme akışları kullanıcı tarafından kabul edildi.
- Periyodik ve sarma kaynaklı ekran kayması bu test kapsamında giderildi.
  Parça/mod değişiminde hiç kayma olmadığı sonucu çıkarılmadı.
- Eksik kart/dosya, sarma sınırları ve duraklatılmış dosyada sarma gibi ayrı
  hata/sınır kontrolleri bu onaylarla yapılmış sayılmadı; bekliyor.
- Bu güncelleme yalnız test kaydıdır; firmware değiştirilmedi veya derlenmedi.

### SD kart yokluğu ve geri takılması — kullanıcı onayı

- MP3 çalarken kapatıp SD kart çıkarılarak açıldığında cihaz MP3 modunda kaldı,
  kart/dosya hatası gösterdi; radyo kendiliğinden başlamadı. Kullanıcı doğruladı.
- Cihaz kapatılıp aynı kart geri takılarak açıldığında önceki son MP3, MP3
  modunda en baştan çaldı. Kullanıcı doğruladı; devam kaydı korunuyor.
- Önceki bekleyen testler içinden eksik kart ve aynı kartla toparlanma tamamlandı.
  Kart mevcutken yalnız dosyanın silinmesi, sarma sınırları ve duraklatılmış
  dosyada sarma ayrı doğrulama bekliyor. Kod değiştirilmedi.

### Sarma ve sınır testleri — 2026-10-22 kullanıcı onayı

- Yaklaşık 30 saniyede duraklatılan parçaya -10 uygulanıp oynatıldığında
  yaklaşık 10 saniye geriden devam ettiği doğrulandı.
- Art arda -10 ile başlangıca dönüşte süre negatif olmadı; cihaz takılmadan
  parça baştan çalmayı sürdürdü. Kullanıcı doğruladı.
- Duraklatılmış parçaya art arda +10 uygulandığında toplam süre aşılmadı;
  sonun yaklaşık bir saniye önünde durdu ve +10 pasifleşti. Kullanıcı doğruladı.
- Bu üç test için önceki bekleyen notlar kapandı. Kart mevcutken yalnız son
  dosyanın silinmesi ve parça/mod geçişindeki görüntü davranışı ayrı bekliyor.
- Yalnız doğrulama kayıtları güncellendi; firmware değiştirilmedi/derlenmedi.
