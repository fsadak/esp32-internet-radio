# Third-party sources and notices

Review date: **2026-10-28**. Firmware: **0.12.0**.

**Dependency licensing review remains open.** The
application's original contributions have a GPLv3 grant in AUTHORS.md. This file
does not certify that every incorporated dependency can be redistributed together
under GPLv3, and does not replace any original notices.

## ESP32-audioI2S 2.0.7

- Source: <https://github.com/esphome/ESP32-audioI2S/tree/2.0.7>.
- Original project: <https://github.com/schreibfaul1/ESP32-audioI2S>.
- Library metadata credits schreibfaul1 and jesserockz; source identifies Wolle.
- The root library license and metadata state GPLv3:
  [original license](lib/ESP32-audioI2S/LICENSE).
- Project-local changes are described in
  [LOCAL-CHANGES.md](lib/ESP32-audioI2S/LOCAL-CHANGES.md).

### Helix-origin MP3/AAC code: unresolved

The bundled `mp3_decoder` and `aac_decoder` sources explicitly identify Helix
origins. A related Helix distribution preserves RealNetworks copyright and RPSL/
RCSL terms; see the unmodified reference documents included as
[Helix notice](LICENSES/Helix-MP3-LICENSE.reference.txt) and
[RPSL reference](LICENSES/Helix-RPSL.reference.txt).

Reference source:
<https://github.com/earlephilhower/ESP8266Audio/tree/master/src/libhelix-mp3>.
That repository is provenance evidence, not proof that its licensing grant
automatically covers every file in this project's merged decoder sources.

The Free Software Foundation lists RPSL 1.0 as GPL-incompatible:
<https://www.gnu.org/licenses/license-list.html#RPSL>.
An alternative applicable grant or an explanation of the exact decoder licensing
chain has not been verified for the vendored 2.0.7 files. Their parent directory's
GPLv3 label alone has not been treated as resolving that issue.
Clarification was requested in [upstream issue #23](https://github.com/esphome/ESP32-audioI2S/issues/23).
The open issue does not establish an additional grant from decoder rights
holders or a declaration of licensing clearance.

### FLAC / Project Nayuki

The bundled FLAC decoder credits the Java implementation at
<https://www.nayuki.io/page/simple-flac-implementation> and the ESP32 adaptation
identifies Wolle. The referenced Java decoder carries Copyright (c) 2017 Project
Nayuki under MIT; its complete source-header notice is retained in
[Project-Nayuki-MIT.txt](LICENSES/Project-Nayuki-MIT.txt).

## esp32_smartdisplay 2.1.2

- Author identified by upstream metadata: Rene Zeldenthuis.
- Exact source revision:
  <https://github.com/rzeldent/esp32-smartdisplay/tree/38e9ebabe3e4044159c4e95192d432607ed70588>.
- At that revision, `library.json` says MIT, while `LICENSE` contains GPLv3.
  Both original files are preserved. This project has not replaced the upstream
  license or represented the dependency as unambiguously MIT-only.
- [Bundled license](lib/esp32_smartdisplay/LICENSE).
- [Local changes and hardware adaptation](lib/esp32_smartdisplay/LOCAL-CHANGES.md).

The radio's board definition is adapted from the working Sunton configuration
used with this library/demo ecosystem. GPIO18 is reserved for audio and GT911
interrupt polling is disabled in favor of polled touch reads. These local
adaptations do not transfer upstream authorship to fsadak.

## Espressif files in the display library

The included touch, IO-expander and panel-IO components carry Espressif Systems
copyright and `SPDX-License-Identifier: Apache-2.0` notices in their source
headers. Preserve those per-file notices. The license text is included in
[Apache-2.0.txt](LICENSES/Apache-2.0.txt), obtained from
<https://www.apache.org/licenses/LICENSE-2.0.txt>.

## LVGL 9.6.0

Source: <https://github.com/lvgl/lvgl>. The pinned dependency is downloaded by
PlatformIO. Its MIT license identifies Copyright (c) 2025 LVGL Kft; the text from
the installed 9.6.0 package is retained in [LVGL-MIT.txt](LICENSES/LVGL-MIT.txt).

## Montserrat

Font source: <https://github.com/JulietaUla/Montserrat>.
Copyright 2011 The Montserrat Project Authors. SIL Open Font License 1.1:
[Montserrat-OFL-1.1.txt](LICENSES/Montserrat-OFL-1.1.txt), also retained under
[design/fonts](design/fonts/Montserrat-OFL.txt).

Generated `radio_turkish_*.c` files contain derived font data and retain the
Montserrat attribution. They are not solely original fsadak font artwork.
The generator uses `lv_font_conv` 1.5.3; build tooling is not bundled as binaries
or node_modules.

## Build toolchain and external content

PlatformIO downloads espressif32 6.10.0 and the Arduino-ESP32 toolchain/framework;
their original licenses remain applicable. Those packages are not copied into
this source release candidate.

Station names/stream URLs identify third-party services. This project does not
grant rights to broadcasts, station trademarks, music recordings or metadata.
Example station lists do not include audio recordings.
