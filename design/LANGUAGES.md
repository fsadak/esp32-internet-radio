# Cihaz dili — 0.10.0

## Kararlaştırılan davranış

- Desteklenen diller: **English** ve **Türkçe**. Kaydedilmiş tercih yoksa
  varsayılan **English**; eski sürümden geçişte de dil tercihi bulunmadığından
  İngilizce kullanılır. Diğer kayıtlı ayarlar korunur.
- Menü: **Settings → Device → Dil / Language** veya
  **Ayarlar → Cihaz → Dil / Language**. Seçenekler her dilde kendi adlarıyla
  `English` ve `Türkçe` görünür; sıra sabittir (0=English, 1=Türkçe).
- Seçim taslak ayardır. **Save/Kaydet** başarılı olunca dil uygulanır ve kalıcı
  olur; **Cancel/Vazgeç** eski dili korur. Kayıt başarısızsa dil değişmez.
- Yeniden başlatma yapılmaz. Çeviri için ekran nesneleri yeniden oluşturulmaz;
  mevcut metinler güncellenir. Ses dosyası/yayın kapatılıp açılmaz.
- Dil başına periyodik NVS yazımı yoktur; mevcut ayar kaydetme işlemi kullanılır.

## Kapsam

Ana ekran, ayar sekmeleri, ekolayzer, istasyon ve SD dosya gezgini, CSV yükleme
onayları/hataları, Wi-Fi kurulumu ve cihazın ürettiği durum/hata mesajları çevrilir.
Dinamik sayılar, sayfa sayaçları, tekrar deneme ve CSV satır hataları için merkezi
tablodaki bütün mesaj şablonları kullanılır.

İstasyon, dosya/klasör, şarkı, sanatçı, yayın türü, SSID ve adresler kullanıcı/yayıncı
verisidir; çevrilmez. Ana ekranda tür/süre etiketleri çevrilir, türün yayıncıdan
gelen değeri aynen kalır. Şarkı alanındaki cihaz mesajları ayrı işaretlenir;
aynı kelimeleri içeren gerçek metadata çeviri anahtarı sayılmaz.

MP3 başlığı sabit ve sığmayan kısmı üç noktalıdır; önceki ses kesilmesi nedeniyle
kapatılmış olan kaydırma yeniden etkinleştirilmez. Dil kaydı için ses pinleri,
ekran zamanlamaları veya SD okuma düzeni değiştirilmedi.

**Web arayüzü ve seri tanılama loglarının çevirisi bu aşamada kapsam dışıdır.**
Web dil desteği kapsamlı web tasarımına bırakıldı. GitHub yayın hazırlığı,
kullanıcının isteğiyle dil desteği tamamlanana kadar beklemededir; yayın yapılmadı.

## Uygulama ve kayıt uyumluluğu

- `RadioLanguage.h/.cpp`: etkin dil ve merkezi Türkçe/İngilizce mesaj tablosu.
  Kanonik anahtarlar mevcut Türkçe cihaz metinleridir. Kullanıcı verisinde sözcük
  değiştirme yapılmaz; tanımlı bütün mesaj/şablonlar eşleştirilir.
- `RadioDisplay.cpp`: yalnız cihaz metinleri kaynak anahtarıyla etiketlere bağlanır.
  Dil değişince bu etiketler tek geçişte güncellenir; aynı mesajın tekrar çizilmesi
  yeniden çeviri/yerleşim işlemi oluşturmaz. Etiket silinince bağ serbest bırakılır.
  Metadata satırları ve giriş alanları çeviri bağı taşımaz.
- Cihaz sekmesindeki mevcut bilgi alanı dil seçimine yer açacak şekilde yeniden
  yerleştirildi; Wi-Fi düğmesi korunur. Liste açılır kutularının seçenekleri de
  güncellenir; istasyon ve ağ adları çevrilmez.
- `RadioSettings` dil alanı içerir; genel ayar blob sürümü **3** oldu. Sürüm 1 ve
  2 kayıtları eski açık yapı tanımlarıyla okunur. Sürüm 2'nin dolgu baytı dil olarak
  yorumlanmaz. Parlaklık, karartma, ses, ekolayzer ve açılış tercihleri taşınır;
  geçersiz dil English'e döner. Mevcut katalog kimliği kontrolü sürer.
- Wi-Fi kimlik bilgisi, istasyon listesi ve son kaynak/parça NVS kayıtları değişmez.

## Doğrulama durumu — 2026-10-22

Dosyalar incelendi ve yedeklendi:
`display-test/backups/before-en-tr-language-20261022-015136`.
Kaynak üzerinden mesaj kapsamı, metadata ayrımı ve kayıt geçiş akışı incelendi.
Kullanıcı kuralı gereği **derleme, yükleme ve otomatik test çalıştırılmadı**.
Yeniden başlatmadan uygulama ve sesin kesintisiz sürmesi cihaz doğrulaması bekler.

Kullanıcı derleyip yükledikten sonra testler tek tek verilecek:

1. Eski ayarlar korunarak İngilizce açılış ve Device altındaki dil seçimi.
2. Türkçe seçip Save: mevcut radyo/MP3 sürerken menü/ana ekran değişimi.
3. Yeniden açılışta kaydedilmiş dil ve diğer ayarlar.
4. Diğer dili seçip Cancel: ekranda ve yeniden açılışta eski dilin korunması.
5. İngilizceye dönüş ve tekrar kalıcılık.
6. MP3 klasör/oynatma, istasyon sayfaları, Wi-Fi tarama/klavye ve CSV mesajları.
7. Türkçe metadata/dosya/ağ adlarının iki dilde de değişmeden görünmesi.
8. Dil değişimini tekrarlayarak ses, tepki ve bellek davranışının gözlenmesi.

Yukarıdaki liste uygulama tamamlandığındaki planı gösterir; güncel kullanıcı
onayları aşağıdadır.

## Kullanıcı cihaz doğrulaması — 2026-10-22

Kullanıcı kendi derleme/yüklemesinden sonra aşağıdaki akışları tek tek onayladı:

- İngilizce açılış; Settings → Device altında English/Türkçe seçeneklerinin görünmesi.
- Radyo veya MP3 çalarken Türkçe seçip Save ile uygulama: yeniden başlatmadan
  menüler/ana ekran Türkçe oldu, ses çalmayı sürdürdü. İki kaynak ayrı ayrı
  raporlanmadığından her iki kaynakta ayrı test yapıldığı varsayılmadı.
- Türkçe kaydından sonra kapat/aç: Türkçe korundu ve oynatma normal başladı.
- Türkçe arayüzde English seçip Vazgeç: arayüz Türkçe kaldı.
- English seçip Kaydet, ardından kapat/aç: İngilizce korundu ve oynatma normaldi.
- İngilizce SD MP3 klasör gezintisi/oynatma: klasör/dosya adları korundu;
  Parent folder, Back to radio, Pause/Play ve Time metinleri doğru göründü.
- İngilizce Wi-Fi ekranında Scan: açıklamalar/tarama sonucu İngilizce, gerçek
  ağ adları değişmeden göründü. Mevcut bağlantı değiştirilmedi.
- İngilizce CSV içe aktarmada geçerli dosya seçimi: onay ekranındaki istasyon
  sayısı/açıklamalar İngilizceydi; Cancel ile çıkıldı, liste değiştirilmedi.

Temel dil akışları kullanıcı tarafından doğrulandı. Kayıt hatası, geçersiz CSV,
Wi-Fi bağlantı hatalarının her iki dilde görünümü, Cancel sonrasında ayrıca
kapat/aç, çok sayıda dil değişimiyle bellek/uzun süreli oynatma ve farklı eski
kayıt sürümlerinin ayrı cihaz testleri yapılmış sayılmadı. Kaynak incelemesi
cihaz testi yerine geçmez. GitHub yayın hazırlığı hâlâ beklemede.

Bu güncellemede yalnız doğrulama kayıtları değişti; firmware değiştirilmedi,
derleme/yükleme veya otomatik test çalıştırılmadı.
