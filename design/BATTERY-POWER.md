# Batarya ve besleme planı

Batarya sistemi henüz kurulmuş veya test edilmiş değildir; sonraki donanım aşamasıdır.
Hedef, şarj olurken radyo/MP3 oynatmayı sürdürmek ve adaptör çıkarıldığında reset olmadan
bataryaya geçmektir. Ekranda pil seviyesi gösterimi mevcut kapsam dışındadır.

## Planlanan yapı

- İki adet aynı model Orion 18650 3200 mAh / 3,7 V Li-ion hücre.
- 1S2P paket: 3,7 V nominal, 4,2 V tam dolu, 6400 mAh / 23,68 Wh.
- Bağımsız 5 V/1 A UPS modülü ve harici durum LED'leri.

```text
USB 5 V adaptör → UPS güç girişi
Korumalı 1S2P paket → UPS batarya girişi
UPS 5 V çıkışı → Sunton kartın ana 5 V/GND beslemesi
```

Bu şema fiziksel ped/kutup doğrulamasının yerine geçmez. Uygulamadan önce hücre
korumasını, modülün gerçek şarj akımını/korumalarını, LED bağlantılarını ve USB
kaynakları arasındaki geri beslemeyi kontrol edin. Adaptör geçişi, şarj sırasında
oynatma, sıcaklık ve düşük batarya koşulları fiziksel olarak test edilmelidir.

## Mevcut USB ölçümleri

| Durum | Güç | Akım |
| --- | ---: | ---: |
| Tam parlaklık | 1,83 W | 0,37 A |
| Arka ışık kapalı | 0,85 W | 0,17 A |

Bunlar oynatma sırasında gözlenen değerlerdir; azami tüketim veya derin uyku akımı değildir.
Nominal enerjinin %80–90'ının 5 V yükte kullanılabildiği varsayımıyla ekran açık
10–12 saat, arka ışık kapalı 22–25 saat hesaplanır. Bunlar çalışma süresi testi değildir;
verim, yük, hücre kapasitesi ve kesme gerilimi sonucu değiştirir.
