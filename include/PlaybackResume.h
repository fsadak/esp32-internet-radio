#pragma once
#include <Arduino.h>

struct PlaybackResume {
    bool sd = false;
    String path;
};
bool loadPlaybackResume(PlaybackResume& state);
bool savePlaybackResume(const PlaybackResume& state);
