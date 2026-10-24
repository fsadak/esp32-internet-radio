# İstasyon listeleri

Web arayüzü ve SD içe aktarma aynı UTF-8, noktalı virgüllü CSV biçimini kullanır:

```csv
name;url
Groove Salad;https://ice5.somafm.com/groovesalad-128-mp3
```

## Biçim ve sınırlar

- İlk dolu satır `name;url` olmalıdır. UTF-8 BOM ve LF/CRLF/CR satır sonları kabul edilir.
- En fazla 32 istasyon ve 12 KiB dosya; ad en fazla 96, adres en fazla 512 bayt.
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
