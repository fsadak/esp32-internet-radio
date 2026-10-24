# Türkçe ve Latin harf ekleri — 0.4.1

Önceki sürümde yalnızca 18 ve 36 piksel fontlar Türkçe harf ekine sahipti. Şarkı adı
28 piksel, bazı başlık/açıklamalar 14 ve 24 piksel olduğundan bu alanlar eksikti.

Artık `src/radio_turkish_14.c`, `18.c`, `24.c`, `28.c`, `36.c` fontları kullanılır.
Her boyutta 233 ek karakter vardır: Latin-1, Latin Extended-A, Türkçe ÇĞİÖŞÜçğıöşü,
şapkalı ÂÎÛâîû ve yaygın tipografik tırnak/tire/noktalama işaretleri.

Kaynak, kullanılan LVGL 9.6.0 paketindeki
`scripts/generators/built_in_font/Montserrat-Medium.ttf`; dönüştürücü `lv_font_conv` 1.5.3.
OFL lisansı `Montserrat-OFL.txt` içindedir. `RadioFonts.cpp`, mevcut Montserrat ASCII
harfleri ve simgelerine bu ekleri fallback olarak bağlar. `RadioDisplay.cpp` bütün
metin boyutlarını `radioFont()` üzerinden alır.

Üretilmiş font kaynakları projeye dahildir; normal PlatformIO derlemesinde
font üreticisini çalıştırmanız gerekmez. Fontları yeniden üretmek için önce
PlatformIO bağımlılıklarını kurun; LVGL içindeki kaynak TTF kullanılır.
Node.js/npm ile proje kökünde şu komutları çalıştırın:

```powershell
npm install --prefix display-test/font-tools lv_font_conv@1.5.3
node design/fonts/generate-radio-fonts.cjs
```

Karakter listesi `radio-font-codepoints.json` içindedir. Üretici, kaynak fontta zorunlu
Türkçe karakter eksikse hata verir. Dönüştürücü yalnızca `display-test/font-tools/`
altındadır; firmware'e yeni kütüphane bağımlılığı eklenmedi.

Gerçek LVGL font motoru ve üretilmiş bitmaplerle bilgisayar kontrolü:

```powershell
python display-test/build-host-lvgl.py
& .\display-test\host-tools\ziglang\zig.exe c++ -target x86_64-windows-gnu -std=c++17 -O2 -DLV_CONF_INCLUDE_SIMPLE -I include -I .pio/libdeps/esp32_radio/lvgl tests/host/test_radio_fonts.cpp src/RadioFonts.cpp display-test/lvgl-font-host.lib -o display-test/test-radio-fonts.exe
& .\display-test\test-radio-fonts.exe
```

Bu kontrol gerçek paneldeki görsel testi veya sunucunun gönderdiği metadata kodlamasının
doğrulanmasını kapsamaz. Metin aktarımında veya yayıncı kodlamasında ayrıca hata varsa,
font kapsamı tamamlandıktan sonra ham metadata örneğiyle incelenir.
