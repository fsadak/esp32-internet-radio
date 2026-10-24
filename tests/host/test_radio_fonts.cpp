#include "RadioFonts.h"
#include <cstdlib>
#include <iostream>

unsigned checks = 0;
void check(bool passed, const char* message) {
    ++checks;
    if (!passed) { std::cerr << "FAIL: " << message << '\n'; std::exit(1); }
}

int main() {
    lv_init();
    initRadioFonts();
    lv_font_glyph_dsc_t oldGlyph{};
    check(!lv_font_get_glyph_dsc(&lv_font_montserrat_28, &oldGlyph, U'ş', 0) || oldGlyph.is_placeholder,
          "regression reproduced: old song font lacks Turkish letter");
    const char32_t* letters = U"ÇĞİÖŞÜçğıöşüÂÎÛâîûéÉäÄß“”’–—…€₺";
    const char32_t* samples = U"ŞİMDİ ÇALIYOR Önceki İstasyon listesi Açılış istasyonu Vazgeç Parlaklık Bağlı değil Gülşen - Bir İhtimal Biliyorum Zeki Müren - Şimdi Uzaklardasın Kâğıt İSTASYON LİSTESİ";
    for (uint16_t size : {14,18,24,28,36}) {
        const lv_font_t* font = radioFont(size);
        for (const char32_t* p = letters; *p; ++p) {
            lv_font_glyph_dsc_t glyph{};
            check(lv_font_get_glyph_dsc(font,&glyph,*p,0) && !glyph.is_placeholder && glyph.resolved_font,
                  "extended glyph resolved through real LVGL fallback");
            check(glyph.box_w > 0 && glyph.box_h > 0 && glyph.adv_w > 0,
                  "extended glyph has drawable metrics");
            glyph.req_raw_bitmap = 1;
            check(glyph.resolved_font->get_glyph_bitmap(&glyph,nullptr) != nullptr,
                  "extended glyph bitmap can be read by real LVGL decoder");
            lv_font_glyph_release_draw_data(&glyph);
        }
        for (const char32_t* p = samples; *p; ++p) {
            lv_font_glyph_dsc_t glyph{};
            check(lv_font_get_glyph_dsc(font,&glyph,*p,0) && !glyph.is_placeholder,
                  "Turkish menu/song sample has no missing glyph");
        }
        for (uint32_t cp : {uint32_t('A'),uint32_t('9'),uint32_t(0xf013),uint32_t(0xf028),uint32_t(0xf04e)}) {
            lv_font_glyph_dsc_t glyph{};
            check(lv_font_get_glyph_dsc(font,&glyph,cp,0) && !glyph.is_placeholder,
                  "ASCII and existing UI icons preserved");
        }
    }
    std::cout << "PASS: " << checks << " real-LVGL font checks at 14/18/24/28/36 px\n";
    lv_deinit();
}
