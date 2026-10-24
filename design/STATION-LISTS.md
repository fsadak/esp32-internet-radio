# SD karttan istasyon listesi — 0.4.0

## Kullanım

1. UTF-8 CSV dosyalarını SD kartın kök dizinine kopyala. Dosya adı serbesttir;
   uzantı `.csv` olmalıdır. Alt klasörler taranmaz. Örnekler `design/sd-lists/` içindedir.
2. Ayarlar → İstasyonlar → SD karttan liste yükle yolunu aç.
3. Dosyayı seç; doğrulama başarılıysa dosya adı ve istasyon sayısını kontrol et.
4. **Listeyi değiştir** düğmesi mevcut listenin tamamını değiştirir ve cihazın NVS
   belleğine kaydeder. Kart üzerindeki dosyaya yazılmaz; biçimlendirme yapılmaz.
5. İlk istasyon seçilir. Önceden yayın istenmişse yeni listenin ilk istasyonu açılır;
   aksi halde listeden istasyon seçilmesi beklenir. SD kart olmadan da liste korunur.

İçe aktarma, ayarlar ekranının Kaydet/Vazgeç işlemlerinden ayrı bir onaydır.
İçe aktarmadan sonra ayarlarda Vazgeç seçmek, yüklenmiş istasyon listesini geri almaz.
İçe aktarma mevcut parlaklık/ses taslak ayarlarını kendiliğinden kaydetmez.
Ağa bağlanmadan dosya seçme, doğrulama ve kaydetme kullanılabilir; yayın dinlemek
internet bağlantısı gerektirir.

## Ortak dosya biçimi

```csv
name;url
Groove Salad;https://ice5.somafm.com/groovesalad-128-mp3
```

- İlk dolu satır `name;url`; sıra dosya satırlarıyla belirlenir.
- Kodlama UTF-8; UTF-8 BOM kabul edilir. LF, CRLF ve CR satır sonları desteklenir.
- Boş satırlar atlanır; alan başı/sonu boşlukları kırpılır.
- İsim UTF-8 olarak en fazla 96 bayt; URL en fazla 512 bayt.
- Yayın adresi doğrudan `http://` veya `https://` adresi olmalı; boşluk ve kullanıcı
  adı/şifre içeren adresler kabul edilmez. Adresin gerçek bir yayın olması ağda denenir;
  dosya doğrulaması yayını açmaz.
- En fazla 32 istasyon, 12 KB dosya; bellek doluysa kayıt reddedilir ve eski liste korunur.
- Noktalı virgül veya çift tırnak içeren alanı çift tırnakla çevrele;
  alan içindeki çift tırnağı iki kez yaz. Alan içinde satır sonu desteklenmez.
- Aynı URL'nin birebir tekrarı, boş/eksik alan, hatalı UTF-8 veya fazla sütun reddedilir;
  hatalarda mümkün olduğunda satır numarası gösterilir. Tüm dosya geçerli olmadan kayıt yapılmaz.
- Dosya seçiminde en fazla 64 CSV, kök dizinde en fazla 512 kayıt desteklenir;
  aşılırsa eksik bir liste göstermek yerine hata verilir.

Excel'in bölge ayarına göre CSV ayırıcı virgül olabilir. Bu cihaz **noktalı virgül**
bekler; dosyayı metin düzenleyicide kontrol et. Türkçe harfler için ANSI yerine UTF-8 kullan.

İlerideki web arayüzü aynı `name;url` biçimini içe/dışa aktaracak. Firmware'in
`StationCsv` ayrıştırıcısı ve dışa aktarma fonksiyonu bu amaçla ortak kullanılabilir.
Web hizmeti ve mevcut listeye ekleme/birleştirme henüz uygulanmadı.

## Doğrulama durumu

- Önceki 0.3.2 SD tanılama sürümünde kartlı açılış ve kart olmadan reset sonrası radyo
  kullanıcı tarafından doğrulandı.
- 0.4.0 COM5 üzerinden yüklendi; açılış ve yayın seri kayıtta doğrulandı.
- Kullanıcı CSV bulunamadı mesajını, dosya seçimini, `sd-test.csv` için 1 istasyon
  önizlemesini, onayla liste değişimini ve `Groove Salad (SD Test)` yayınının çaldığını doğruladı.
- Kullanıcı cihazı kapatıp SD kartı çıkardıktan sonra yeniden açılışta istasyon adının
  korunduğunu ve yayının çaldığını doğruladı.
- Kullanıcı boş URL içeren `sd-invalid.csv` dosyasının reddedildiğini ve hata/koruma
  mesajını doğruladı. Menüden çıkınca `Groove Salad (SD Test)` adı ve yayın korundu.
- Kullanıcı `groove-salad.csv` önizlemesinde Vazgeç seçilince `Groove Salad (SD Test)`
  adının ve yayının korunduğunu doğruladı.
- Kullanıcı üçlü listeyi, üç yayının çalmasını, önceki/sonraki geçişlerini ve iki yönde
  sarma davranışını doğruladı. Listeden Drone Zone (2/3) seçimi başarılı.
- Otomatik yayın ve son istasyonu hatırlama açıkken reset sonrası Drone Zone (2/3)
  otomatik çaldı; kullanıcı doğruladı.
- Son istasyonu hatırlama kapalı ve sabit açılış istasyonu Secret Agent iken Groove Salad
  çalınıp reset yapıldı. Secret Agent (3/3) otomatik çaldı; kullanıcı doğruladı.
- SD istasyon yüklemenin temel akışları kullanıcı tarafından doğrulandı; kabul edilen sürüm 0.4.0.
- Kullanıcı Türkiye seçme CSV’sinin 32 istasyon önizlemesini ve yüklenmesini doğruladı.
  Radyo 7 (1/32), Süper FM (2/32), TRT FM (4/32) ve Radyo Spor (32/32) çaldı.
- 4. liste sayfasına geçiş, son satırdan Radyo Spor seçimi ve Sonraki ile 32→1 dönüşü
  doğrulandı. Bu sonuç diğer 28 yayının cihazda çaldığını göstermez.
- 12 KB dosya, 64 CSV/512 kayıt sınırları, kayıt hatası ve kayıt sırasında güç kesintisi
  donanım testleri henüz yapılmadı.
  Bilgisayar test/derleme kayıtları `display-test/` içinde.
- Bilgisayardaki kalıcılık testleri NVS benzetimi kullanır; gerçek flash/SD kartın
  fiziksel hata ve güç kesintisi testleri yerine geçmez.
