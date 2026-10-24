# Host-side checks / Bilgisayarda kontroller

These optional C++17 sources exercise the station CSV parser, station storage and
settings using Arduino/Preferences stubs. They do not test the real SD card, NVS,
network, audio or display. They are separate from the PlatformIO firmware build.

Bu isteğe bağlı C++17 kaynakları, Arduino/Preferences benzetimleriyle CSV ayrıştırma,
istasyon saklama ve ayarları kontrol eder. Gerçek SD/NVS, ağ, ses veya ekran
kontrolü sağlamaz; normal PlatformIO derlemesinin parçası değildir.

C++17 destekli bir derleyici kullanın. Proje kökünden örnek komut:

```sh
c++ -std=c++17 -Wall -Wextra -I tests/host -I include tests/host/test_station_import.cpp src/StationCsv.cpp src/StationStore.cpp src/RadioSettings.cpp -o station-import-check
./station-import-check
```

Windows üzerinde çıktı uzantısını ve çalıştırma biçimini derleyicinize göre uyarlayın.
`test_radio_fonts.cpp`, gerçek LVGL font kaynaklarıyla ayrıca bağlanmalıdır;
Arduino benzetimleri tek başına yeterli değildir. Donanım davranışlarını gerçek
kart üzerinde ayrıca doğrulayın.
