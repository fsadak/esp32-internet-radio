#pragma once
#include <lvgl.h>

// Built-in Montserrat glyphs/icons plus Latin/Turkish fallback at every UI size.
void initRadioFonts();
const lv_font_t* radioFont(uint16_t pixels);
