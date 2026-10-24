#include "PlaybackResume.h"
#include "SdMusicFiles.h"
#include <Preferences.h>
#include <cstring>

namespace {
// Preserve the v1 blob layout for migration, but v2 stores only source/path.
// Legacy time, position, size and playing fields are zeroed and never restored.
struct StoredResume {
    uint32_t version, fileSize, position, seconds, duration;
    uint8_t sd, playing;
    char path[241];
};
}

bool loadPlaybackResume(PlaybackResume& state) {
    Preferences prefs;
    if (!prefs.begin("playback", false)) return false;
    StoredResume stored{};
    const bool found = prefs.isKey("resume") && prefs.getBytesLength("resume") == sizeof(stored) &&
        prefs.getBytes("resume", &stored, sizeof(stored)) == sizeof(stored);
    prefs.end();
    if (!found || (stored.version != 1 && stored.version != 2) || stored.sd > 1 || !memchr(stored.path, 0, sizeof(stored.path))) return false;
    if (stored.path[0]) {
        String path = stored.path, lower = path; lower.toLowerCase();
        if (!validSdMusicPath(path) || !lower.endsWith(".mp3")) return false;
    } else if (stored.sd) return false;
    state.sd = stored.sd; state.path = stored.path;
    return true;
}

bool savePlaybackResume(const PlaybackResume& state) {
    if (state.path.length() > 240 || (state.sd && state.path.isEmpty())) return false;
    StoredResume stored;
    memset(&stored, 0, sizeof(stored));
    stored.version = 2;
    stored.sd = state.sd;
    state.path.toCharArray(stored.path, sizeof(stored.path));
    Preferences prefs;
    if (!prefs.begin("playback", false)) return false;
    StoredResume previous{};
    const bool unchanged = prefs.isKey("resume") && prefs.getBytesLength("resume") == sizeof(previous) &&
        prefs.getBytes("resume", &previous, sizeof(previous)) == sizeof(previous) &&
        memcmp(&previous, &stored, sizeof(stored)) == 0;
    const bool saved = unchanged || prefs.putBytes("resume", &stored, sizeof(stored)) == sizeof(stored);
    prefs.end();
    return saved;
}
