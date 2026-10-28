# Favori istasyonlar

Radyo modunda istasyon adının yanındaki boş kalbe dokunarak seçili istasyonu
favorilere ekleyin. Kırmızı dolu kalbe tekrar dokunarak favorilerden çıkarın.
Ana ekrandaki **Favoriler / Favorites** düğmesiyle kutu listesine geçin.

- Sayfa başına 4 sütun × 2 sıra, toplam 8 kutu bulunur. Oklarla sayfa değiştirin.
- Her kutuda sabit radyo simgesi bulunur. İstasyon logosu desteği iptal edilmiştir;
  logo indirme, dönüştürme, saklama veya yönetimi planlanmamaktadır.
  İnternetten logo veya albüm görseli indirilmez.
- İstasyon adları en fazla 20 Unicode kod noktasıyla, Türkçe UTF-8 karakterleri
  bölünmeden gösterilir. Metin tek satırdır; kaymaz ve sığmayan kısım kısaltılır.
- Kutuyu seçtiğinizde istasyon açılır ve liste kapanır. Favoriler MP3 modunda görünmez.
- Boş listede açıklama gösterilir. Görünen favoriler mevcut istasyon listesinin sırasındadır.

## 128 istasyon desteği

0.14.1 sürümünde ekrandaki favori durumu 128 bitlik sabit bir yapıyla tutulur;
33–128 arasındaki istasyonlar da kalbe ve kutu listesine dahil edilir. Bu alan
eski 4 bayt yerine 16 bayttır. Kalıcı kayıt biçimi aynı URL temelli CSV olarak
korunmuştur. Sayfa başına sekiz kutu kullanılır; tüm kayıtlar favoriyse 16 sayfa
oluşur. Sınır koşullarını cihazınızda ayrıca doğrulayın.

## Kalıcılık ve kimlik

Favoriler ayrı `favorites/csv1` NVS kaydında saklanır; genel ayarlar ve istasyon
kataloğu değiştirilmez. Kimlik, yayın adresinin birebir eşleşmesidir. Ad veya
sıralama değişikliği aynı URL'nin favori durumunu bozmaz. URL değişirse yeni
istasyon sayılır. Mevcut katalogda olmayan kayıtlar kutularda gösterilmez;
bir sonraki açık favori düzenlemesinde kayıt mevcut katalogdan yeniden oluşturulur.

Yazım yalnızca kalbe dokunulduğunda yapılır. Kayıt başarısızsa önceki favoriler
korunur ve hata ana ekranda gösterilir. Kaydet/Vazgeç menüsünden bağımsızdır.
NVS yazımı sırasında kısa görüntü kayması mevcut donanım sınırlamaları dahilindedir.

## Doğrulama kapsamı

Kalp özel çizim kullanır. Favori durumu veya seçili istasyon değiştiğinde kalbin
durumu karşılaştırılır; değişmişse yalnız kalp alanı açıkça yeniden çizime alınır.
Bu düzenleme, listenin açılıp kapanmasına kadar eski ikonun görünmesi sorununu
hedefler. 2026-10-28 tarihinde kalbin anlık güncellenmesi fiziksel kartta doğrulanmıştır.

## Sonraki yerleşim düzenlemesi

0.14.0 çalışma sürümünde **Sayfa 1/1** göstergesi, önceki/sonraki ok düğmelerinin
tam ortasına hizalanmıştır. Yeni yerleşim için cihaz doğrulaması beklenmektedir.

Bu özellik 0.13.0 çalışma sürümüne eklenmiştir. Kalp yenileme onayı; Türkçe/uzun adlar,
çok sayfalı listeler, yeniden açılış ve katalog değişimi senaryolarını ayrıca doğrulamaz.
Kalıcılık, URL eşleştirme, boş liste ve kayıt hatası için host test kaynağı bulunur;
bu düzenlemede derleme/yükleme veya otomatik test çalıştırılmamıştır.
