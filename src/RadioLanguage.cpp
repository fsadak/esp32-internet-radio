#include "RadioLanguage.h"
#include <lvgl.h>
#include <cstring>

namespace {
RadioLanguage activeLanguage = RadioLanguage::English;
struct Message {
    const char* tr; const char* en; uint8_t translateArguments;
    constexpr Message(const char* a, const char* b, uint8_t flags = 0)
        : tr(a), en(b), translateArguments(flags) {}
};
// Keys are device-owned canonical messages, never station/file/SSID metadata.
// {0}..{3} preserve dynamic values. Only marked reason arguments are translated.
const Message messages[] = {
    {"Ekran", "Display"}, {"Ses", "Sound"}, {"Radyo", "Radio"},
    {"İstasyonlar", "Stations"}, {"Cihaz", "Device"},
    {"SD MP3", "SD MP3"}, {"İSTASYON", "STATION"}, {"SES KAYNAĞI", "AUDIO SOURCE"},
    {"SD KART", "SD CARD"}, {"ŞİMDİ ÇALIYOR", "NOW PLAYING"}, {"SES", "VOLUME"},
    {"İSTASYON LİSTESİ", "STATION LIST"}, {"Dinlemek için bir istasyona dokunun", "Tap a station to listen"},
    {"Tekrar: 1", "Repeat: one"}, {"Tekrar: tüm", "Repeat: all"},
    {LV_SYMBOL_AUDIO "  İNTERNET RADYOSU", LV_SYMBOL_AUDIO "  INTERNET RADIO"},
    {LV_SYMBOL_SHUFFLE " Karışık", LV_SYMBOL_SHUFFLE " Shuffle"},
    {LV_SYMBOL_PREV "  Önceki", LV_SYMBOL_PREV "  Previous"},
    {"Sonraki  " LV_SYMBOL_NEXT, "Next  " LV_SYMBOL_NEXT},
    {LV_SYMBOL_LIST "  İstasyon listesi", LV_SYMBOL_LIST "  Station list"},
    {LV_SYMBOL_LIST "  Dosya listesi", LV_SYMBOL_LIST "  File list"},
	{LV_SYMBOL_WIFI "  Bağlı", LV_SYMBOL_WIFI "  Connected"},
	{LV_SYMBOL_WIFI "  Bekliyor", LV_SYMBOL_WIFI "  Waiting"},
	{LV_SYMBOL_PAUSE " Duraklat", LV_SYMBOL_PAUSE " Pause"},
	{LV_SYMBOL_PLAY " Oynat", LV_SYMBOL_PLAY " Play"},
	{LV_SYMBOL_STOP " Durdur", LV_SYMBOL_STOP " Stop"},
	{"SD KARTTAN İSTASYON LİSTESİ", "STATION LIST FROM SD CARD"},
    {"Yeniden tara", "Rescan"}, {"Vazgeç", "Cancel"}, {"Listeyi değiştir", "Replace list"},
    {"Kaydet", "Save"}, {"Geri", "Back"}, {"Düz", "Flat"},
    {LV_SYMBOL_SETTINGS "  AYARLAR", LV_SYMBOL_SETTINGS "  SETTINGS"},
    {"Karartma süresi", "Display timeout"},
    {"Favoriler", "Favorites"}, {"FAVORİLER", "FAVORITES"},
    {"Sil", "Delete"}, {"İstasyon silinsin mi?", "Delete this station?"},
    {"Yayına ulaşılamadı. Bu durum geçici olabilir.", "Unable to reach the stream. This may be temporary."},
    {"Silme kalıcıdır. Çalan istasyon silinirse yayın durur.", "Deletion is permanent. Deleting the playing station stops playback."},
    {"Son istasyon silinemez. Önce başka bir istasyon ekleyin.", "The last station cannot be deleted. Add another station first."},
    {"İstasyon listesi değişti. Vazgeç ile kapatıp yeniden seçin.", "The station list changed. Close with Cancel and select again."},
    {"İstasyon silindi; başka bir istasyon seçin.", "Station deleted; select another station."},
    {"Henüz favori istasyon yok. Kalp simgesine dokunarak ekleyin.", "No favorite stations yet. Tap the heart to add one."},
    {"Favoriler kaydedilemedi. Önceki favoriler korundu.", "Unable to save favorites. Previous favorites were preserved."},
    {"VU metre", "VU meter"},
    {"Kapalı\n30 saniye\n1 dakika\n2 dakika\n5 dakika\n10 dakika", "Off\n30 seconds\n1 minute\n2 minutes\n5 minutes\n10 minutes"},
    {"Süre dolunca %50 parlaklık; 15 sn sonra arka ışık kapanır.", "Timeout: 50% brightness; backlight off 15 seconds later."},
    {"İlk dokunuş yalnızca ekranı uyandırır.", "The first touch only wakes the display."},
    {"Son ses seviyesini hatırla", "Remember last volume"},
    {"Ekolayzer", "Equalizer"}, {"EKOLAYZER", "EQUALIZER"},
    {"Bas", "Bass"}, {"Orta", "Mid"}, {"Tiz", "Treble"},
    {"Açılışta otomatik yayın", "Play radio on startup"},
    {"Son istasyonu hatırla", "Remember last station"}, {"Açılış istasyonu", "Startup station"},
    {"CSV dosyası seçerek tüm istasyon listesini değiştirin.", "Select a CSV file to replace the entire station list."},
    {LV_SYMBOL_SD_CARD "  SD karttan liste yükle", LV_SYMBOL_SD_CARD "  Import list from SD card"},
    {"Liste cihaza kaydedilir; SD kart olmadan da korunur.", "The list is stored on the device; the SD card is not needed."},
    {"Wi-Fi ağını ayarla / değiştir", "Set up / change Wi-Fi"},
    {"Değişiklikleri deneyin, sonra Kaydet veya Vazgeç.", "Try your changes, then Save or Cancel. Language applies on Save."},
    {"Bağlı", "Connected"}, {"Bağlı değil", "Disconnected"},
    {"Web: Wi-Fi bağlantısı gerekli", "Web: Wi-Fi connection required"},
    {"WI-FI KURULUMU", "WI-FI SETUP"},
    {"2,4 GHz ağ seçin veya gizli ağın adını yazın.", "Select a 2.4 GHz network or enter a hidden network name."},
    {"Ağları taramak için Tara'ya dokunun", "Tap Scan to find networks"},
    {"Tara", "Scan"}, {"Ağ adı (SSID)", "Network name (SSID)"},
    {"Şifre (açık ağda boş bırakın)", "Password (leave blank for open networks)"},
    {"Bağlan ve kaydet", "Connect and save"}, {"Kapat / denemeyi iptal et", "Close / cancel connection"},
    {"Ağ seçin...", "Select a network..."},
    {"Liste boş; Tara veya elle ağ adı girişi", "No networks; Scan or enter the network name"},
    {"SD MÜZİK", "SD MUSIC"}, {"Üst klasör", "Parent folder"}, {"Radyoya dön", "Back to radio"},
    {"Wi-Fi bağlanıyor...", "Connecting to Wi-Fi..."}, {"Yayın hazır", "Stream ready"},
    {"Yayın başlatılamadı", "Unable to start stream"}, {"Wi-Fi bekleniyor", "Waiting for Wi-Fi"},
    {"Yayın açılıyor...", "Opening stream..."}, {"Yayın bağlantısı kurulamadı", "Unable to connect to stream"},
    {"Yayın bağlantısı kesildi", "Stream disconnected"}, {"Yayın yanıt vermedi", "Stream did not respond"},
    {"Dosya açılamadı veya boş. SD kartı kontrol edin.", "File unavailable or empty. Check the SD card."},
    {"MP3 açılıyor...", "Opening MP3..."}, {"SD müzik çalıyor", "Playing SD music"},
    {"MP3 açılamadı; dosyayı veya kartı kontrol edin.", "Unable to open MP3; check the file or SD card."},
    {"Liste tamamlandı", "Playlist finished"}, {"Duraklatıldı", "Paused"}, {"Durduruldu", "Stopped"},
    {"SD kart çıkarıldı veya okunamadı.", "SD card removed or unreadable."},
    {"MP3 çalınamadı; başka dosya seçin.", "Unable to play MP3; select another file."},
    {"Dosya okunması ilerlemiyor; SD kartı kontrol edin.", "File reading stalled; check the SD card."},
    {"SD kart taranıyor...", "Scanning SD card..."},
    {"Bu klasörde MP3 veya alt klasör yok.", "No MP3 files or subfolders in this folder."},
    {"Klasöre girmek veya çalmak için dokunun.", "Tap a folder to browse or a file to play."},
    {"Dosya açılamadı veya boş.", "File unavailable or empty."},
    {"Son MP3 açılamadı; SD kartı kontrol edin.", "Unable to open last MP3; check the SD card."},
    {"Son MP3 bulunamadı; listeden dosya seçin.", "Last MP3 not found; select a file from the list."},
    {"Son MP3 okunamadı; dosya seçin.", "Last MP3 unreadable; select a file."},
    {"Wi-Fi bağlantısı bekleniyor...", "Waiting for Wi-Fi connection..."},
    {"CSV yüklemek için önce Radyo kaynağına dönün.", "Switch to Radio before importing a CSV list."},
    {"Yüklemek istediğiniz CSV dosyasını seçin.\nYalnızca SD kartın kök dizini listelenir.", "Select a CSV file to import.\nOnly the SD card root directory is listed."},
    {"Dosya okunuyor ve doğrulanıyor...", "Reading and validating file..."},
    {"Yeni liste hazır; istasyon seçin", "New list ready; select a station"},
    {"Açılış seçimi kaydedilemedi; ilk istasyon kullanılır.", "Startup selection could not be saved; the first station will be used."},
    {"Kaydedilemedi. Tekrar deneyin veya Vazgeç.", "Unable to save. Try again or Cancel."},
    {"Yayın durduruldu", "Radio stopped"}, {"Başlatmak için istasyon seçin", "Select a station to start"},
    {"Yayın bekliyor", "Radio idle"}, {"Mevcut liste korundu.", "The existing list was preserved."},
    {"İlk kullanım: ağınızı seçip şifrenizi girin.", "First use: select your network and enter its password."},
    {"Kayıtlı ağa bağlanılıyor...", "Connecting to saved network..."},
    {"Ağ seçin veya ağ adını elle girin. Şifreyi yeniden yazın.", "Select a network or enter its name. Re-enter the password."},
    {"Önce bağlantı denemesinin tamamlanmasını bekleyin.", "Wait for the current connection attempt to finish."},
    {"Ağlar taranıyor...", "Scanning networks..."},
    {"Tarama tamamlanamadı. Tara'ya dokunun veya ağ adını yazın.", "Scan failed. Tap Scan or enter the network name."},
    {"Bir bağlantı denemesi sürüyor.", "A connection attempt is in progress."},
    {"Ağ adı boş olamaz ve en fazla 32 bayt olabilir.", "Network name is required and must not exceed 32 bytes."},
    {"Şifre 8–63 karakter veya 64 onaltılık hane olmalı. Açık ağda boş bırakın.", "Password must be 8-63 characters or 64 hex digits. Leave blank for an open network."},
    {"Bağlanılıyor... Başarılı olursa kaydedilecek.", "Connecting... Settings will be saved if successful."},
    {"Ağ bulunamadı. Yeniden tarayın veya ağ adını yazın.", "No networks found. Scan again or enter the network name."},
    {"Ağ seçin, şifreyi girin; Bağlan ve kaydet'e dokunun.", "Select a network, enter its password, then tap Connect and save."},
    {"Eski bağlantı kapatılamadı; tekrar deneyin.", "Unable to close the previous connection; try again."},
    {"Ağ bağlantısı yenilenemedi. Wi-Fi ayarlarını kontrol edin.", "Unable to renew connection. Check Wi-Fi settings."},
    {"Bağlantı kuruldu ama bilgiler kaydedilemedi.", "Connected, but settings could not be saved."},
    {"Bağlandı ve kaydedildi. Kapat ile radyoya dönebilirsiniz.", "Connected and saved. Close this screen to return."},
    {"Kayıtlı ağa bağlandı.", "Connected to saved network."},
    {"Bağlanılamadı. Ağ adını ve şifreyi kontrol edin.", "Connection failed. Check network name and password."},
    {"Önceki ağ da erişilebilir değil; kayıt korundu. Yeni bilgiler deneyebilirsiniz.", "Previous network is also unavailable; saved settings were preserved. Try another network."},
    {"Kayıtlı ağa bağlanılamadı. Ağı ve şifreyi kontrol edin.", "Unable to connect to saved network. Check network and password."},
    {"SD kart yok veya okunamadı. Kartı takıp tekrar deneyin.", "SD card missing or unreadable. Insert it and try again."},
    {"Geçersiz klasör yolu.", "Invalid folder path."}, {"SD dizini açılamadı. Kartı kontrol edin.", "Unable to open SD directory. Check the card."},
    {"Bir dizinde en fazla 512 kayıt taranabilir.", "A directory scan supports at most 512 entries."},
    {"Dosya/klasör yolu geçersiz veya 240 bayttan uzun.", "Invalid file/folder path or path longer than 240 bytes."},
    {"Bir klasörde en fazla 128 MP3/alt klasör listelenebilir.", "A folder supports at most 128 MP3 files/subfolders."},
    {"SD kartın kök dizini açılamadı.", "Unable to open the SD card root directory."},
    {"Kök dizinde çok fazla kayıt var (en fazla 512).", "Too many entries in the root directory (maximum 512)."},
    {"En fazla 64 CSV dosyası listelenebilir.", "At most 64 CSV files can be listed."},
    {"Kök dizinde .csv dosyası bulunamadı.", "No .csv files found in the root directory."},
    {"Geçersiz dosya yolu.", "Invalid file path."}, {"Dosya açılamadı. SD kartı kontrol edin.", "Unable to open file. Check the SD card."},
    {"Dosya 12 KB sınırını aşıyor.", "File exceeds the 12 KB limit."},
    {"Dosya tam okunamadı. Mevcut liste korundu.", "File could not be fully read. The existing list was preserved."},
    {"Kalıcı bellek açılamadı. Mevcut liste korundu.", "Unable to open persistent storage. The existing list was preserved."},
    {"Liste kaydedilemedi. Mevcut liste korundu.", "Unable to save list. The existing list was preserved."},
    {"Satır çok uzun.", "Line is too long."},
    {"İki sütun gerekli: name;url. Tırnakları kontrol edin.", "Two columns required: name;url. Check the quotation marks."},
    {"İlk satır name;url olmalı.", "The first line must be name;url."},
    {"İsim boş veya 96 bayttan uzun.", "Name is empty or longer than 96 bytes."},
    {"İsim UTF-8 olmalı; kontrol karakteri kullanılamaz.", "Name must be UTF-8 without control characters."},
    {"Geçerli http/https yayın adresi gerekli (en fazla 512 bayt).", "A valid http/https stream URL is required (maximum 512 bytes)."},
    {"Aynı yayın adresi tekrar ediyor.", "Duplicate stream URL."},
	{"En fazla 128 istasyon yüklenebilir.", "At most 128 stations can be imported."},
	{"Dosyada en az bir istasyon olmalı.", "The file must contain at least one station."},
	{"SD kartı çıkarabilirsiniz.", "You can remove the SD card."},
	{"Yeni listede ilk istasyon seçildi.", "The first station in the new list was selected."},
	{"FIRMWARE GÜNCELLEME", "FIRMWARE UPDATE"},
	{"Gücü kesmeyin veya cihazı kapatmayın.", "Do not disconnect power or turn off the device."},
	{"Firmware güncellemesi", "Firmware update"},
	{"Güncelleme başlatıldı.", "Update started."},
	{"Güncelleme doğrulandı. Yeniden başlatılıyor...", "Update verified. Restarting..."},
	{"Güncelleme tamamlandı. Yeniden başlatılıyor...", "Update complete. Restarting..."},
	{"Güncelleme kesildi. Yeniden deneyin; oynatmayı Başlat ile sürdürün.", "Update interrupted. Try again; press Play to resume playback."},
	{"Başka bir güncelleme sürüyor.", "Another update is already in progress."},
	{"Firmware boyutu uygun değil veya OTA alanı bulunamadı.", "Invalid firmware size or OTA partition unavailable."},
	{"Güncelleme alanı açılamadı.", "Unable to open update partition."},
	{"Güncelleme verisi beklenen boyutta değil.", "Update data does not match the expected size."},
	{"Bu dosya uygun ESP32-S3 uygulama firmware'i değil. firmware.bin dosyasını seçin.", "Not a valid ESP32-S3 application image. Select firmware.bin."},
	{"Firmware yazılamadı. Mevcut sürüm korunuyor.", "Unable to write firmware. The current version was preserved."},
	{"Dosya eksik veya bu radyo projesine ait değil.", "Incomplete file or incompatible firmware image."},
	{"Firmware bütünlük doğrulaması başarısız. Mevcut sürüm korunuyor.", "Firmware integrity check failed. The current version was preserved."},
	{"Yeni firmware etkinleştirilemedi. Mevcut sürüm korunuyor.", "Unable to activate new firmware. The current version was preserved."},
	// Formatted messages preserve values, and explicitly translate nested reasons.
	{"{0} istasyon kalıcı belleğe kaydedildi.", "{0} stations saved to persistent storage."},
	{"Firmware yükleniyor: %{0}", "Uploading firmware: {0}%"},
	{"Tür: {0}", "Genre: {0}"}, {"Tür: ", "Genre: "}, {"Süre: ", "Time: "},
    {"Sayfa {0} / {1}", "Page {0} / {1}"},
    {"{0} / {1} parça", "{0} / {1} tracks"}, {"{0} / {1} istasyon", "{0} / {1} stations"},
    {"Parlaklık: %{0}", "Brightness: {0}%"},
    {"Açılış ses seviyesi: {0}", "Startup volume: {0}"}, {"Azami ses seviyesi: {0}", "Maximum volume: {0}"},
    {"Wi-Fi: {0}", "Wi-Fi: {0}", 1}, {"Sinyal: {0} dBm", "Signal: {0} dBm"}, {"Yazılım: {0}", "Firmware: {0}"},
    {"Dosya doğrulandı: {0} istasyon.\nMevcut istasyon listesi değiştirilecek.\nYeni liste kalıcı bellekte saklanacak.\nOnaylıyor musunuz?", "File validated: {0} stations.\nThe existing station list will be replaced.\nThe new list will be stored on the device.\nContinue?"},
    {"{0} istasyon kalıcı belleğe kaydedildi.\nSD kartı çıkarabilirsiniz.\nYeni listede ilk istasyon seçildi.", "{0} stations saved to persistent storage.\nYou can remove the SD card.\nThe first station in the new list was selected."},
    {"HTTP {0}: yayın isteği reddedildi", "HTTP {0}: stream request rejected"},
    {"{0}; listeden yeniden seçin", "{0}; select again from the list", 1},
    {"{0}: {1} sn sonra tekrar", "{0}: retry in {1} seconds", 1},
    {"Yeniden bağlantı {0}/{1}", "Reconnecting {0}/{1}"},
    {"{0} Önceki kayıt korundu; eski ağa dönülüyor.", "{0} Previous settings preserved; reconnecting to the previous network.", 1},
    {"{0} Eski ağa bağlandı.", "{0} Connected to the previous network.", 1},
    {"Satır {0}: {1}", "Line {0}: {1}", 2},
};

// Whole-message templates only: no word replacement inside arbitrary metadata.
bool matchMessage(const String& input, const char* pattern, String (&args)[4]) {
    size_t pos = 0;
    while (*pattern) {
        if (pattern[0] == '{' && pattern[1] >= '0' && pattern[1] <= '3' && pattern[2] == '}') {
            const uint8_t index = pattern[1] - '0'; pattern += 3;
            const char* endLiteral = strchr(pattern, '{');
            const String delimiter = endLiteral ? String(pattern).substring(0, endLiteral - pattern) : String(pattern);
            int end = delimiter.length() ? input.indexOf(delimiter, pos) : input.length();
            if (end < 0) return false;
            args[index] = input.substring(pos, end); pos = end;
        } else {
            if (pos >= input.length() || input[pos++] != *pattern++) return false;
        }
    }
    return pos == input.length();
}

String translate(const String& input, uint8_t depth) {
    if (depth > 5 || input.isEmpty()) return input;
    for (const auto& message : messages)
        if (!strchr(message.tr, '{') && input == message.tr) return String(message.en);
    for (const auto& message : messages) {
        if (!strchr(message.tr, '{')) continue;
        String args[4];
        if (!matchMessage(input, message.tr, args)) continue;
        for (uint8_t i = 0; i < 4; ++i)
            if (message.translateArguments & (1 << i)) args[i] = translate(args[i], depth + 1);
        String result;
        for (const char* p = message.en; *p;) {
            if (p[0] == '{' && p[1] >= '0' && p[1] <= '3' && p[2] == '}') {
                result += args[p[1] - '0']; p += 3;
            } else result += *p++;
        }
        return result;
    }
    // Composed device/error panels contain independently localizable lines.
    if (input.indexOf('\n') >= 0) {
        String result; int begin = 0;
        while (begin <= int(input.length())) {
            int end = input.indexOf('\n', begin); if (end < 0) end = input.length();
            if (begin) result += '\n';
            result += translate(input.substring(begin, end), depth + 1);
            begin = end + 1;
        }
        return result;
    }
    return input;
}
}

void setRadioLanguage(RadioLanguage language) { activeLanguage = language; }
RadioLanguage radioLanguage() { return activeLanguage; }
String uiText(const String& canonical) {
    return activeLanguage == RadioLanguage::Turkish ? canonical : translate(canonical, 0);
}
