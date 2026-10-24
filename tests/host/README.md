# Host-side station-list checks / İstasyon listesi kontrolleri

These are optional test sources, not part of the normal PlatformIO firmware build.
They have not been rerun for the 0.10.0 publication candidate. A C++17 compiler
and the local stubs are required. The historical Zig commands below refer to
private development tools that are not distributed.

Bu kaynaklar normal PlatformIO firmware derlemesinin parçası değildir.
0.10.0 yayın adayı için yeniden çalıştırılmamıştır. C++17 derleyicisi ve yerel
benzetimleri kullanın. Aşağıdaki geçmiş Zig komutları dağıtıma dahil edilmeyen
özel geliştirme araçlarına atıf yapar; bu araçları paket içinde aramayın.

Firmware'in gerçek `StationCsv.cpp`, `StationStore.cpp` ve `RadioSettings.cpp`
dosyaları bilgisayarda derlenir. Arduino String ve Preferences için küçük test
benzetimleri kullanılır; gerçek SD/flash veya ekran testi yapılmaz.

Windows'ta kullanılan komut (Zig 0.13.0, sadece proje içindeki test aracı):

```powershell
& .\display-test\host-tools\ziglang\zig.exe c++ -target x86_64-windows-gnu -std=c++17 -Wall -Wextra -I tests/host -I include tests/host/test_station_import.cpp src/StationCsv.cpp src/StationStore.cpp src/RadioSettings.cpp -o display-test/test-station-import.exe
& .\display-test\test-station-import.exe
```

Standart C++17 derleyicisiyle de aynı kaynaklar kullanılabilir. Testler UTF-8/BOM,
satır sonları, tırnaklama, hatalı/sınır dosyalar, yinelenen URL, başarısız ayrıştırmada
mevcut listenin korunması, tüm listeyi değiştirme, yeniden açılış, NVS kayıt hatası
ve farklı listede eski istasyon indekslerinin kullanılmamasını kapsar.
