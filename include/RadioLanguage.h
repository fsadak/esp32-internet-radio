#pragma once
#include <Arduino.h>
#include "RadioSettings.h"

void setRadioLanguage(RadioLanguage language);
RadioLanguage radioLanguage();
// Device-generated UI text only. Never pass broadcaster metadata, paths or SSIDs.
// Canonical Turkish keys are retained internally for live language changes.
String uiText(const String& canonical);
