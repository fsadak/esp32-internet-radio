# Yayın ve parça bilgileri

Alt çubuk durum, tür/format ve istasyon sayacını ayrı satırlarda gösterir.
Tür icy-genre başlığından, codec ses kütüphanesinden alınır; 500 ms aralıkla
kontrol edilir ve değişiklikte ekran güncellenir. İstasyon adıyla aynı olan
icy-genre değeri tür olarak kullanılmaz. Tür yoksa çizgi gösterilir.
Parça/sanatçı bilgisi yoksa alan boş kalır; bağlantı hataları durum alanındadır.
İstasyon değişimi ve yeniden bağlantı eski tür/format bilgisini temizler.
LVGL çizimi ses callback işlevlerinin içinden yapılmaz.
