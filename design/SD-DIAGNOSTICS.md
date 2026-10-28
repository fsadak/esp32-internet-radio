# SD kart tanılama — 0.3.2

Bu adım yalnızca kartı tanıma ve kök dizini listelemedir. MP3 çalma ve istasyon
dosyası içe aktarma henüz uygulanmaz. MP3 işlevleri TODO.md dosyasına kaydedildi.

- Kart tanımı: CS10, MOSI11, SCLK12, MISO13; SPI 1 MHz.
- Kontrol açılışta, ekran başlatıldıktan sonra ve yayın açılmadan önce yapılır.
- Kart tipi, kapasite ve kökteki en fazla 40 kaydın adı/boyutu 115200 baud seri
  çıktıda gösterilir. Alt klasörlerin içerikleri dolaşılmaz.
- Dosyalar FILE_READ ile açılır. Biçimlendirme açıkça kapalıdır; dosya oluşturma,
  yazma, silme veya ayar/istasyon değişikliği yapılmaz.
- Kart yoksa veya okunamazsa tanılama hata mesajı verip radyo başlangıcını sürdürür.
- Bu sürüm çalışırken kart takmayı/çıkarmayı izleyen bir mekanizma içermez;
  kart açılıştan önce takılmalı veya kart takıldıktan sonra cihaz yeniden başlatılmalıdır.

## Fiziksel test

Kartta bilinen bir dosya bulundurun. Açılış seri kaydında kart türü/kapasitesi ve
bu dosyanın listelendiğini kontrol edin. Ekran, ses ve kayıtlı ayarların çalışmasını
kontrol edin. İkinci kontrolde kart yokken de radyo normal açılmalıdır.


## İlk fiziksel sonuç

0.3.2 COM5’e yüklendi. Seri kayıtta SDHC sınıflandırmasıyla 60948 MB kart algılandı; kökte 3 dosya ve 1 klasör listelendi. Sonrasında Wi-Fi ve radyo yayını başladı, parça bilgisi alındı. Kart olmadan açılış testi henüz yapılmadı.
