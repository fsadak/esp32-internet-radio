# Web arayüzü — 0.6.0

## Kapsam

Telefon/bilgisayar cihazla aynı yerel ağdayken, Ayarlar → Cihaz bölümünde görünen
`http://<cihazın-IP-adresi>/` adresi açılır. Siyah-turuncu sayfa telefon ekranına
uyumludur; liste tablosu küçük ekranda yatay kaydırılabilir. Harici CDN, font,
internet hizmeti veya dosya sistemi yüklemesi gerektirmez; sayfa firmware içindedir.

### Kumanda — uygulandı, cihaz testi bekliyor

- İstasyon, ham parça başlığı, yayın durumu, tür, format, ses ve sessiz durumu.
- Önceki/sonraki, kayıtlı listeden seçim, ses eksi/artı, sessize alma/açma.
- Yayını başlatma ve durdurma. Durdurma bağlantıyı ve otomatik tekrar denemeyi kapatır;
  sonraki açılışın otomatik yayın tercihini değiştirmez.
- Yaklaşık 2 saniyede bir cihaz durumunu okuma; yoğun istek sırasında yeni anket açmama.
- Dokunmatik ve web aynı `processCommand` işlevini kullanır. HTTP istekleri ana
  döngüde işlenir; farklı görevden Audio veya LVGL nesnesine erişilmez.
- Cihazda ayarlar açıkken web yazma/komut istekleri 409 ile reddedilir. Ekrandaki
  taslak ile uzaktan değişiklikler birbirinin üzerine yazılmaz; okumalar sürer.

### İstasyon yönetimi — uygulandı, cihaz testi bekliyor

- Ad/URL alanlarını düzenleme, ekleme, silme ve yukarı/aşağı sıralama.
- Mevcut ortak ayrıştırıcıyla UTF-8 `name;url` CSV doğrulama; aynı 32 istasyon,
  12 KB ve alan sınırları. Dosya önce doğrulanır ve web taslağına alınır.
- Cihaza kaydetmeden radyo listesi değişmez. Geri al cihazdaki kayıtlı listeyi getirir.
- CSV indir **cihazda kayıtlı** listeyi UTF-8 BOM ile indirir; kaydedilmemiş taslağı içermez.
- Sayfa kapatılırken kaydedilmemiş değişiklikler için tarayıcı uyarısı istenir.
- Liste değiştirilecekse alınan katalog kimliğiyle karşılaştırılır. Başka tarayıcıdan
  veya SD'den değişmiş listeye eski taslak yazılmaz; güncel listeyi alma gerekir.
- Yeni listede seçili istasyon aynı URL ile varsa indeksi yeniden eşleştirilir.
  Açık yayın bağlantısı korunur; ad değişikliği ekrana yansır.
- Çalması istenen istasyon çıkarıldıysa ayrıca onay gerekir; kayıttan sonra yayın
  durdurulur ve kullanıcı seçimi beklenir. Yayın zaten kapalıysa liste kaydı başlatmaz.
- Sabit açılış istasyonu da URL ile eşleştirilir; çıkarılmışsa ilk istasyona döner.
- Ana liste kaydı başarısızsa eski liste korunur. Liste kaydedilip ikincil açılış
  tercihleri kaydedilemezse başarı yanıtında kısmi kayıt durumu açıkça bildirilir.
- Tamamen aynı liste yeniden gönderilirse gereksiz NVS yazımı yapılmaz.

## HTTP altyapısı

Yerel Arduino WiFiServer/WiFiClient kullanılır; yeni kütüphane bağımlılığı eklenmedi.
Tek etkin bağlantı, döngü başına en fazla 768 bayt okuma/yazma, 2048 bayt başlık,
12 KB gövde sınırı, 3 saniye hareketsizlik ve 15 saniye toplam aktarım sınırı vardır.
Yanıt yazımı `send(..., MSG_DONTWAIT)` ile parçalara bölünür. Böylece yavaş bir
tarayıcı için bekleyen okuma/yazma döngüsü kurulmaz. JSON/CSV doğrulama ve NVS
kaydı sınırlı boyutlarda eşzamanlıdır; bunların ses üzerindeki gerçek etkisi cihazda
denenmelidir. İstasyon bağlantısı açma, mevcut radyodaki bağlantı işlevini kullanır.

| Yol | Yöntem | İşlem |
|---|---|---|
| `/` | GET | Firmware içindeki sayfa |
| `/api/state` | GET | Ortak radyo durumu, liste kimliği, CSRF belirteci |
| `/api/stations` | GET | Kayıtlı kanonik CSV ve liste kimliği |
| `/api/validate` | POST | Gövde CSV; doğrular ve kanonik CSV döndürür, kayıt yapmaz |
| `/api/stations` | POST | Gövde CSV; kimlik karşılaştırmasıyla kalıcı liste değişimi |
| `/api/command` | POST | Düz metin kumanda komutu |

Komutlar: `volumeDown`, `volumeUp`, `mute`, `previous`, `next`, `play`, `stop`,
`select:<sıfır-tabanlı-indeks>`. Seçim ve liste yazımı `X-Radio-Revision` ister.
Tüm POST istekleri `X-Radio-Token` ister; çalan istasyonu çıkarma onayı
`X-Radio-Confirm: stop` başlığıdır. JSON metinleri kaçışlanır; yayın verileri ve
istasyon adları tarayıcıda innerHTML yerine textContent/value ile gösterilir.

Bu sürüm yerel ağ kullanımına yöneliktir; kullanıcı oturumu/şifre ve TLS yoktur.
Rastgele CSRF belirteci, IP tabanlı Host/Origin kontrolü ve frame engelleme bulunur;
bunlar aynı ağa erişebilen kişilere karşı bir kullanıcı yetkilendirmesi değildir.
İnternete açma veya port yönlendirme bu sürümün kapsamı değildir.

## Sonraki aşama için altyapı ve yapılacaklar

Hedef yalnızca istasyon düzenlemek değil, cihazın normal ekran işlevlerinin web
karşılıklarını da sağlamaktır. Yeni kontroller ortak komut/ayar akışına bağlanacak.

- Bas/orta/tiz; Düz; taslak önizleme, Kaydet ve Vazgeç.
- Parlaklık, karartma süresi, gelecekteki ekran kapatma/uyandırma.
- Azami ses, açılış sesi, son ses seviyesini hatırlama.
- Otomatik yayın, son istasyon ve sabit açılış istasyonu.
- Cihaz bilgileri ve gerçek erişim adresi için QR.
- Web/ekran ayar düzenlemesi için açık sahiplik ve süre aşımı; sayfa kapanırsa
  önizleme taslağının geri alınması. İlk sürümde yerel ayarlar açıkken yazma kilidi var.
- SD karttaki CSV dosyalarını uzaktan seçme/yükleme; ileride SD müzik kontrolü.
- İstasyon listesine ekleme/birleştirme. İlk sürüm tüm listeyi değiştirir.

## Doğrulama durumu

Kaynak incelemesi yapıldı; kullanıcı kuralı gereği derleme, yükleme ve otomatik
test çalıştırılmadı. Cihaz testi aşağıdaki sırayla, kullanıcı OK bildirimleriyle yapılacak:

1. Firmware derleme/yükleme; radyo çalarken Cihaz sekmesindeki adresten sayfanın açılması.
2. Ses eksi/artı ve sessize alma; cihaz ekranıyla iki yönlü durum uyumu.
3. Önceki/sonraki, doğrudan seçim; durdur/başlat.
4. Liste taslağını düzenleme ve geri alma; bu sırada cihaz listesinin değişmemesi.
5. Kayıtlı CSV indirme; dosya doğrulama; hatalı dosyada liste koruma.
6. Çalan URL korunarak sıralama/ad değişimi; kesinti ve yeni indeks kontrolü.
7. Çalan istasyonu çıkarma; önce onaydan vazgeçme, sonra onayla durdurma.
8. İki tarayıcı veya SD değişimiyle eski taslak koruması; yerel ayar kilidi.
9. Kaydet/reset kalıcılığı, açılış istasyonu eşleştirmesi.
10. Büyük dosya ve birden fazla istemcide ses/dokunma akıcılığı.

Önceki çalışma noktası: 0.5.0. Yedek:
`display-test/backups/before-web-interface-20261020-134048`.

### Kullanıcı tarafından doğrulananlar — 2026-10-20

- Kullanıcı derleyip yükledi; yerel IP'den sayfa açıldı, istasyon ve ses doğru
  göründü; radyo çalmaya devam etti.
- Web ses +/-, sessize alma/açma ve cihaz ekranıyla durum uyumu doğrulandı.
  Cihazdan ses değişimi webde yaklaşık iki saniye içinde göründü.
- Sonraki, seçim kutusundan istasyon seçme ve durdur/aynı yayını başlatma doğrulandı.
  Web Önceki düğmesi ayrıca tek başına test ettirilmedi.
- Kaydedilmemiş ad değişikliği cihazı etkilemedi; Geri al eski adı getirdi.
- Kayıtlı CSV indirildi; başlığı ve istasyonları doğrulandı. Aynı dosya doğrulanıp
  taslağa alındı; bu aşamada cihaz kaydı değiştirilmedi.
- Çalan URL korunarak ad değiştirme, adı geri getirme ve bir sıra taşıyıp kaydetme:
  yayın devam etti, ad/indeks cihazla eşleşti.
- Çalan istasyonu çıkarma uyarısından vazgeçilince yayın ve cihaz listesi korundu.
  İşlem onaylandığında yayın durdu ve cihaz seçim bekledi.
- İndirilen CSV ile liste geri yüklendi; silinen istasyon döndü, yayın kendiliğinden başlamadı.
- Cihaz ayarları açıkken web kumanda düğmeleri pasifleşti; açılır pencere çıkmadı.
  Bu beklenen davranış. Vazgeç ile menü kapatıldığında düğmeler aktifleşti ve ses + çalıştı.
- Reset sonrasında webden kaydedilmiş liste ve kayıtlı açılış davranışı korundu.
- İki sekmede birinci sekme listeyi değiştirdikten sonra ikincinin eski taslağı 409
  mesajıyla reddedildi. Test adı geri alınıp kaydedildi; ikinci sekmede Geri al ile
  aynı düzeltilmiş liste görüldü.

- Boş URL içeren test CSV'si webde satır 2 hatasıyla reddedildi; kullanıcı mevcut
  cihaz listesinin korunduğunu ve yayının kesilmeden sürdüğünü doğruladı.
- Web Önceki düğmesi önceki istasyonu çaldı; cihaz ve web bilgileri eşleşti.
- Temel web kullanım akışları kullanıcı tarafından doğrulandı; 0.6.0 bu kapsamda kabul edildi.

Henüz tamamlanmayanlar: SD değişimiyle eski web taslağı, sabit açılış istasyonu URL eşleştirmesinin özel
silme/sıralama durumları, 12 KB sınırı ve eşzamanlı istemci yükünde ses akıcılığı.
İki sekme testi çok istemcili yük/uzun süreli çalışma testi olarak kabul edilmedi.
