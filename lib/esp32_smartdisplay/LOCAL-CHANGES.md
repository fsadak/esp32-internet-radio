# Local display initialization fix

Upstream: https://github.com/rzeldent/esp32-smartdisplay
Commit: 38e9ebabe3e4044159c4e95192d432607ed70588 (2.1.2).

This source copy is stored in the radio project's vendored `lib` directory so the
fix survives removal of `.pio`. PlatformIO discovers it through the existing
`esp32_smartdisplay.h` include. LVGL remains pinned to 9.6.0 in platformio.ini.

The esp32-8048S070C board configures ST7262_PANEL_CONFIG_DISP as GPIO_NUM_NC.
With espressif32 6.10.0 / Arduino 2.0.17, calling panel display on/off without
that pin returns ESP_ERR_NOT_SUPPORTED, and the upstream ESP_ERROR_CHECK aborts.

In include/lvgl_panel_common.h, skip only the display on/off call for an
ST7262 RGB panel whose DISP pin is GPIO_NUM_NC. Panel initialization still runs;
other panel configurations retain the original error checks. No pin, timing,
touch, or backlight settings were changed.

The radio board definition is boards/sunton-8048s070-radio.json. It preserves
the working LCD configuration and sets GT911_TOUCH_CONFIG_INT to GPIO_NUM_NC:
GPIO18 is reserved for audio LRC. Touch coordinates are polled over I2C.
The combined configuration targets Sunton ESP32-8048S070 PCB V1.3.

## RGB frame callback (2026-10-28)

Removed the extra GDMA stop/reset/start and LCD FIFO reset from the frame-done
callback. ESP-IDF 4.4.7 invokes this callback from its VSYNC ISR, then restarts
the LCD engine and DMA itself. The application must not independently reset the
DMA while the LCD engine is still running. The callback now only signals LVGL.

Driver reference:
https://github.com/espressif/esp-idf/blob/v4.4.7/components/esp_lcd/src/esp_lcd_rgb_panel.c

This restores the original callback path for the pinned platform. PCLK and pin
assignments are unchanged. The change targets horizontal stretching/flicker during
OTA; hardware confirmation is pending and it is not a claim that all flash-write
or PSRAM bandwidth artefacts are resolved.
