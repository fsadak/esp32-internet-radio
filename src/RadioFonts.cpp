#include "RadioFonts.h"

extern "C" {
LV_FONT_DECLARE(radio_turkish_14);
LV_FONT_DECLARE(radio_turkish_18);
LV_FONT_DECLARE(radio_turkish_24);
LV_FONT_DECLARE(radio_turkish_28);
LV_FONT_DECLARE(radio_turkish_36);
}

namespace {
lv_font_t fonts[5];
bool initialized = false;
}

void initRadioFonts() {
    if (initialized) return;
    const lv_font_t* bases[] = {&lv_font_montserrat_14, &lv_font_montserrat_18,
        &lv_font_montserrat_24, &lv_font_montserrat_28, &lv_font_montserrat_36};
    const lv_font_t* fallbacks[] = {&radio_turkish_14, &radio_turkish_18,
        &radio_turkish_24, &radio_turkish_28, &radio_turkish_36};
    for (size_t i = 0; i < 5; ++i) {
        fonts[i] = *bases[i];
        fonts[i].fallback = fallbacks[i];
    }
    initialized = true;
}

const lv_font_t* radioFont(uint16_t pixels) {
    initRadioFonts();
    switch (pixels) {
    case 14: return &fonts[0];
    case 24: return &fonts[2];
    case 28: return &fonts[3];
    case 36: return &fonts[4];
    default: return &fonts[1];
    }
}
