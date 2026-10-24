# Batarya ve besleme planı — 2026-10-22

## Güncel durum: sonraki donanım iyileştirmelerine ertelendi

Kullanıcı, batarya/UPS montajı ve besleme bağlantılarını daha sonraki donanım
iyileştirmeleri aşamasına bıraktı. Modülün harici LED bağlantıları da aynı aşamada
ele alınacak; şu anda LED bağlantısı veya firmware entegrasyonu yapılmayacak.
İki hücreli paket, şarj olurken çalışma ve LED ile durum gösterimi kararları korunur.
Ekranda pil göstergesi şimdilik iptal olarak kalır.

Kararlaştırılan besleme topolojisi:

```text
USB adaptörün 5 V beslemesi → UPS IN+ / IN−
Uygun korumalı 1S2P paket  → UPS B+ / B−
UPS OUT+ / OUT−           → Sunton kartın ana 5 V / GND beslemesi
```

Bu, fiziksel bağlantı yapılmış veya uçların doğrulanmış olduğu anlamına gelmez.
Donanım aşamasında modülün gerçek pedleri, pil koruması, çıkış gerilimi/kutupları,
USB güç kaynakları arasında geri beslemenin önlenmesi ve LED bağlantılarının
elektriksel koşulları kontrol edilecek. Ardından adaptör takma/çıkarma,
şarj sırasında oynatma, sıcaklık ve düşük batarya testleri yapılacak.

## Kullanıcı tarafından belirlenen kapsam

- Hücreler: 2 adet yeni, aynı model Orion 18650 3200 mAh / 3,7 V Li-ion.
- Paket: **1S2P**, yani iki hücre paralel; seri bağlanmayacak.
- Nominal değerler: 3,7 V, 6400 mAh, 23,68 Wh; tam dolu gerilim 4,2 V.
- Başlangıçtaki 4 hücre / 1S4P / 12.800 mAh planı iptal edildi.
  Gerekçe: maliyet ve ağırlığı düşürmek, incelenen modülün kılavuzundaki
  10.000 mAh altı kapasite önerisinin altında kalmak.
- Bağımsız Motorobit 5 V/1 A UPS modülü üzerinden ilerleniyor.
- Cihaz, batarya şarj olurken radyo/MP3 çalmayı sürdürebilmeli; adaptör
  çıkarıldığında bataryaya geçişte reset olmaması fiziksel testle doğrulanmalı.
- Pil durumunu göstermek için UPS modülünün LED göstergeleri kullanılacak.
  LED pedlerinin anlamı/bağlantısı gerçek kart ve dokümanla doğrulanmadan
  kesin pin bağlantısı veya dirençsiz bağlantı önerilmedi.
- Ekranda pil seviyesi gösterimi, ADC ile ölçüm ve fuel-gauge entegrasyonu
  kullanıcı tarafından şimdilik iptal edildi. Firmware işi başlatılmayacak.

## Kullanıcı ölçümleri ve enerji hesabı

USB besleme ile kart arasındaki güç ölçerden bildirilen değerler:

| Durum | Güç | USB akımı |
| --- | --- | --- |
| Ekran tam açık | 1,83 W | 0,37 A |
| Ekran karanlık | 0,85 W | 0,17 A |

Ekranın karanlık olması tüm cihazın uykuda olduğu anlamına gelmez; mevcut
uygulama arka ışığı kapatır, ses çalmayı sürdürür. Tüketim farkı yaklaşık %54.
23,68 Wh nominal enerjinin %80–90'ının 5 V yükte kullanılabildiği varsayımıyla:

- Ekran açık: yaklaşık **10–12 saat**.
- Ekran karanlık: yaklaşık **22–25 saat**.

Bunlar hesaplanan tahminlerdir; modül verimi, hücrelerin gerçek kapasitesi,
ses seviyesi, yük değişimleri ve kesme gerilimiyle değişir. Çalışma süresi testi
yapılmadı. 6400 mAh değeri pil gerilimindedir; 5 V çıkış kapasitesi değildir.

## Doğrulanmamış donanım ayrıntıları

- Motorobit kartının gerçek entegreleri ve şeması henüz doğrulanmadı.
  ETA9740/DW01/8205A gibi parça adları kesin BOM olarak kabul edilmiyor.
- Gerçek şarj akımı, koruma eşikleri ve farklı çalışma durumlarındaki kısa devre
  koruması kesinleşmedi. Hücreler korumasız; paket koruması donanım aşamasında
  doğrulanmalı. Daha küçük paket seçmek bu belirsizlikleri çözülmüş saydırmaz.
- "0 ms geçiş", gerilimde hiç düşme olmaması veya %96 sistem verimi doğrulanmadı.
- İlk güç testi: kararlı 5 V, açılış/Wi-Fi/ses yükü, adaptör takma/çıkarma,
  eşzamanlı şarj/çalışma, sıcaklık ve düşük batarya davranışı.
- Modül OUT hattı ile bilgisayar USB beslemesinin birlikte kullanımı için
  geri besleme yolu incelenmeden iki güç kaynağı paralel bağlanmayacak.

## Kaynaklar ve durum

- Pil: https://www.pilpaketi.com/orion-18650-3200mah-3.7v-3c-sarj-edilebilir-li-ion-pil
- UPS: https://www.motorobit.com/5v-1a-ups-modulu-kesintisiz-guc-kaynagi-devresi
- Paylaşılan kılavuz (Motorobit kartıyla birebir eşleşmesi kesinleşmedi):
  https://manuals.plus/m/f001085c9eb8bf6abed3ee9d2d3a3823f93f02c8ec22f061b0a387af9b0b3d60

Bu güncelleme plan/karar kaydıdır. Firmware değiştirilmedi; derleme, yükleme,
pil montajı, bağlantı veya donanım testi yapılmadı.
