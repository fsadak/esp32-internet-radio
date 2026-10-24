# Yerel web arayüzü

Cihazla aynı ağa bağlanın ve **Ayarlar → Cihaz** bölümündeki IP adresini
`http://DEVICE_IP/` biçiminde açın. Sayfa firmware içindedir; ayrıca dosya sistemi,
font veya harici CDN yüklemesi gerekmez. Web arayüzü şu anda Türkçedir.

## Radyo kumandası

İstasyon, parça başlığı, durum, tür, format ve ses bilgileri gösterilir.
Önceki/sonraki, listeden seçim, ses −/+, sessize alma ve yayını başlatma/durdurma
kullanılabilir. Durum yaklaşık iki saniyede bir okunur.

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
- Ayarlar/Wi-Fi kurulumu açıkken veya MP3 modunda webden yazma ve kumanda engellenir;
  durum bilgisi okunabilir. MP3 gezintisi ve uzaktan MP3 kumandası henüz yoktur.
- İnternete açma ve port yönlendirme desteklenen kurulumun dışındadır.
- Tek etkin bağlantı, 2048 bayt başlık, 12 KiB gövde; 3 saniye hareketsizlik ve
  15 saniye toplam aktarım sınırı uygulanır. Okuma/yazma döngü başına sınırlanır.

## HTTP uçları

| Yol | Yöntem | İşlem |
| --- | --- | --- |
| `/` | GET | Web sayfası |
| `/api/state` | GET | Durum, liste kimliği ve CSRF belirteci |
| `/api/stations` | GET | Kayıtlı CSV ve liste kimliği |
| `/api/validate` | POST | CSV doğrulama; kayıt yapmaz |
| `/api/stations` | POST | Kalıcı liste değişimi |
| `/api/command` | POST | Kumanda komutu |

Komutlar: `volumeDown`, `volumeUp`, `mute`, `previous`, `next`, `play`, `stop`,
`select:<sıfır-tabanlı-indeks>`. POST için `X-Radio-Token`, seçim/liste kaydı için
`X-Radio-Revision`, çalan istasyonu çıkarma onayı için `X-Radio-Confirm: stop` kullanılır.
