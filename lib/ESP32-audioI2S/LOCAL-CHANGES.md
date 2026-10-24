# Local HTTP authorization fix

Source: PlatformIO esphome/ESP32-audioI2S 2.0.7, copied from the version used by
the working radio. LICENSE and original library metadata are retained.

In Audio::connecttohost, append the Basic Authorization
header only if auth > 0 (a username or password was provided). Public streams
requested without credentials no longer send an empty Basic credential.

A narrow public disconnectStream() method stops playback and closes both TCP/TLS
clients before an application-managed retry. stopSong() by itself does not close
those clients; the existing internal setDefaults() helper is private and also
resets decoder/buffer state. Decoder reset remains in the original connecttohost
path. No audio or I2S configuration was changed.

This follows the conditional-header behavior in the upstream projects:

- https://github.com/e2002/yoradio/blob/main/yoRadio/src/audioI2S/Audio.cpp
- https://github.com/schreibfaul1/ESP32-audioI2S/blob/master/src/Audio.cpp

The library stays at 2.0.7. The project-private lib source is automatically
discovered by PlatformIO through Audio.h. No cached .pio source is edited.
Application HTTP error presentation and bounded retry are in src/main.cpp.

## Broadcaster genre callback (radio 0.4.2)

The previously ignored `icy-genre` response header is trimmed and forwarded to
the optional weak `audio_icygenre(const char*)` callback. The application copies
the value into its display state; it never draws from the header parser.
No genre is inferred from the station name or URL. The actual codec is read by
the application with `getCodec()`, not guessed from a stream URL extension.

The backup location is recorded in display-test/http-recovery-backup-path.txt
at the project root. It contains the complete original audio source and radio
files; the main.cpp backup contains Wi-Fi credentials and must remain private.

## MP3 duration precision (radio 0.8.1)

For MP3 only, calculate duration as `(uint64_t(audioDataSize) * 8) / averageBitrate`.
The old `8 * (audioDataSize / averageBitrate)` truncated before multiplication,
quantizing the estimate in eight-second steps. The wide multiplication also
avoids overflow when converting large byte counts to bits. Decoder, I2S and
other codec calculations are unchanged. VBR duration still depends on the
library's average bitrate estimate; exact container-index duration is not added.

## Reverted read-size experiment (radio 0.8.2 → 0.8.3)

The 2 KiB local MP3 read cap and additional post-render audio servicing did not
resolve the reported interruption. Both were reverted in 0.8.3. Local input
reads use the original 16 KiB limit again. The application now shows static,
ellipsized MP3 titles at the user's request. The 0.8.1 duration fix remains.


## Local MP3 seek and resume (radio 0.9.0)

- Expose the queued file position so the application can save/display a pending
  seek while paused, without using the SD read-ahead cursor as the play position.
- Accept 32-bit absolute seconds and use wide arithmetic to convert time to bytes.
  A zero-time seek uses a nonzero byte sentinel, then aligns to an MP3 frame.
- Flush decoded/output samples and DMA for an explicit seek; clear MP3 decoder
  history on applying the seek. Clear the local EOF flag when moving away from EOF.
- Calculate seek elapsed time without eight-second truncation. On reopening,
  retain the aligned byte offset until the first decoded frame gives a bitrate.
- Reset pending positions with connection defaults, assigning the supplied resume
  byte after defaults in connecttoFS. Pending state cannot leak to another source.
- Bound the MP3 sync search by EOF and 64 KiB. The previous loop could remain at
  EOF indefinitely on a seek into a trailer or corrupt data. No sync falls back
  to the audio-data start. This is frame alignment, not a complete frame index;
  VBR duration and time-based seeking remain estimates.

No decoder/library upgrade, SD write, or 16 KiB read-size change was made.
Firmware compilation and hardware checks are performed by the user.
