# Dil desteği

Cihaz arayüzü İngilizce ve Türkçe sunar; varsayılan dil İngilizcedir.
**Settings → Device → Dil / Language** bölümünden dili seçip **Save / Kaydet**
düğmesine basın. Değişiklik yeniden başlatmadan uygulanır ve kalıcı kaydedilir.
**Cancel / Vazgeç** önceki dili korur.

Ana ekran, ayarlar, ekolayzer, Wi-Fi kurulumu, istasyon/dosya listeleri, CSV onayları
ve cihazın durum/hata mesajları çevrilir. İstasyon adları, yayıncı metadata'sı,
dosya/klasör adları, SSID ve adresler olduğu gibi gösterilir.

Web arayüzü şu anda yalnızca Türkçedir; cihaz dili web sayfasını değiştirmez.
Seri tanılama mesajları dil seçiminin kapsamı dışındadır.

## Uygulama yapısı

- `RadioLanguage.cpp`: cihaz metinleri ve İngilizce karşılıkları.
- `RadioDisplay.cpp`: arayüz etiketleri ve dil değişiminde yenileme.
- `RadioFonts.cpp`: Türkçe/Latin karakterler için font fallback bağlantıları.
- Dil, genel ayar kaydında saklanır; eski kayıtlarda İngilizce varsayılır.
- Çeviri, metadata üzerinde sözcük değiştirme işlemi uygulamaz.

Çeviri eklerken değişken yer tutucularını ve arayüz simgelerini koruyun.
Font kapsamı tüm Unicode karakterlerini içermez; [font notlarını](fonts/README.md) inceleyin.
