# Wi-Fi kurulumu — 0.7.0

## Kullanım

İlk açılışta ayrı Wi-Fi kaydı yoksa dokunmatik kurulum ekranı otomatik açılır.
Ağ taraması başlar. Açılır listeden ağ seçilir, ekran klavyesiyle şifre yazılır ve
**Bağlan ve kaydet** seçilir. Gizli ağ veya taramada görünmeyen ağ için SSID elle
yazılabilir. İlk liste seçeneği yer tutucudur; ilk bulunan ağ da açıkça seçilebilir.

Yalnızca 2,4 GHz ağlar hedeflenir. Bu sürüm tek kayıtlı ağ profili, açık ağ ve
kişisel şifreli ağ girişini sunar. Kurumsal kullanıcı adı/sertifika girişi ve
çoklu profil yöneticisi yoktur. Liste en fazla 32 farklı görünür SSID gösterir;
aynı SSID'li erişim noktaları tek satırda birleştirilir.

SSID en fazla 32 bayt. Şifre açık ağda boş; kişisel şifreli ağda 8–63 bayt
veya 64 onaltılık hane olabilir. Şifre alanı maskelidir; kayıtlı şifre forma
geri doldurulmaz. Ağ değiştirilirken şifre yeniden girilir. Şifrenin başındaki
ve sonundaki boşluklar otomatik kırpılmaz.

Sonradan **Ayarlar → Cihaz → Wi-Fi ağını ayarla / değiştir** ile aynı ekran açılır.
**Kapat / denemeyi iptal et** süren yeni bağlantı denemesini iptal eder; önceden
kayıtlı ağ varsa ona döner. Başarıyla kaydedilmiş yeni bir ağı geri almaz.
İlk kurulum kapatılırsa menüler internetsiz kullanılabilir; aynı yoldan tekrar açılır.

Başarılı bağlantıdan sonra kurulum ekranında başarı mesajı görünür. Kapat ile
radyoya dönülür. Açılışta otomatik yayın açıksa ağ hazır olunca radyo başlar;
kapalıysa kullanıcı seçimi beklenir. Ağ değişikliğinden önce yayın isteniyorsa
bağlantı sonrasında yeniden açılır; kullanıcı yayını durdurmuşsa başlatılmaz.

## Kayıt ve hata davranışı

- Wi-Fi bilgileri `radioWifi/network` isimli tek sürümlü NVS bloğunda saklanır.
  İstasyon listesine, genel ayarlara ve ekolayzer kaydına dokunulmaz.
- Yeni bilgiler deneme boyunca RAM'de tutulur; eski NVS kaydı korunur.
- Eski bağlantı kesilmeden yeni denemeye başlanmaz. Böylece eski `WL_CONNECTED`
  durumu yeni SSID/şifre başarılı olmuş gibi yorumlanmaz.
- Yeni ağdan IP alınıp doğru SSID'ye bağlanıldığında NVS kaydı yapılır. Bu, Wi-Fi
  bağlantısını doğrular; internet erişimi veya yayın sunucusuna erişim garantisi değildir.
- 30 saniye içinde bağlantı kurulmazsa yeni bilgiler kaydedilmez; eski profil
  varsa geri bağlantı denenir. Kalıcı kayıt hatasında da eski profil korunur.
- Kayıtlı ağa açılışta bağlanılamazsa kurulum ekranı açılır. Hatalı kaydı otomatik
  silme veya flash silme yapılmaz. Kullanıcı yeni bilgi girebilir.
- Sonradan bağlantı koparsa kayıtlı ağ yeniden denenir. Deneme başarısız olursa
  kurulum açılır. Kurulum açıkken arka planda yeni kayıtlı-ağ denemesi başlatılmaz;
  devam eden geri dönüş denemesi 30 saniye içinde sonuçlanır.
- Tarama ve bağlantı bekleyişi ana döngüde durum makinesiyle yürür; 30 saniye
  bekleyen while döngüsü yoktur. Ekran ve isteğe bağlı SD liste akışı kullanılabilir.
- Ağ değiştirme mevcut yayını keser. RF taramasının ses üzerindeki etkisi ayrıca
  cihazda gözlenmeli; kesintisiz tarama başarısı henüz iddia edilmiyor.
- Wi-Fi ekranı açıkken web yazma/kumanda kilidi de devrededir. Sayfa okunabilir;
  ağ/IP değiştiğinde yeni IP'den tekrar açılması gerekebilir.

## Şifrelerin ele alınması

Sabit SSID/şifre tanımları `main.cpp` içinden kaldırıldı. Form yalnızca kullanıcının
yazdığı yeni şifreyi taşır; kayıtlı şifreyi göstermez. Deneme kabul edilince ve
form kapatılınca alan temizlenir. Komut kuyruğundan alınan şifre kopyası kuyrukta
tutulmaz. Seri çıktı ve web durumuna şifre eklenmedi. Standart NVS saklama kullanılır;
bu sürüm ayrıca şifreli NVS yapılandırması yapmaz.

İlk 0.7.0 yüklemesinde önceki kaynak kodundaki ağ bilgileri otomatik taşınmaz:
**bir kez ekrandan kurulum gerekir.** Sonraki açılışlarda kayıtlı ağ kullanılır.
Önceki kaynak dosyalarının yedeği kullanıcı talebi doğrultusunda korunmuştur.

## Altyapı

- `RadioWifi`: ayrı sürümlü NVS profili, asenkron tarama, deneme/kayıt/geri dönüş.
- `RadioDisplay`: tembel oluşturulan kurulum paneli, ağ listesi, SSID/şifre alanları,
  ekran klavyesi ve durum mesajı. LVGL'nin mevcut klavye/textarea desteği kullanıldı.
- `main.cpp`: ortak komutlar, ağ durumuna göre ses bağlantısını duraklatma/yeniden
  açma, ağ hazır olduğunda mevcut web sunucusunu başlatma.
- I2S pinleri, ses kütüphanesi ve genel ayar blob biçimi değiştirilmedi.
- Telefonla kurulum için geçici erişim noktası/captive portal veya webden ağ
  değiştirme bu adımda eklenmedi; cihaz ilk kurulumda dokunmatik ekranını kullanır.

## Doğrulama ve sonraki adımlar

Kullanıcı kuralı gereği derleme, yükleme ve otomatik test çalıştırılmadı.
Kaynak ve kullanılan yerel Arduino/LVGL API bildirimleri incelendi.

Sırayla kullanıcıya verilecek cihaz testleri:

1. Derleme/yükleme sonrası ilk kurulum ekranı ve ağ listesinin açılması.
2. Ağ seçimi, maskeli şifre klavyesi, başarılı bağlantı ve yayın davranışı.
3. Reset sonrası şifre sormadan otomatik bağlantı; liste ve diğer ayarların korunması.
4. Sonradan kurulum açma/kapatma; yayın ve web kilidi davranışı.
5. Yanlış şifreyle deneme; eski profilin korunması ve eski ağa dönüş.
6. Deneme sırasında iptal; ilk kayıtsız kurulumdan çıkış ve tekrar giriş.
7. Elle SSID, açık ağ, ulaşılamayan ağ ve açılış bağlantı zaman aşımı.
8. Gerçekten farklı ağa geçiş; yeni IP'den web, yeni kaydın resetle kalıcılığı.
9. Yayın durdurulmuşken ağ değişimi; otomatik yayın kapalı açılış.
10. Tarama/bağlantı esnasında dokunma ve bellek davranışı.

Kaynak yedeği: `display-test/backups/before-wifi-setup-20261020-141329`.
Önceki kabul edilen çalışma noktası 0.6.0 temel web işlevleridir.

## 0.7.1 — ilk cihaz geri bildirimi

Kullanıcı 0.7.0'da ağ bulunamadı mesajı ve titreyen/tepki vermeyen boş açılır
liste bildirdi. İlk Wi-Fi bağlantısı henüz doğrulanmadı.

- Negatif tarama sonucu eskiden sıfır ağ gibi gösterilebiliyordu. Artık başarısız
  tarama ayrı mesaj verir; bir kez, 500 ms arayla yeniden denenir.
- Önceki 120 ms/kanal (Arduino'da 2,4 saniye zaman aşımı) ayarı kaldırıldı;
  varsayılan 300 ms/kanal ve 6 saniyelik Arduino zaman aşımı kullanılır.
- Başlangıç/bağlantı kesme olaylarına alan bırakmak için tarama 250 ms sonra
  ana döngüden başlar; bu süre boyunca bloklayan bekleme yoktur.
- Her döngüde UI kontrol durumlarını yeniden uygulamak yerine yalnızca durum
  değişince uygulanır. Bunun fiziksel titremeyi giderip gidermediği test bekliyor.
- Liste boşken neden pasif olduğu açılır alanın kendi metninde de belirtilir.
- Seri tanılama yalnızca sayısal tarama kodunu, deneme sayısını ve ağ adedini yazar;
  SSID, BSSID veya şifre yazmaz.
- Yedek: `display-test/backups/before-wifi-scan-fix-20261020-153737`.
- Derleme/yükleme ve cihaz doğrulaması kullanıcı tarafından yapılacak.

## 0.7.2 — tanılama, çözüm henüz doğrulanmadı

Kullanıcı 0.7.1'de iki taramada da başlangıç -1, ardından hata -2 bildirdi.
Bu kayıt tek başına tarama süresini veya sürücü tamamlanma olayını göstermiyor;
kesin kök neden çıkarılmadı. 0.7.1 başarılı düzeltme kabul edilmedi.

- İlk kayıtsız kullanım için `isKey` kontrolü eklendi; beklenen eksik NVS kaydı
  getBytesLength hata çıktısı üretmez.
- Sonuç vermeyen 250 ms başlangıç ertelemesi ve otomatik ikinci tarama kaldırıldı.
  Varsayılan Arduino tarama süresi korundu. Tara yeni bir deneme başlatır.
- Sayısal tanılama: tarama süresi, STA modu/durumu, boş heap, sürücüdeki sonuç adedi
  ve sürücünün SCAN_DONE durum/sonuç/kimlik olayı. SSID/şifre yazılmaz.
- SCAN_DONE callback'i yalnızca seri tanılama yapar; LVGL/Audio nesnesine erişmez.
- Bu sürüm esas olarak tanılama içindir. Derleme, yükleme ve log cihazda kullanıcıdan beklenir.
- Yedek: `display-test/backups/before-wifi-scan-diagnostics-20261020-154916`.

## 0.7.3 — ekran dışındaki klavye

Kullanıcı 0.7.2'de ağların bulunup listelendiğini ve ağ seçebildiğini bildirdi.
Önceki tarama başarısızlığının kesin nedeni bu geri bildirimle kanıtlanmadı;
yalnızca bu denemede tarama/seçim çalıştı. Şifre alanına dokununca klavye görünmedi.

Yerel LVGL kaynağı klavyenin kurucuda BOTTOM_MID ile hizalandığını gösteriyor.
Ardından kullanılan set_pos(0,320), üstten 320 yerine alttan +320 kaydırmış ve
klavyeyi ekran dışına taşımış. Konumlandırma açıkça BOTTOM_MID, 0,0 olarak düzeltildi;
800×160 klavye 800×480 panelin alt bölümünü kullanacak. Şifre alanına hedefleme
ve Wi-Fi tarama/bağlantı kodu değiştirilmedi.

Yedek: `display-test/backups/before-wifi-keyboard-fix-20261020-160750`.
Derleme/yükleme ve klavye/bağlantı doğrulaması kullanıcı tarafından yapılacak.

## 0.7.3 kullanıcı doğrulaması — 2026-10-20

- Kullanıcı derleyip yükledi; klavye düzeltmesi için “OK sorun yok” bildirdi.
- Ağ seçimi ve şifre girişinden sonra “Bağlandı ve kaydedildi” mesajı doğrulandı.
- Kurulum kapatıldığında radyo çaldı.
- Reset sonrasında şifre sormadan bağlantı ve kayıtlı açılış davranışı doğrulandı.
- Ayarlar → Cihaz üzerinden kurulum açılıp değişiklik yapılmadan kapatıldı;
  kullanıcı yayının devam ettiğini doğruladı.
- Aynı ağ için bilerek yanlış şifreyle deneme yapıldı; yaklaşık 30 saniye sonrası
  başarısızlık, eski ağa dönüş ve radyo yayınının yeniden başlaması doğrulandı.
- Bu başarısız denemeden sonra resetle eski doğru bilgilerin korunduğu ve radyo
  çaldığı doğrulandı; yanlış şifre kalıcı kayda geçmedi.
- Yeni yanlış şifre denemesi beklenmeden Kapat ile iptal edildi; eski ağa dönüş
  ve radyonun yeniden çalması doğrulandı.

0.7.3 temel Wi-Fi kurulum/değiştirme ve geri dönüş akışları için kabul edilen
çalışma noktasıdır. Gerçekten farklı ağ/IP'ye geçiş, elle/gizli SSID, açık ağ,
kayıtlı ağ ulaşılamazken açılış, otomatik yayın kapalı durum, web kilidi ve yük/
bellek davranışlarının özel testleri henüz tamamlanmadı. Yeni ağa geçiş doğrulandı
diye raporlanmamalı; test edilen hatalı denemeler aynı SSID üzerinde yapıldı.
