# Türkçe ve Latin font desteği

Üretilmiş 14/18/24/28/36 piksel fontlar `src/radio_turkish_*.c` dosyalarındadır.
Normal PlatformIO derlemesinde yeniden üretim gerekmez. `RadioFonts.cpp`, ek
karakterleri LVGL Montserrat fontlarına fallback olarak bağlar.

Kaynak LVGL 9.6.0 paketindeki `scripts/generators/built_in_font/Montserrat-Medium.ttf`,
dönüştürücü `lv_font_conv` 1.5.3'tür. Kaynak lisansı [Montserrat-OFL.txt](Montserrat-OFL.txt)
dosyasındadır. Karakter kapsamı [radio-font-codepoints.json](radio-font-codepoints.json)
içinde listelenir; bütün Unicode karakterleri kapsanmaz.

Yeniden üretmek için PlatformIO bağımlılıklarını, Node.js ve npm'yi kurun;
proje kökünde şu komutları çalıştırın:

```sh
npm install --prefix .tools/font-tools lv_font_conv@1.5.3
node design/fonts/generate-radio-fonts.cjs
```

Üretici, zorunlu Türkçe karakterler kaynak fontta eksikse hata verir. Üretilen
kaynakları gözden geçirin ve gerçek ekranda metinleri kontrol edin. Yayıncıdan
bozuk kodlamayla gelen metadata, font kapsamından ayrı bir sorundur.
