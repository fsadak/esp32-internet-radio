# İstasyon listeleri

## 0.14.1 kapasite güncellemesi

SD ve web üzerinden en fazla 128 kayıt kabul edilir. Dosyanın tamamı yine
12 KiB sınırı içindedir; uzun ad ve adreslerle 128 kayıttan önce boyut sınırına
ulaşılabilir. Varsayılan bölüm tablosunda NVS alanı 20 KiB olup Wi-Fi, ayarlar
ve favorilerle paylaşılır. Alan yetersizse mevcut hata akışı önceki listeyi korur.
Bölüm adresleri ve NVS kayıt biçimi değiştirilmediği için mevcut listeler
ve URL ile saklanan favoriler okunmaya devam eder. 128 kayıt, son sayfa,
favoriler ve yeniden açılış cihazda doğrulanmayı beklemektedir.


## Cihazdan tek istasyon silme — 0.14.0

İstasyon listesindeki satırın sağındaki çöp kutusuna dokunun. Adı kontrol edip
kalıcı silmek için **Sil / Delete**, listeyi korumak için **Vazgeç / Cancel** seçin.
Çöp kutusu istasyon seçimi yapmaz. Son istasyon silinemez; önce başka bir istasyon ekleyin.

Başka istasyon silinirse mevcut yayın bağlantısı korunur. Çalan/seçili istasyon
silinirse yayın durur; yeni listeden seçim yapın. Açılış ve son istasyon indeksleri
URL ile yeniden eşleştirilir. Favorilerde yalnız mevcut katalogdaki istasyonlar görünür.
Liste NVS'ye kaydedilemezse mevcut liste korunur ve onay ekranı hata gösterir.
Liste kaydedilip ikincil açılış tercihleri kaydedilemezse kısmi kayıt mesajı verilir.
Bu işlem ayarlar menüsünün Kaydet/Vazgeç işleminden bağımsızdır; SD dosyası değiştirilmez.

Wi-Fi bağlıyken yayın bağlantısı kalıcı bir hatayla reddedilirse veya mevcut
yeniden deneme bütçesi tükenirse silme onayı önerilir. Geçici erişim sorunu kalıcı
yayın kapanması sayılmaz; silme yalnız açık onayla yapılır. Wi-Fi kopması tek başına
silme sorusu oluşturmaz. Onay açıkken web/OTA değişiklikleri engellenir.

Bağlantı zaman aşımları ve 3/10/30 saniyelik yeniden deneme aralıkları değiştirilmedi;
uyarının gelmesi anlık olmayabilir. Sayfa başına beş satır ve ayrı silme düğmeleri vardır.
Yeni akışın cihaz doğrulaması beklenmektedir; derleme/yükleme/otomatik test yapılmamıştır.

Web arayüzü ve SD içe aktarma aynı UTF-8, noktalı virgüllü CSV biçimini kullanır:

```csv
name;url
Groove Salad;https://ice5.somafm.com/groovesalad-128-mp3
```

## Biçim ve sınırlar

- İlk dolu satır `name;url` olmalıdır. UTF-8 BOM ve LF/CRLF/CR satır sonları kabul edilir.
- En fazla 128 istasyon ve 12 KiB dosya; ad en fazla 96, adres en fazla 512 bayt.
- Alanların başındaki/sonundaki boşluklar kırpılır; boş satırlar atlanır.
- Adres doğrudan HTTP/HTTPS yayın adresi olmalıdır. Boşluk veya adres içinde
  kullanıcı adı/şifre kabul edilmez. Doğrulama yayını açarak erişim testi yapmaz.
- Noktalı virgül veya çift tırnak içeren alanı çift tırnakla çevreleyin;
  alan içindeki çift tırnakları iki kez yazın. Alan içinde satır sonu desteklenmez.
- Yinelenen URL, boş/eksik alan, hatalı UTF-8 ve fazla sütun reddedilir.
  Dosyanın tamamı geçerli olmadan mevcut liste değiştirilmez.

## SD karttan yükleme

1. CSV dosyalarını kartın kök dizinine kopyalayın; alt klasörler taranmaz.
2. Radyo modunda **Ayarlar → İstasyonlar → SD karttan liste yükle** yolunu açın.
3. Dosyayı seçin, adını ve istasyon sayısını inceleyin.
4. Tüm listeyi değiştirmek için **Listeyi değiştir**, mevcut listeyi korumak için **Vazgeç** seçin.

Dosya seçimi en fazla 64 CSV ve kök dizinde 512 kayıtla sınırlıdır.
Liste NVS'de saklanır; SD kart çıkarıldıktan sonra da kullanılabilir.
İçe aktarma ayrı bir onaydır; genel ayarlarda Vazgeç seçmek yüklenmiş listeyi geri almaz.
Ağ bağlantısı olmadan yükleme yapılabilir; yayın dinlemek için internet gerekir.
SD karta yazma veya biçimlendirme yapılmaz. Birleştirme/sona ekleme henüz yoktur.

Webden düzenleme ve dışa aktarma için [web arayüzü](WEB-INTERFACE.md) belgesini,
örnekler için [sd-lists](sd-lists/README.md) klasörünü inceleyin.
