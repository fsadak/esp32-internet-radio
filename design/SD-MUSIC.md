# SD karttan MP3 — 0.9.2

## Son cihaz doğrulaması — 2026-10-21

Kullanıcı 0.9.2'de 40 saniyelik oynatma ve ±10 saniye sırasında görüntünün sabit
kaldığını; kapat/aç sonrasında son MP3'ün baştan çaldığını doğruladı. Hem elle
Sonraki ile seçilen hem doğal parça sonunda otomatik açılan yeni dosya yeniden
açılışta doğru hatırlandı. Radyo moduna geçiş sonrası kapat/aç da radyo modunu
ve yayını geri getirdi. Bu temel akışlar kabul edildi. Önceki klasör gezinme,
gezinirken çalmayı koruma ve klasöre göre parça sayacı testleri de doğrulanmıştı.

Sonraki kullanıcı testinde kart çıkarılarak açılışta MP3 modunda hata gösterildiği
ve radyonun başlamadığı doğrulandı. Aynı kart geri takılıp yeniden açıldığında
önceki son MP3 baştan çaldı; kayıt korunuyor. Kart mevcutken yalnız son dosyanın
silinmesi ayrı doğrulama bekliyor. Parça/mod geçişindeki flash yazısında hiç kayma olmadığı
henüz doğrulanmadı. Aşağıdaki eski sürüm test notları tarihsel kayıttır.

22 Ekim 2026 kullanıcı onayı: duraklatılmış parçaya -10 uygulayıp oynatınca
yaklaşık 10 saniye geriden devam edildi. Art arda -10 ile başlangıca dönüldüğünde
süre negatif olmadı, takılma yaşanmadı ve parça baştan çalmayı sürdürdü.
Duraklatılmış parçaya art arda +10 uygulandığında toplam süre aşılmadı;
sonun yaklaşık bir saniye önünde durdu ve +10 düğmesi pasifleşti.

## Kullanım

- MP3 dosyaları kök dizinde veya alt klasörlerde tutulabilir. Dosya gezgini kökten
  başlar; klasöre dokunarak girilir, **Üst klasör** ile geri çıkılır.
- Ana ekranın üstündeki **SD MP3** düğmesi dosya seçimini açar. Dosya seçmeden
  listeyi kapatmak mevcut kaynağı değiştirmez; radyo çalıyorsa çalmaya devam eder.
- Listeden dosyaya dokununca internet yayını kapatılır ve SD dosyası açılır.
- Yalnızca klasörlerde gezinmek çalan parçayı veya çalma listesini değiştirmez.
  Dosya seçildiğinde o klasördeki MP3 dosyaları yeni çalma listesi olur.
- Ana ekranda **SD KART**, parça bilgisi, MP3 formatı ve parça sıra/sayısı görünür.
  Başlık/sanatçı ID3 ile gelirse kullanılır; başlık yoksa dosya adı gösterilir.
- **Önceki / Sonraki** artık MP3 dosyalarını değiştirir; iki yönde liste sarılır.
- **Dosya listesi** yeniden listeyi açar. Beş dosyalık sayfalar ve **Yeniden tara** vardır.
- Ortadaki **oynat/duraklat** simgesi dosyayı duraklatır veya aynı yerden sürdürür.
  Tamamen durmuş/liste bitmişse seçili dosyayı baştan açar.
- Alt bölümdeki **-10 / +10** düğmeleri yaklaşık 10 saniye geri/ileri gider.
  Başlangıç ve son sınırları aşılmaz. Süre bilinmiyorsa düğmeler devre dışıdır.
  Duraklatılmış dosyada seçilen konuma oynatma sürdürülünce geçilir.
- Parça doğal olarak bitince sonraki dosya açılır. Son dosya bitince **Liste tamamlandı**
  gösterilir ve durur; sonsuz tekrar yapılmaz.
- Ana ekrandaki **Radyo** veya dosya listesindeki **Radyoya dön**, yerel dosyayı
  kapatıp son seçili radyo istasyonunu açar. Wi-Fi yoksa mevcut radyo bağlantı
  yönetimi devreye girer; MP3 kaynak seçimine geri dönmek mümkündür.

Ses +/-, sessize alma, azami ses sınırı ve mevcut ekolayzer ortak kullanılır.
Karartma/uyandırma ve genel ayarlar devam eder. Wi-Fi kurulumu ilk açılışta
görünüyorsa kapatılıp internetsiz SD müzik kullanılabilir.

## Sınırlar ve hata davranışı

- Yalnızca `.mp3`/`.MP3` dosyaları listelenir; farklı uzantılar gösterilmez.
  Uzantı, dosyanın gerçekten geçerli MP3 olduğunu garanti etmez.
- Klasör başına en fazla 128 MP3/alt klasör; her taranan dizinde en fazla 512 kayıt.
  Dosya yolu en fazla 240 bayt. Klasörler önce, sonra dosyalar bayt sırasıyla
  sıralanır; Türkçe sözlük sırası vaat edilmez. Boş klasöre girilebilir.
- Sınırlar aşılırsa kısmi liste sessizce kullanılmaz; hata gösterilir.
- Dosya ön kontrolünde açılmayan/boş dosya reddedilir. Çözücü açma hatası veya
  okumanın durması ana ekranda bildirilir. Bozuk dosyalar arasında sonsuz geçiş yapılmaz.
- Kart yok/okunamıyor veya MP3 bulunamadı durumu dosya ekranında gösterilir.
- Kartın sonradan takılması için, yerel dosya açık değilken tarama kartı yeniden
  açar. Kart biçimlendirilmez, karta veri yazılmaz. Önceki 1 MHz SPI/pinler korunur.
- Yerel dosya açık veya duraklatılmışken tekrar tarama SD.end/SPI.end çağırmaz.
  CSV içe aktarma SD kaynağında engellenir; önce Radyo'ya dönmek gerekir.
- Yerel oynatmada yaklaşık 2 saniyede bir salt okunur sektör kontrolü vardır;
  kart çıkarılması/okuma hatasında dosya kapatılır ve hata gösterilir. Bu
  kontrolün gerçek cihazdaki gecikme/ses etkisi henüz ölçülmedi.
- Duraklatma, ses kütüphanesinin isRunning=false değeriyle yayın hatasına
  karıştırılmaz. İlerleme kontrolü duraklatılmış dosyaya uygulanmaz.
- Wi-Fi kesilmesi/yeniden bağlanması SD dosyasını kapatmaz ve radyoyu kendiliğinden
  başlatmaz. Tarama/RF yükünün ses etkisi fiziksel test bekler.

## Kalıcılık ve web kapsamı

Son kaynak (radyo/MP3) ve son açılan MP3 yolu ayrı `playback/resume` NVS kaydında
saklanır. İlk dosya seçimi, elle parça değiştirme ve doğal parça sonunda yeni
dosya açılması kaydı günceller; mod değişimi de kaydedilir. Aynı mod/dosya tekrar
açılıyorsa değişmeyen kayıt yeniden yazılmaz. SD karta kayıt yazılmaz.
Periyodik konum kaydı yoktur; sarma, duraklatma veya durdurma kayıt oluşturmaz.
Yalnız başarısız parça/mod kaydı 10 saniye arayla yeniden denenebilir; seri logda
bildirilir. Bu, normal oynatmada periyodik kayıt yapıldığı anlamına gelmez.
Radyo modunda açılış, mevcut otomatik oynatma ve istasyon tercihlerini kullanır.
Son radyo istasyonu MP3 indeksiyle değiştirilmez; ortak ses tercihi korunur.

MP3 modunda son dosyanın klasörü yeniden okunur ve aynı dosya en baştan çalar.
Konum, süre ve duraklatma durumu saklanmaz; eski sürümdeki konum da yok sayılır.
Duraklatılmış veya liste sonunda durmuş olsa bile açılışta son dosya baştan çalar.
Kart/dosya yoksa radyo kendiliğinden başlamaz,
hata gösterilir ve önceki kayıt korunur. Kartı takıp yeniden başlatmak veya listeden
dosya seçmek mümkündür. Aynı yoldaki içerik değiştirilirse mevcut dosya baştan çalar.
Özellikle VBR dosyalarda ekrandaki süre ve ±10 saniyelik sarma tahminidir.
Tekrar/karışık çalma tercihleri oturuma aittir ve bu kayıtla kalıcılaştırılmaz.

### Konum kaydının kaldırılma nedeni — 2026-10-21

0.9.0'da görüntü yaklaşık 10 saniyede bir ve sarma düğmelerinde sağa kayıp geri
geliyordu. Bu anlar flash'a NVS konum kaydıyla örtüştü. 0.9.1-diag sürümünde
yalnız yeni devam kaydının flash yazımı kapatıldı; kullanıcı hem beklerken hem
±10 düğmelerinde kaymanın kaybolduğunu doğruladı. Sonuç, bu kaymaların yeni flash
yazılarıyla ilişkisini doğruladı; donanım düzeyinde ölçüm yapılmadı.

Kullanıcı SD'ye 10 saniyede bir kayıt seçeneğini istemedi; konumdan devam özelliğini
kaldırıp yalnız yeni parçayı ve modu saklamayı seçti. 0.9.2 bu kararı uygular.
Parça/mod değişimindeki flash yazısında kısa görüntü kayması hâlâ mümkün;
periyodik ve sarma kaynaklı kaymaların giderilmesi cihaz doğrulaması bekler.
Ekran zamanlaması, saat/pinler ve ses besleme düzeni değiştirilmedi.

Kullanıcı kapsamlı web tasarımını sonraya bıraktı. Mevcut sayfayı uyumsuz kaynak
işlemlerinden korumak için SD kaynağında web yazma/kumanda istekleri kilitlidir;
durum ve kayıtlı radyo listesi okunabilir. API `source: sd/radio` alanını verir;
sayfa kilit nedenini açıklar. SD dosya yükleme/silme, uzaktan MP3 kumandası yoktur.

İleride: web müzik yönetimi.

## Uygulama ve doğrulama

Yeni dosyalar: `SdMusicFiles.h/.cpp`; mevcut SD bağlama işlemi ortak salt okunur
yardımcıya alındı. `RadioDisplay` kaynak/dosya/oynatma kontrollerini, `main.cpp`
tek Audio nesnesinin radyo/SD geçişini ve EOF/ID3 olaylarını yönetir. Ses kütüphanesi,
I2S pinleri, NVS genel ayar biçimi ve Wi-Fi tarama/bağlantı kodu değiştirilmedi.

Kaynaklar yedeklendi: `display-test/backups/before-sd-mp3-20261020-161758`.
Kaynak/API incelemesi yapıldı. Kullanıcı kuralı gereği derleme, yükleme veya otomatik
test çalıştırılmadı; özellik henüz cihazda doğrulanmadı.

Kullanıcıya sırayla verilecek kontroller:

1. Derleme/yükleme; önceki radyo ve Wi-Fi açılışının korunması.
2. Kartın köküne veya `/music` altına en az iki MP3; SD MP3 listesi ve dosya seçimi.
3. Yerel ses, başlık, MP3 etiketi ve sayaç; ses/sessize alma.
4. Duraklat/devam et; önceki/sonraki, liste seçimi ve sayfalama.
5. Parça sonundan sonraki dosyaya geçiş; son dosya sonunda durma.
6. Radyo → MP3 → Radyo geçişleri; duraklatılmış dosyadan radyoya dönüş.
7. MP3 sırasında ekolayzer ve ekran ayarları; Wi-Fi kesilmesi/geri gelişi.
8. Kartsız/boş kart, bozuk/boş dosya ve kart çıkarma/tekrar takıp tarama.
9. Reset davranışı, radyo listesinin korunması ve SD sırasında web kilidi.
10. Uzun süreli dinleme, yüksek bit hızlı MP3, ID3/Türkçe adlar, dosya sınırları.

## 0.8.0 kullanıcı doğrulaması

- Kullanıcı derleyip yükledi; önceki Wi-Fi/radyo açılışı ve SD MP3 düğmesi göründü.
- İlk bulunamadı bildiriminin ardından açılış kaydı kartın okunduğunu ve kökteki
  müziklerin .m4a/.webm olduğunu gösterdi. Tarama kodu değiştirilmedi. Gerçek MP3
  konunca dosyalar listelendi; diğer biçimler bu sürümün kapsamına alınmadı.
- Dosya seçimiyle radyo durdu, MP3 çaldı; SD KART ve parça adı göründü.
- Tek düğmede duraklat/devam; başa dönmeden sürdürme doğrulandı.
- Ses +/-, sessize alma/açma ve önceki/sonraki parça; ad/sayaç uyumu doğrulandı.
- Dosya listesinden farklı seçim çalıştı.
- Doğal parça bitiminde sonraki dosya başladı; son dosya sonunda Liste tamamlandı
  mesajıyla durdu. Radyo düğmesi son istasyonu açtı ve radyo kontrolleri geri geldi.
- Kart çıkarma, sınırlar, bozuk dosya, sayfalama, offline/uzun süre ve ID3 kodlama
  çeşitleri henüz ayrı ayrı doğrulanmış sayılmıyor.

## 0.8.1 — süre, tekrar ve karışık çalma

Kullanıcı mevcut MP3 ekranına süre, tek düğmeli oynat/duraklat ve üç mod düğmesi istedi.

- Oynat/duraklat tek düğmedir; simgeye Oynat/Duraklat yazısı eklendi.
- Yalnızca SD modunda Tür satırı `Süre: 00:00/03:24` biçiminde geçen/toplam
  süreyi gösterir. Toplam henüz bilinmiyorsa `--:--`. MP3 etiketi yerinde kalır.
  Süre ses çözücüsünden alınır; 250 ms aralıkla kontrol edilir, saniye değişince
  ekran güncellenir. Duraklatıldığında sabit kalır. Son durmada değerler korunur.
- Ses kütüphanesinin MP3 toplam süre formülündeki erken tam sayı bölmesi düzeltildi;
  önce 64 bit ile 8 çarpımı, sonra bölme yapılır. VBR süre hâlâ ortalama bit hızına
  bağlı bir tahmindir; kesin Xing/VBR indeks okuması eklenmedi.
- SD KART başlığının sağında **Tekrar: 1**, **Tekrar: tüm**, **Karışık** bulunur.
  Etkin düğme turuncu. Tek parça ve tüm liste tekrarı birbirini dışlar; aynı
  düğmeye yeniden basmak tekrarı kapatır. Karışık ayrı açılıp kapatılır.
- Tekrar: 1 doğal EOF'ta aynı parçayı baştan açar. Kullanıcının Önceki/Sonraki
  komutunu engellemez. Diğer modlara göre otomatik geçişte önceliklidir.
- Tekrar: tüm son parçadan ilk parçaya döner; kapalıysa liste sonunda durur.
- Karışık modda tek tek rastgele seçim yerine her parça bir kez bulunan sıra
  oluşturulur. Mevcut parça devam eder; sonraki parçalar bu sıradan seçilir.
  Doğal oynatmada bir tur içinde tekrar yoktur (Tekrar: 1 hariç).
- Karışık + tüm liste tekrarı yeni turda yeni sıra oluşturur; birden fazla
  dosyada son çalan parçayı yeni turun ilk parçası olarak seçmez.
- Önceki/Sonraki aynı karışık sıra içinde ilerler; elle sıra sınırını aşma mümkündür.
  Dosya listesinden doğrudan seçim veya yeniden tarama yeni sıra kurar.
- Başlangıçta tekrar ve karışık kapalıdır; seçimler aynı çalışma oturumunda korunur,
  reset sonrasına yazılmaz. Radyo modunda bu kontroller gizlenir ve Tür geri gelir.

Yedek: `display-test/backups/before-mp3-controls-20261020-192849`.
Derleme/yükleme ve yeni özelliklerin cihaz testi kullanıcıdan bekleniyor.
Sonraki testler sırayla: yerleşim/süre, duraklamada sayaç, tek tekrar, liste tekrarı,
karışık tur, karışık+liste tekrarı ve radyo görünümüne dönüş.

## 0.8.2 — kayan yazıyla ilişkili ses kesintisi, 2026-10-21

Kullanıcı 0.8.1'i yükledikten sonra MP3 sesinin kayan parça adıyla senkron
kesildiğini bildirdi. Bu, kabul edilmiş parlaklık parazitinden ayrı bir hata;
normal dinlemede kabul edilen durum sayılmadı. Yeni tekrar/süre işlevlerinin
tamamı henüz tek tek doğrulanmış değil.

Kaynakta yerel oynatmanın tek audio.loop çağrısında 16 KB'a kadar SD okuduğu
görüldü. 1 MHz SPI'da yalnızca bu verinin aktarımı yaklaşık 131 ms; dosya sistemi
ve çizim buna ekleniyor. Bu tespit ölçülmüş toplam gecikme veya tek başına kesin
kök neden kanıtı değildir; uzun bloklama kaynağını azaltmak için hedefli düzenleme yapıldı.

- Sadece yerel MP3 okuması çağrı başına 2 KB ile sınırlandı (ham aktarım yaklaşık
  16 ms). Giriş tamponunun toplam kapasitesi azaltılmadı.
- LVGL çizimi döndükten hemen sonra yerel MP3 için bir ek audio.loop çağrısı var;
  web/diğer bakım işleri bundan sonra yürür.
- Kayan yazı, fontlar, panel zamanlaması, SPI hızı, I2S ve diğer formatlar korunur.
- Yedek: `display-test/backups/before-mp3-scroll-audio-fix-20261021-141216`.
- Derleme/yükleme/test yapılmadı. Kullanıcı aynı uzun başlıklı dosyada kesintiyi
  yeniden kontrol edecek; başarılı düzeltme olarak henüz kaydedilmedi.

## 0.8.3 — sabit MP3 parça adı, 2026-10-21

Kullanıcı 0.8.2'de kesintinin sürdüğünü, kısa başlıklarda sorun olmadığını ve
kayan uzun başlıklı dosyaların tamamında kesinti oluştuğunu bildirdi. Kullanıcı
kaydırma yerine yalnızca ekrana sığan bölümün gösterilmesini istedi.

- SD MP3 modunda başlık `LV_LABEL_LONG_MODE_DOTS`: tek satır, sabit yazı,
  sığmayan bölüm üç noktayla kısaltılır. Orijinal başlık verisi değiştirilmez.
- Kaynak değişiminde radyo görünümünün önceki başlık davranışı geri yüklenir.
- Sonuç vermeyen 0.8.2 okuma-boyutu ve ek ses çağrısı denemeleri geri alındı;
  süre düzeltmesi ve yeni MP3 kontrolleri korundu.
- Yedek: `display-test/backups/before-static-mp3-title-20261021-141927`.
- Derleme/yükleme yapılmadı; aynı uzun başlıklı MP3 ile kullanıcı testi bekleniyor.

### Çözüm doğrulaması — 2026-10-21

Kullanıcı 0.8.3'ü kendi derleyip yükledikten sonra “OK sorun çözüldü” bildirdi.
Uzun MP3 başlığının kaydırılması yerine sabit ve üç noktayla kısaltılmış gösterimi
bu sorun için kabul edilen çözümdür.

- Belirti: uzun başlığı kayan MP3 dosyalarında başlık hareketiyle eşzamanlı ses
  kesintisi; kısa, kaymayan başlıklı dosyalarda sorun yok. Bu karşılaştırma kullanıcı
  tarafından bildirildi; düşük seviyede CPU/DMA gecikme ölçümü yapılmadı.
- Başarısız deneme: 0.8.2'de yerel okuma parçasını 16 KB'dan 2 KB'a düşürmek ve
  çizim sonrası ek audio.loop çağrısı kesintiyi gidermedi. 0.8.3'te ikisi de geri alındı.
- Kabul edilen değişiklik: yalnızca SD MP3 başlığında LV_LABEL_LONG_MODE_DOTS;
  kaydırma yok, tek satırda sığan bölüm ve üç nokta var. Tam başlık verisi korunuyor.
- Doğrulanan sonuç: kullanıcı aynı sorun için kesintinin giderildiğini onayladı.
  Radyo kaynağının önceki başlık davranışı korunuyor; radyo kaydırmasının ayrı
  bir performans testi yapıldığı iddia edilmiyor.
- Bu hata kapatıldı; kabul edilen parlaklık değiştirme parazitiyle karıştırılmaz.
  Süre/tekrar/karışık modların henüz yapılmamış ayrı testleri bu onayla tamamlanmış sayılmaz.

### Süre, tekrar ve karışık çalma testleri — 2026-10-21

Kullanıcı “MP3 süre, tekrar ve karışık çalma testlerini ben yaptım kaydedebiliriz”
bildirimiyle bu test grubunun tamamlandığını onayladı. Güncel çalışma noktası 0.8.3.

- MP3 süre gösterimi test edildi ve kullanıcı tarafından kabul edildi.
- Tek parça ve tüm liste tekrarı test grubu tamamlandı olarak kaydedildi.
- Karışık çalma test grubu tamamlandı olarak kaydedildi.
- Ayrıntılı dosya/format, tur sayısı veya tek tek tuş adımlarına ilişkin ek test
  kaydı verilmedi; bu ayrıntılar ayrıca denenmiş gibi raporlanmadı.
- Kart hataları, sınır/yük testleri, uzun süreli çalışma ve diğer kaynakların
  testleri bu bildirim kapsamında tamamlanmış sayılmaz.
