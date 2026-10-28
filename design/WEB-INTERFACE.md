# Yerel web arayüzü

Cihazla aynı ağa bağlanın ve **Ayarlar → Cihaz** bölümündeki IP adresini
`http://DEVICE_IP/` biçiminde açın. Sayfa firmware içindedir; ayrıca dosya sistemi,
font veya harici CDN yüklemesi gerekmez. Web arayüzü şu anda Türkçedir.

## Radyo kumandası

İstasyon, parça başlığı, durum, tür, format ve ses bilgileri gösterilir.
Önceki/sonraki, listeden seçim, ses −/+, sessize alma ve yayını başlatma/durdurma
kullanılabilir. Durum yaklaşık iki saniyede bir okunur.

API'de `play` yalnızca başlatır/sürdürür; zaten çalan MP3'ü duraklatmaz.
`pause`, MP3 çalıyorsa duraklatır; zaten duraklamışsa veya durmuşsa başlatmaz.
Canlı radyoda `pause` bağlantıyı kapatır. `toggle` mevcut durumu tersine çevirir.
Webdeki tek MP3 düğmesi çalma durumuna göre açıkça `play` veya `pause` gönderir.
Bu komut ayrımı için yeni cihaz doğrulaması beklenmektedir.

## İstasyon yönetimi

Ad/URL düzenleme, ekleme/silme, sıralama ve ortak CSV biçiminde içe/dışa aktarma
bulunur. Düzenlemeler cihaza kaydedilene kadar tarayıcıda taslaktır. CSV indirme,
kayıtlı listeyi verir; kaydedilmemiş taslağı içermez.

Başka tarayıcı veya SD içe aktarma listeyi değiştirmişse eski taslak reddedilir;
güncel listeyi yeniden alın. Çalan istasyon çıkarılacaksa ayrıca onay gerekir.
Aynı URL korunursa seçili istasyon yeniden eşleştirilir. Ana kayıt başarısızsa
önceki liste korunur; ikincil ayar kaydı başarısızsa kısmi kayıt durumu bildirilir.

## Erişim ve sınırlar

- Yerel HTTP kullanılır; hesap/şifre veya TLS sunulmaz. Güvenilen yerel ağ içindir.
- Host/Origin denetimi ve CSRF belirteci bulunur; bunlar kullanıcı yetkilendirmesi değildir.
- Ayarlar/Wi-Fi kurulumu veya OTA güncellemesi sırasında webden yazma ve kumanda engellenir;
  durum bilgisi okunabilir. SD MP3 modunda uzaktan oynatma/ses kumandası ve **Radyoya dön**
  komutu kullanılabilir; istasyon listesi kaydı ise engellenir. Webden MP3 klasör gezintisi henüz yoktur.
- İnternete açma ve port yönlendirme desteklenen kurulumun dışındadır.
- Tek etkin bağlantı, 2048 bayt başlık, CSV için 12 KiB gövde; normal isteklerde 3 saniye
  hareketsizlik ve 15 saniye toplam aktarım sınırı (OTA yüklemesinde 15 sn / 180 sn) uygulanır.

## HTTP uçları

| Yol | Yöntem | İşlem |
| --- | --- | --- |
| `/` | GET | Web sayfası |
| `/api/state` | GET | Durum, cihaz kimliği, liste kimliği ve CSRF belirteci |
| `/api/stations` | GET | Kayıtlı CSV ve liste kimliği |
| `/api/validate` | POST | CSV doğrulama; kayıt yapmaz |
| `/api/stations` | POST | Kalıcı liste değişimi |
| `/api/command` | POST | Kumanda komutu |
| `/api/firmware` | GET | Firmware sürümü, ürün kimliği ve OTA alan kapasitesi |
| `/api/firmware` | POST | Uygulama `.bin` OTA yüklemesi (`X-Radio-Confirm: update`) |

Komutlar: `volumeDown`, `volumeUp`, `mute`, `previous`, `next`, `play`, `pause`, `stop`,
`radio`, `select:<sıfır-tabanlı-indeks>`. POST için `X-Radio-Token`, seçim/liste kaydı için
`X-Radio-Revision`, çalan istasyonu çıkarma onayı için `X-Radio-Confirm: stop` kullanılır.
