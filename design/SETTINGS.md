# Ayarlar menüsü — 0.3.0

## VU metre tercihi — 2026-10-22

- Ayarlar → Ekran / Settings → Display bölümünde VU metre / VU meter anahtarı
  eklendi. Varsayılan açıktır; kapalıyken ana ekrandaki gösterge gizlenir.
- Tercih önizlemesi anında uygulanır. Kalıcılaştırmak için Kaydet / Save seçin;
  Vazgeç / Cancel veya çarpı önceki tercihi geri getirir.
- Kapalı tercihte ve arka ışık tamamen kapalıyken VU örnek ölçümü, seviye/tepe
  hesabı ve periyodik gösterge çizimi durur. Ses çıkışı devam eder.
  I2S callback'i yalnızca sesi geçiren kısa kontrolü çalıştırır.
- %50 kararma aşamasında gösterge çalışır. İlk dokunuşla ekran uyandığında,
  tercih açıksa gösterge 100 ms güncellemeyle yeniden çalışır.
- Ayar bloğu sürüm 4 oldu. Eski 1/2/3 kayıtların değerleri korunarak VU açık
  varsayılır. Sürüm 3 dil seçimi korunur; eski dolgu baytları tercih sayılmaz.
- 10 Hz göstergenin iki sağlam MP3 ile kesintisiz çalışması kullanıcı tarafından
  doğrulandı. 2026-10-24 tarihinde kullanıcı şu testleri de onayladı:
  kapatma/Kaydet ile gizlenme ve sesin devamı; yeniden açılışta kapalı tercihin
  korunması; açma/Vazgeç ile kapalı tercihe dönüş; açma/Kaydet ile çubuk ve tepenin
  yeniden hareketi ve kesintisiz ses; 30 saniye kararma, 15 saniye sonra arka ışığın
  kapanması, sesin devamı ve tek dokunuşla uyanıp VU hareketinin geri gelmesi.
  Temel kullanıcı test grubu tamamlandı. CPU/güç ölçümü yapılmadı; bütün eski kayıt
  geçişleri, çarpıyla iptal, açık tercihin yeniden açılış kalıcılığı ve uzun süreli
  radyo testi ayrıca doğrulanmış sayılmaz. Asistan derleme/yükleme/otomatik test yapmadı.

## Bölümler

- Ekran: %5–100 parlaklık; karartma kapalı / 30 saniye / 1, 2, 5, 10 dakika.
  Varsayılan parlaklık 160/255 (yaklaşık %63), karartma 2 dakika. 0.8.4 ile seçilen
  süre sonunda mevcut parlaklık yarıya iner; 15 saniye sonra arka ışık sıfırlanır.
  Kapalı seçimi iki aşamayı da kapatır. İlk dokunuş yalnızca uyandırır; alttaki düğmeye basmaz.
- Ses: açılış seviyesi 0–21, azami seviye 0–21, son seviyeyi hatırlama.
  Başlangıç değeri 11, azami 21, hatırlama açık. Açılış seviyesi ve azami seviye
  değişiklikleri mevcut ses üzerinde denenebilir; sessize alma seviyesi saklanmaz.
- Radyo: otomatik yayın, son istasyonu hatırlama ve sabit açılış istasyonu.
  Varsayılan olarak otomatik yayın ve son istasyon açıktır. Son istasyon seçiliyken
  sabit istasyon alanı pasiftir. Otomatik yayın kapalıysa açılışta istasyon
  listesinden seçim yapılarak yayın başlatılır.
- Cihaz: Wi-Fi bağlantısı, sinyal gücü, IP adresi ve yazılım sürümü. Şifre gösterilmez.
  Web sunucusu henüz olmadığı için çalışmayan bir adres veya QR kodu üretilmez.

## Kaydet ve Vazgeç

Menü açılırken o anki ses ve sessiz durumu saklanır. Parlaklık ve ses sınırı
anında denenir. Kaydet, sürümlü ayar bloğunu Preferences/NVS üzerinden kalıcı
yazar; başarılı olursa menü kapanır. Hata olursa menü açık kalır. Vazgeç veya
çarpı, önceki parlaklığı, ses sınırını, ses seviyesini ve sessiz durumunu geri getirir.
Radyo menü açıkken çalmaya devam eder; açılış tercihleri mevcut yayını kesmez.

Son ses/istasyon bilgisi, düzenleme sırasında yazılmaz. Normal kullanımda son
değişiklikten 2 saniye sonra, yalnızca değer değişmişse yazılır. Bu iki saniye
içinde ani güç kesilirse son tuş değişikliği hatırlanmayabilir. Kalıcı ayar bloğu
tek NVS yazımıyla saklanır. Uyumsuz sürüm veya geçersiz değerler varsayılana döner.

## Kartta doğrulama

1. Yayın çalarken ayarların dört sekmesini açın; yayının devam ettiğini kontrol edin.
2. Parlaklık ve sesi değiştirip Vazgeç'e basın; önceki değerlerin geri gelmesini kontrol edin.
3. Ses sınırını düşürün, Kaydet'e basın; ana ekrandaki + sınırda pasif olmalı.
4. 30 saniye karartmayı kaydedin. Bekleyip + üzerine bir kez dokunun:
   yalnızca aydınlanmalı; ikinci dokunuş sesi artırmalı.
5. Tercihleri kaydedip yeniden başlatın; parlaklık, ses sınırı ve açılış tercihlerini kontrol edin.
6. Son ses hatırlamayı kapatıp açılış seviyesini seçin. Yeniden başlatmada seçilen seviye kullanılmalı.
7. Otomatik yayını kapatın; yeniden başlatınca yayın beklemeli, listeden seçimle başlamalı.

Web arayüzü, istasyon düzenleme/toplu dosya yükleme ve Wi-Fi şifre girişi ayrı aşamalardır.

## Doğrulama durumu

2026-10-20: Kullanıcı parlaklık önizlemesini ve Vazgeç ile geri dönüşü doğruladı. İlk sürümdeki kaydırma sırasında ses kesilmesi düzeltildi ve kullanıcı tarafından yeniden test edilerek onaylandı. Kaydet/yeniden açılış, karartma/uyandırma ve diğer açılış tercihleri henüz doğrulanmadı.

2026-10-20: Kullanıcı parlaklığı Kaydet ile saklayıp cihazı kapatıp açtı; seçilen parlaklığın korunduğunu doğruladı.

2026-10-20: Kullanıcı 30 saniye karartmayı ve ilk dokunuşun yalnızca ekranı uyandırıp ses seviyesini değiştirmediğini doğruladı.

2026-10-20: Kullanıcı azami sesi 8 olarak kaydetti; ana ekranda sesin 8 üzerinde artmadığını ve + tuşunun sınırda pasif olduğunu doğruladı.

2026-10-20: Kullanıcı son ses seviyesini hatırlama açıkken sesi 5 yaptı, bekleyip cihazı kapatıp açtı; seviyenin 5 olarak geri geldiğini doğruladı.

2026-10-20: Kullanıcı son ses hatırlamayı kapatıp açılış sesini 3 yaptı. Ana ekranda sesi 5 yaptıktan sonra yeniden açılışta sesin 3 olduğunu doğruladı.

2026-10-20: Kullanıcı otomatik yayın kapalıyken açılışta beklendiğini ve listeden Groove Salad seçilince çalmaya başladığını doğruladı. Reset sonrası aralıklı HTTP 403 sorunu ayrı olarak HTTP-RECOVERY.md dosyasında incelendi.

## 0.8.4 — tek süreyle iki aşamalı karartma (2026-10-21)

Kullanıcı ikinci bir ayarlanabilir süre istemedi. Mevcut karartma süresi korunur:

1. Seçilen süre dokunmadan geçince arka ışık, kullanıcının seçtiği parlaklığın
   yarısına iner. Örneğin normal %60 ise ilk aşama yaklaşık %30'dur.
2. Kararma başladıktan 15 saniye sonra arka ışık sıfırlanır.
3. İki aşamada da ilk dokunuş normal parlaklığı geri getirir, altındaki düğmeyi
   çalıştırmaz; bekleme sayacı yeniden başlar.

Örnek: 30 saniye seçilirse yaklaşık 30. saniyede yarı parlaklık, 45. saniyede
tam karanlık. Kapalı seçilirse her iki aşama devre dışıdır. Kaydet/Vazgeç aynı
ayar akışını kullanır; eski parlaklık ve süre kayıtları geçerlidir.

Arka ışık yalnızca aşama değiştiğinde ayarlanır; her döngüde PWM güncellemesi yoktur.
Radyo/MP3 oynatma durdurulmaz. RGB panel beslemesi kesilmez; bu, batarya için
gerçek güç kapatma ölçümü/uygulaması yerine geçmez.

Kaynak yedeği: `display-test/backups/before-two-stage-dimming-20261021-150354`.
Derleme, yükleme ve test yapılmadı. Bekleyen kullanıcı kontrolleri sırayla:
30 saniye + 15 saniye, her aşamada yalnız uyandıran dokunuş, sesin devamı,
Kapalı seçimi ve reset sonrası ayarın korunması.

### Kullanıcı onayı — 2026-10-21

Kullanıcı 0.8.4 için “OK test ettim sorunsuz çalışıyor” bildirdi. Tek ayarlanan
süre sonunda mevcut parlaklığın yarıya inmesi ve 15 saniye sonra arka ışığın
kapanması davranışı kullanıcı tarafından kabul edildi.

Ayrı adım/ölçüm kaydı verilmediğinden Kapalı seçimi, her aşamada dokunuş,
radyo ve MP3'ün ayrı ayrı devamı veya reset kalıcılığı için yeni, ayrıntılı test
sonuçları varsayılmadı. Önceki temel ayar testleri kendi kayıtlarında geçerlidir.
# Dil seçimi — 0.10.0 güncellemesi (2026-10-22)

Device/Cihaz sekmesine **Dil / Language** seçimi eklendi: English ve Türkçe.
Tercih yoksa varsayılan English. Kaydet başarılı olduğunda yeniden başlatmadan
uygulanır; Vazgeç dili değiştirmez. Genel ayar kaydı sürüm 3 oldu; önceki sürüm
1/2 ayarları korunarak okunur. Dil değişimi ve sesin sürmesi cihaz testi bekler.
Ayrıntılar ve güncel dil kapsamı: LANGUAGES.md. Bu dosyadaki önceki sürüm
kayıtları tarihsel açıklamaları içerir.

2026-10-22 kullanıcı doğrulaması: İngilizce açılış, Türkçe kaydetme ve yeniden
açılışta korunması, English taslağından Vazgeç ile Türkçeyi koruma, tekrar
İngilizce kaydedip yeniden açılışta korunması geçti. Dil uygulanırken oynatma
sürdü; cihaz yeniden başlamadı. Ayrıntılı kapsam ve ayrı bekleyen kontroller
LANGUAGES.md içindeki kullanıcı doğrulaması bölümündedir.

