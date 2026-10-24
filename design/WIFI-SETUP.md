# Wi-Fi kurulumu

Cihaz 2,4 GHz Wi-Fi ağlarıyla çalışır ve tek ağ profili saklar.
Kayıtlı ağ yoksa açılışta kurulum ekranı görüntülenir.

1. Listeden ağınızı seçin veya gizli ağ için SSID'yi elle yazın.
2. Şifrenizi ekran klavyesiyle girin.
3. **Bağlan ve kaydet / Connect and save** seçin.
4. Bağlantı kurulduktan sonra kurulum ekranını kapatın.

Sonradan **Ayarlar → Cihaz → Wi-Fi ağını ayarla / değiştir** yolunu kullanın.
Liste en fazla 32 farklı görünür SSID gösterir; aynı adlı erişim noktaları birleştirilir.
SSID sınırı 32 bayttır. Açık ağda şifreyi boş bırakın; kişisel şifreli ağda 8–63 bayt
veya 64 onaltılık hane kullanın. Şifre başındaki/sonundaki boşluklar korunur.
Kurumsal sertifika/kimlik girişi ve çoklu profil yöneticisi sunulmaz.

## Kayıt ve bağlantı davranışı

Yeni bilgiler deneme süresince RAM'de tutulur. Doğru SSID'den IP alınınca
`radioWifi/network` NVS bloğuna kaydedilir. IP alınması internet erişimi garantisi değildir.
30 saniye içinde bağlantı kurulamazsa eski kayıt korunur ve varsa önceki ağ yeniden denenir.
**Kapat / denemeyi iptal et** devam eden denemeyi iptal eder; başarıyla kaydedilmiş
bir profili geri almaz. Kayıtlı ağa bağlanılamazsa kurulum yeniden açılır; flash silinmez.

Ağ değiştirme mevcut yayını keser. Bağlantı sonrası oynatma, radyo isteği ve açılış
tercihlerine göre sürer. İlk kurulumu kapatarak çevrimdışı SD MP3 kullanabilirsiniz.
Wi-Fi kurulumu sırasında webden değişiklik engellenir; IP değişirse yeni adresi açın.

## Şifreler

Şifre maskelenir ve kayıtlı şifre forma geri doldurulmaz. Ağ değiştirirken şifrenizi
yeniden girin. Şifre seri çıktıya veya web durumuna eklenmez. Standart NVS kullanılır;
ayrıca şifreli NVS yapılandırması sunulmaz. Kaynak koda ağ bilgisi yazmanız gerekmez.
