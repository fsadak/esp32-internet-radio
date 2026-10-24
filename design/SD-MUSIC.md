# SD MP3 oynatıcı

## Dosya seçimi

MP3 dosyalarını SD kartın kök dizinine veya alt klasörlere kopyalayın.
**SD MP3** listesinden klasör açın ve bir dosya seçin. Seçilen klasördeki MP3'ler
çalma listesini oluşturur. Başka klasörlerde gezinmek yeni dosya seçilene kadar
çalan dosyayı veya listeyi değiştirmez. Üst klasör düğmesiyle geri dönebilirsiniz.

Klasör başına en fazla 128 MP3/alt klasör, tarama başına 512 dizin kaydı ve
240 bayt dosya yolu desteklenir. `.mp3` uzantısı büyük/küçük harfe duyarsızdır.
M4A, WebM ve diğer yerel biçimler mevcut oynatıcı kapsamı dışındadır.

## Oynatma

- Oynat/duraklat, önceki/sonraki ve −10/+10 saniye kontrolleri bulunur.
- Süre alanında geçen/toplam zaman gösterilir; yeterli çözücü bilgisi oluşunca sarma etkinleşir.
- Tek parça tekrarı, tüm liste tekrarı ve karışık çalma kullanılabilir.
- Tekrar kapalıyken sonraki parçaya otomatik geçilir; liste sonunda durulur.
- ID3 başlık/sanatçı bilgisi varsa gösterilir; yoksa dosya adı kullanılır.
- Uzun MP3 başlıkları sabit ve üç noktayla kısaltılmış gösterilir.
- Ses, sessize alma, ekolayzer ve VU metre radyo ile ortak kullanılır.

## Açılış ve hata davranışı

Son radyo/MP3 kaynağı ve seçilen dosya NVS'de saklanır. Son MP3 yeniden açılışta
**en baştan** çalar; zaman konumu ve duraklatma durumu kalıcı kaydedilmez.
Periyodik konum yazımları RGB görüntü kaymasına yol açabildiğinden kullanılmaz.

Kart veya son dosya bulunamazsa MP3 modunda hata gösterilir; radyo kendiliğinden
başlamaz. Uygun kartı takıp yeniden başlatabilir veya listeden başka dosya seçebilirsiniz.
Firmware SD karta yazmaz veya kartı biçimlendirmez.

## Sınırlar

VBR süre ve sarma konumu tahminidir; hassas bir çerçeve indeksi kullanılmaz.
Uzun başlık kaydırma, ses sürekliliği için kapalıdır. Kaynak/dosya değişimindeki
NVS yazımları kısa görüntü kaymasına neden olabilir. Bozuk dosyalar ve farklı SD
kartlar ayrıca değerlendirilmelidir; bütün dosyalarla uyumluluk garanti edilmez.
