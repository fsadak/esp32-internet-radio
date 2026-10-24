# Türkiye internet radyo kataloğu

Kontrol tarihi: 2026-10-20T04:10:37.292701+03:00. **222 farklı kanal**: 190 doğrudan MP3/AAC, 32 HLS.

Bu katalog Türkiye merkezli radyo ve internet kanallarına yönelik bir seçkidir; tüm Türkiye radyolarını kapsadığı iddia edilmez. Ulusal, yerel, üniversite ve tematik yayınları içerir. Grup başlıkları dosya düzenidir; kesin tür veya karasal kapsama sınıflandırması değildir.

Doğrudan yayınlarda bilgisayardan kısa bir örnekte art arda MP3/AAC çerçeveleri kontrol edildi. HLS grubunda ana/alt oynatma listesi ve kısa medya parçası alındı. Bu kanalların tamamı ESP32 üzerinde denenmedi; uzun süreli dinleme, ses içeriği veya kesintisiz erişim garantisi verilmez. Bağlantılar zamanla değişebilir.

Bit hızı gerçek zamanlı ölçüm değildir: varsa akış sunucusunun başlığından, yoksa dizin bildiriminden alınır. Boş bırakılan yerlerde tahmin yapılmaz. Kaynak adları, yayın bildirimi ve adresler karşılaştırıldı; görülen yanlış eşleşmeler hazır CSV’lerden çıkarıldı. Kimlik kontrolü kısa metadata incelemesidir; tüm yayınların ses içeriği dinlenmedi.

## Dosyalar

| Dosya | İçerik | İstasyon |
|---|---|---:|
| [01_tr_secme_32.csv](sd-listeleri/01_tr_secme_32.csv) | Seçme 32 istasyon | 32 |
| [02_tr_pop_slow_1.csv](sd-listeleri/02_tr_pop_slow_1.csv) | Karma, pop ve slow — bölüm 1 | 32 |
| [02_tr_pop_slow_2.csv](sd-listeleri/02_tr_pop_slow_2.csv) | Karma, pop ve slow — bölüm 2 | 5 |
| [03_tr_turku_arabesk.csv](sd-listeleri/03_tr_turku_arabesk.csv) | Türkü, arabesk, özgün ve nostalji | 29 |
| [04_tr_alternatif_caz.csv](sd-listeleri/04_tr_alternatif_caz.csv) | Alternatif, rock, caz ve elektronik | 26 |
| [05_tr_haber_spor.csv](sd-listeleri/05_tr_haber_spor.csv) | Haber, spor ve sohbet | 15 |
| [06_tr_dini.csv](sd-listeleri/06_tr_dini.csv) | Dini ve manevi yayınlar | 17 |
| [07_tr_yerel_universite_1.csv](sd-listeleri/07_tr_yerel_universite_1.csv) | Yerel ve üniversite radyoları — bölüm 1 | 32 |
| [07_tr_yerel_universite_2.csv](sd-listeleri/07_tr_yerel_universite_2.csv) | Yerel ve üniversite radyoları — bölüm 2 | 15 |
| [08_tr_power.csv](sd-listeleri/08_tr_power.csv) | Power ve PowerTürk tematik kanalları | 19 |
| [90_tr_hls_deneme.csv](hls-deneme/90_tr_hls_deneme.csv) | HLS deneme listesi | 32 |

Seçme 32 listesi diğer gruplardan derlenmiştir; toplam farklı kanal sayısına ikinci kez eklenmez. Her CSV yüklemesi cihazdaki listenin tamamını değiştirir. Cihazda tek seferde 32 kanal sınırı vardır.

## Karma, pop ve slow (37)

| No | İstasyon | Biçim | Bildirilen bit hızı | Adres | Kaynak |
|---:|---|---|---|---|---|
| 1 | Slow Türk | AAC (ADTS) | Belirtilmedi | [Yayın](https://radyo.duhnet.tv/ak_dtvh_slowturk) | [Kaynak](https://www.radio-browser.info/history/97f3bb39-24a0-461d-b282-b0ccae5a12c4) |
| 2 | Radyo 7 | MP3 | 128 kbps (akış başlığı) | [Yayın](http://46.20.3.250/;stream) | [Kaynak](https://www.radio-browser.info/history/df369586-40fc-11e9-aa55-52543be04c81) |
| 3 | Süper FM | MP3 | 64 kbps (akış başlığı) | [Yayın](https://playerservices.streamtheworld.com/api/livestream-redirect/SUPER_FM_SC) | [Kaynak](https://www.radio-browser.info/history/706def0c-7017-4601-8305-db1cdb2e9420) |
| 4 | Radyo Fenomen | AAC (ADTS) | 56 kbps (akış başlığı) | [Yayın](https://live.radyofenomen.com/fenomen/128/icecast.audio) | [Kaynak](https://www.radio-browser.info/history/b3e6320b-0ffc-4d4e-a999-2467897b17e6) |
| 5 | JoyTürk | MP3 | 64 kbps (akış başlığı) | [Yayın](https://playerservices.streamtheworld.com/api/livestream-redirect/JOY_TURK_SC) | [Kaynak](https://www.radio-browser.info/history/2a372113-4b74-41aa-a1e8-3dcb5d66de61) |
| 6 | Radyo 45'lik | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://stream.radyo45lik.com:4545/) | [Kaynak](https://www.radio-browser.info/history/585600d9-bdc4-4a2e-93c5-2421041cc55c) |
| 7 | Alem FM | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://turkmedya.radyotvonline.net/alemfmaac) | [Kaynak](https://www.radio-browser.info/history/a96f83d1-d225-4eda-91cd-1a99dc230032) |
| 8 | Pal Station | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](http://shoutcast.radyogrup.com:1020/) | [Kaynak](https://www.radio-browser.info/history/0edc51ea-fc6b-4ab8-a687-8dde4730007b) |
| 9 | Kalp FM | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://yayin.kalpfm.com:8080/) | [Kaynak](https://www.radio-browser.info/history/fc04c24b-1a01-4ec6-b7d3-64eba89e075f) |
| 10 | Show Radyo | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](http://showradyo.radyotvonline.net/showradyoaac/) | [Kaynak](https://www.radio-browser.info/history/7f1ef327-ff1c-4027-b679-59e5cf2739c6) |
| 11 | TRT FM | AAC (ADTS) | 129 kbps (akış başlığı) | [Yayın](https://trt.radyotvonline.net/trtfm) | [Kaynak](https://www.radio-browser.info/history/0a2c3506-a136-40f2-b008-38daf2b12fd6) |
| 12 | Baba Radyo | MP3 | 128 kbps (akış başlığı) | [Yayın](http://officialbabaradyo.radyotvonline.net/babaofficial) | [Kaynak](https://www.radio-browser.info/history/9a7f14e6-2826-47e4-8008-d54af25cf0d8) |
| 13 | Radyo Alaturka | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://yayin.jumboserver.net:9100/stream) | [Kaynak](https://www.radio-browser.info/history/3dfe8d72-9ba4-47f2-9f57-a16cd5b273df) |
| 14 | FG 93.8 | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://edge1.radyotvonline.net/shoutcast/play/fg) | [Kaynak](https://www.radio-browser.info/history/f0be7682-2aab-45b6-b66a-d4380897a6bc) |
| 15 | Virgin Radio Türkiye | MP3 | 128 kbps (akış başlığı) | [Yayın](https://playerservices.streamtheworld.com/api/livestream-redirect/VIRGIN_RADIO_ITUNES.mp3) | [Kaynak](https://www.radio-browser.info/history/82c18698-9cf7-4bf3-a737-998a47b02407) |
| 16 | Radio Mydonose | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://playerservices.streamtheworld.com/api/livestream-redirect/RADIO_MYDONOSE128AAC.aac) | [Kaynak](https://www.radio-browser.info/history/926a0629-3096-4755-a3b3-031e78bef5c1) |
| 17 | Radyo Voyage | AAC (ADTS) | 63 kbps (akış başlığı) | [Yayın](http://voyagewmp.radyotvonline.com/;) | [Kaynak](https://www.radio-browser.info/history/fa9d480d-3db9-49c6-a801-c0f5301b9072) |
| 18 | Radyo ODTÜ | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](http://stream.radyoodtu.com.tr/;) | [Kaynak](https://www.radio-browser.info/history/7ccb7f82-7dd6-11e8-b68b-52543be04c81) |
| 19 | Radyo Eksen | MP3 | 224 kbps (akış başlığı) | [Yayın](http://eksenwmp.radyotvonline.com/;stream.mp3) | [Kaynak](https://www.radio-browser.info/history/961fbfc8-0601-11e8-ae97-52543be04c81) |
| 20 | Max FM | MP3 | 128 kbps (akış başlığı) | [Yayın](http://live.radyotvonline.com:8090/;) | [Kaynak](https://www.radio-browser.info/history/b1443978-7de0-11e8-b68b-52543be04c81) |
| 21 | Borusan Klasik | AAC (ADTS) | 64 kbps (akış başlığı) | [Yayın](https://playerservices.streamtheworld.com/api/livestream-redirect/BORUSAN_KLASIKAAC_SC) | [Kaynak](https://www.radio-browser.info/history/ccfd28c8-9f0a-11e9-a787-52543be04c81) |
| 22 | JoyTürk Akustik | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](http://playerservices.streamtheworld.com/api/livestream-redirect/JOYTURK_AKUSTIK128AAC.aac) | [Kaynak](https://www.radio-browser.info/history/a7dc38d4-920a-4de2-8fac-5106abccf69e) |
| 23 | Slow Time | MP3 | 112 kbps (akış başlığı) | [Yayın](http://46.20.7.103:8040/;) | [Kaynak](https://www.radio-browser.info/history/3bc2e65e-76da-4b32-89f6-0fde42ec19c0) |
| 24 | PowerTürk Slow | AAC (ADTS) | 250 kbps (akış başlığı) | [Yayın](https://listen.powerapp.com.tr/powerturkslow/mpeg/icecast.audio) | [Kaynak](https://www.radio-browser.info/history/e2091ce6-2152-4fea-ac8b-45b19adda1c7) |
| 25 | Küpe FM | AAC (ADTS) | 192 kbps (akış başlığı) | [Yayın](https://icecast.netmedya.net/kupefm) | [Kaynak](https://www.radio-browser.info/history/c745e25e-0af7-431b-a0c2-ee20bd695893) |
| 26 | Metro FM | AAC (ADTS) | 64 kbps (akış başlığı) | [Yayın](https://playerservices.streamtheworld.com/api/livestream-redirect/METRO_FMAAC_SC) | [Kaynak](https://www.radio-browser.info/history/434b73f8-2fc5-40e5-af3c-254b6d603d08) |
| 27 | Pop 90 | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://playerservices.streamtheworld.com/api/livestream-redirect/POP90128AAC.aac) | [Kaynak](https://www.radio-browser.info/history/75c5ca67-d613-4a9f-8bcf-eb69473153f9) |
| 28 | Efkar | MP3 | 64 kbps (akış başlığı) | [Yayın](https://playerservices.streamtheworld.com/api/livestream-redirect/EFKAR.mp3) | [Kaynak](https://www.radio-browser.info/history/5159b1df-7f85-4b67-a08d-953431d11c27) |
| 29 | Mydonose Türk | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://playerservices.streamtheworld.com/api/livestream-redirect/MYDONOSE_TURK128AAC.aac) | [Kaynak](https://www.radio-browser.info/history/7ee088e7-375f-4f1f-bd60-55a3606e9d3f) |
| 30 | Radyo Viva | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://edge1.radyotvonline.net/shoutcast/play/radyoviva) | [Kaynak](https://www.radio-browser.info/history/989cc154-83bc-4f61-9b42-2ecb3b3739ed) |
| 31 | Kıyı Müzik | MP3 | 320 kbps (akış başlığı) | [Yayın](https://kiyi.radioca.st/stream) | [Kaynak](https://www.radio-browser.info/history/0377dd73-ac4e-4ab4-88ae-ac81c70676be) |
| 32 | Retro Türk | AAC (ADTS) | 64 kbps (akış başlığı) | [Yayın](https://playerservices.streamtheworld.com/api/livestream-redirect/RETROTURKAAC_SC) | [Kaynak](https://www.radio-browser.info/history/24a3740f-4a8e-46e5-8b39-c8b4a2730ded) |
| 33 | Dream Türk | AAC (ADTS) | Belirtilmedi | [Yayın](https://radyo.duhnet.tv/dreamturk) | [Kaynak](https://www.radio-browser.info/history/eea9a230-f288-4312-834e-ac3914f533ca) |
| 34 | Radyo D | AAC (ADTS) | Belirtilmedi | [Yayın](https://radyo.duhnet.tv/radyod) | [Kaynak](https://www.radio-browser.info/history/e5e4f77a-3821-45b3-9d59-14f5d8b83ab4) |
| 35 | Joy FM | AAC (ADTS) | 64 kbps (akış başlığı) | [Yayın](https://playerservices.streamtheworld.com/api/livestream-redirect/JOY_FMAAC_SC) | [Kaynak](https://www.radio-browser.info/history/c7dadc87-3c43-46f5-aa24-91e0fa9a7643) |
| 36 | Kiss Türk Slow | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](http://dijimedya.radyotvonline.net/turkslow) | [Kaynak](https://www.radio-browser.info/history/40ad551a-86df-4c4b-91d5-22aee0b6d57a) |
| 37 | Hayatmix Slow | MP3 | 128 kbps (akış başlığı) | [Yayın](https://radyo.yayin.com.tr:4090/mp3) | [Kaynak](https://www.radio-browser.info/history/bef9a5a3-997a-42ed-a480-f0302253b7d5) |

## Türkü, arabesk, özgün ve nostalji (29)

| No | İstasyon | Biçim | Bildirilen bit hızı | Adres | Kaynak |
|---:|---|---|---|---|---|
| 38 | Arabesk FM | AAC (ADTS) | 64 kbps (akış başlığı) | [Yayın](http://yayin.arabeskfm.biz:8042/) | [Kaynak](https://www.radio-browser.info/history/96201b3d-0601-11e8-ae97-52543be04c81) |
| 39 | DamarTürk FM | MP3 | 128 kbps (akış başlığı) | [Yayın](https://live.radyositesihazir.com:10997/) | [Kaynak](https://www.radio-browser.info/history/f3dfbec0-8ddb-457c-94cd-368631337c0f) |
| 40 | Türkülerle Türkiye | MP3 | 128 kbps (akış başlığı) | [Yayın](http://37.247.98.8/stream/22/;) | [Kaynak](https://www.radio-browser.info/history/360028f5-46ba-11ea-a860-52543be04c81) |
| 41 | Ankara Havaları | MP3 | 128 kbps (akış başlığı) | [Yayın](http://37.247.98.8/stream/30/;) | [Kaynak](https://www.radio-browser.info/history/8c01f241-6fe8-42b1-afd3-d6abf90ec78e) |
| 42 | Radyo Şiran | MP3 | 128 kbps (akış başlığı) | [Yayın](https://live.radyositesihazir.com/8078/stream) | [Kaynak](https://www.radio-browser.info/history/1e0dcee9-bfb6-41a4-8356-e8c4540dfcf0) |
| 43 | Radyo Seymen | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://yayin.radyoseymen.com.tr:1070/stream) | [Kaynak](https://www.radio-browser.info/history/de28a78a-6cee-453d-b9df-13557334dfdb) |
| 44 | Altın Şarkılar | MP3 | 128 kbps (akış başlığı) | [Yayın](http://37.247.98.8/stream/25/;) | [Kaynak](https://www.radio-browser.info/history/31da019f-9f0a-11e9-a787-52543be04c81) |
| 45 | Türkü Radyo (turkuradyo.net) | MP3 | 64 kbps (akış başlığı) | [Yayın](http://radyo.turkuradyo.net:4591/turkuradyo) | [Kaynak](https://www.radio-browser.info/history/8b4be047-aeac-4ccf-8eb5-35bc2f9740df) |
| 46 | Radyo Ekin | AAC (ADTS) | 160 kbps (akış başlığı) | [Yayın](http://yayin.turkhosted.com:6006/) | [Kaynak](https://www.radio-browser.info/history/9aab2dd5-ec19-469a-a048-e3f7c63f8585) |
| 47 | Türkü Radyo (turkuradyo.info) | MP3 | 96 kbps (akış başlığı) | [Yayın](https://eu8.fastcast4u.com/proxy/ugur4129?mp=/1) | [Kaynak](https://www.radio-browser.info/history/ead1ae8e-10ae-4f90-9b83-d6ab9b40830f) |
| 48 | Dost Radyo | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](http://yayin1.yayindakiler.com:3156/) | [Kaynak](https://www.radio-browser.info/history/9626456e-0601-11e8-ae97-52543be04c81) |
| 49 | Halk Radyo | AAC (ADTS) | 104 kbps (akış başlığı) | [Yayın](https://anadolu.liderhost.com.tr:10876/;) | [Kaynak](https://www.radio-browser.info/history/7ab25d55-40ef-4dcc-8e85-1dd99650d830) |
| 50 | Park FM | AAC (ADTS) | 32 kbps (akış başlığı) | [Yayın](http://yayin.netradyom.com:8050/PARKFM/) | [Kaynak](https://www.radio-browser.info/history/2871e14f-254c-4b0a-b506-c59bc6175e43) |
| 51 | Hatay Tempo FM | MP3 | 96 kbps (akış başlığı) | [Yayın](https://sslyayin.netyayin.net:1215/stream) | [Kaynak](https://www.radio-browser.info/history/0730a3ce-73e4-4f39-b60c-ffeaa6ee1f1b) |
| 52 | Radyo Pir Yolu | MP3 | 128 kbps (akış başlığı) | [Yayın](http://radyopiryolu.canliyayinda.com:8165/) | [Kaynak](https://www.radio-browser.info/history/4533a6a3-0b8e-476b-b0ec-b4a56162caa6) |
| 53 | Radyo Türkü | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://yayin.radyomadyo.com:8008/stream) | [Kaynak](https://www.radio-browser.info/history/90fd1210-34f4-46a7-92bc-7f1ba7634460) |
| 54 | Pal Doğa | AAC (ADTS) | 96 kbps (akış başlığı) | [Yayın](http://shoutcast.radyogrup.com:1040/;*.mp3) | [Kaynak](https://www.radio-browser.info/history/0da692ab-2ee2-45e6-8303-26ff30949f91) |
| 55 | Radyo Gönül Mısraları | MP3 | 192 kbps (akış başlığı) | [Yayın](https://radyo.gonulmisralari.com/listen/canli/radio.mp3) | [Kaynak](https://www.radio-browser.info/history/f6516fa9-1e5b-4bc6-a196-35f8dd745496) |
| 56 | Kıyı Türkçe | MP3 | 128 kbps (akış başlığı) | [Yayın](https://kiyiturk.radioca.st/stream) | [Kaynak](https://www.radio-browser.info/history/c0f5e413-4c6a-4146-90d9-774ef452ee4b) |
| 57 | Radyo Banko | AAC (ADTS) | 32 kbps (akış başlığı) | [Yayın](http://yayin.netradyom.com:9910/RADYOBANKO) | [Kaynak](https://www.radio-browser.info/history/306970f9-8063-481d-9f6b-00b99833d206) |
| 58 | Mastika FM | MP3 | 128 kbps (akış başlığı) | [Yayın](https://mastika1.80.yayin.com.tr/) | [Kaynak](https://www.radio-browser.info/history/d6d307f6-038b-4b88-b70c-753004b3adcb) |
| 59 | Sızır FM | MP3 | Belirtilmedi | [Yayın](http://stream.radiojar.com/tamm1f7kk48uv) | [Kaynak](https://www.radio-browser.info/history/37ba98b1-f055-4363-893a-b0efe305f6a4) |
| 60 | Hayatmix Arabesk | MP3 | 128 kbps (akış başlığı) | [Yayın](https://radyo.yayin.com.tr:4078/mp3) | [Kaynak](https://www.radio-browser.info/history/a1cd74f5-1f8b-4fff-aab5-f0cbbddd2b77) |
| 61 | Radyo Musiki | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://dijimedya.radyotvonline.net/radyomusiki) | [Kaynak](https://www.radio-browser.info/history/ab7e542e-9cbe-4ad1-aea6-2aa31f1b3457) |
| 62 | Yaren FM | MP3 | 128 kbps (akış başlığı) | [Yayın](https://yarenfmn.80.yayin.com.tr//stream) | [Kaynak](https://www.radio-browser.info/history/1cc2e54d-0843-4606-b86c-5947908b27b0) |
| 63 | Tüm Zamanların En İyileri | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://playerservices.streamtheworld.com/api/livestream-redirect/GHOAT_TR128AAC.aac) | [Kaynak](https://www.radio-browser.info/history/811d9ec1-4b69-4759-8af6-682f5161eada) |
| 64 | Kiss Gold Türk | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://dijimedya.radyotvonline.net/goldturk) | [Kaynak](https://www.radio-browser.info/history/eadab7bc-b6d4-489b-a349-e6168fa78002) |
| 65 | Kalbe Dokunan Şarkılar | MP3 | 128 kbps (akış başlığı) | [Yayın](https://live.sinirsizhost.com.tr/8058/stream) | [Kaynak](https://www.radio-browser.info/history/51d0f041-acdf-44ab-aa2d-a2056aa3c640) |
| 66 | Yön Radyo | MP3 | 112 kbps (akış başlığı) | [Yayın](https://stream.radyolarburada.com:8020/yonradyo) | [Kaynak](https://www.radio-browser.info/history/3fdf15a4-685f-422c-bfee-49af64f5dbd1) |

## Alternatif, rock, caz ve elektronik (26)

| No | İstasyon | Biçim | Bildirilen bit hızı | Adres | Kaynak |
|---:|---|---|---|---|---|
| 67 | dinamo.fm sleep | MP3 | 320 kbps (akış başlığı) | [Yayın](http://channels.dinamo.fm/sleep-mp3) | [Kaynak](https://www.radio-browser.info/history/04407d31-2cc0-11e8-91bf-52543be04c81) |
| 68 | dinamo.fm deep | MP3 | 320 kbps (akış başlığı) | [Yayın](http://channels.dinamo.fm/deep-mp3) | [Kaynak](https://www.radio-browser.info/history/fd92e72f-2cbe-11e8-91bf-52543be04c81) |
| 69 | dinamo.fm caffe | MP3 | 320 kbps (akış başlığı) | [Yayın](http://channels.dinamo.fm/caffe-mp3) | [Kaynak](https://www.radio-browser.info/history/2815bbc7-2cbf-11e8-91bf-52543be04c81) |
| 70 | dinamo.fm discotheque | MP3 | 320 kbps (akış başlığı) | [Yayın](http://channels.dinamo.fm/discotheque-mp3) | [Kaynak](https://www.radio-browser.info/history/8c76958a-2cbf-11e8-91bf-52543be04c81) |
| 71 | Türk Rock FM | MP3 | 128 kbps (akış başlığı) | [Yayın](https://yayin5.radyohizmeti.com/8090/stream;) | [Kaynak](https://www.radio-browser.info/history/c8d41d0e-0437-485c-a62c-ae95ecfdae40) |
| 72 | Mayday Records | MP3 | 256 kbps (akış başlığı) | [Yayın](https://stream.maydayrecords.net/radio/8000/maydayrecords.mp3) | [Kaynak](https://www.radio-browser.info/history/60f46e8b-c57f-4ef0-8aea-be92dddfca43) |
| 73 | JoyTürk Rock | MP3 | 64 kbps (akış başlığı) | [Yayın](https://playerservices.streamtheworld.com/api/livestream-redirect/JOYTURK_ROCK_SC) | [Kaynak](https://www.radio-browser.info/history/354ffe25-8a45-4534-888b-c900054c1ba7) |
| 74 | Radyo Fenomen Clubbin | AAC (ADTS) | 56 kbps (akış başlığı) | [Yayın](https://live.radyofenomen.com/fenomenclubbin/128/icecast.audio) | [Kaynak](https://www.radio-browser.info/history/6f3d3422-9998-4079-8e1a-7d23cef150ea) |
| 75 | X Radio | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://stream.xradio.com.tr:8770/stream) | [Kaynak](https://www.radio-browser.info/history/f21b4940-2b6e-4436-b30f-fe16ce46f15a) |
| 76 | dinamo.fm fluent | MP3 | 320 kbps (akış başlığı) | [Yayın](http://channels.dinamo.fm/fluent-mp3) | [Kaynak](https://www.radio-browser.info/history/45bb5fc8-84df-4561-b282-935bb5dd7c86) |
| 77 | İTÜ Rock | MP3 | 128 kbps (akış başlığı) | [Yayın](http://160.75.86.29:8095/) | [Kaynak](https://www.radio-browser.info/history/39e9fafe-85ca-422b-b448-20f47c67c47c) |
| 78 | Loops Radio Progressive | MP3 | 320 kbps (akış başlığı) | [Yayın](https://progressive.ozelip.com/7670/stream) | [Kaynak](https://www.radio-browser.info/history/d5fb7ff2-4b30-4f0f-a66f-c44a480337ec) |
| 79 | Kıyı Caz | MP3 | 128 kbps (akış başlığı) | [Yayın](http://caz.radioca.st/) | [Kaynak](https://www.radio-browser.info/history/131f6fc8-1e32-49ab-9a4f-d556db5c7f3e) |
| 80 | Radyo ODTÜ Rock | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](http://stream.radyoodtu.com.tr/rock) | [Kaynak](https://www.radio-browser.info/history/aec438e2-74b3-11ea-b1cf-52543be04c81) |
| 81 | Radyo ODTÜ Easy | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://stream3.radyoodtu.com.tr:8000/easy) | [Kaynak](https://www.radio-browser.info/history/e15a3233-3781-4056-831e-c07dd5fc60be) |
| 82 | Radyo Black Love | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://anadolu.liderhost.com.tr:10964/) | [Kaynak](https://www.radio-browser.info/history/cad85f62-a486-4b1c-b90d-9a3a5956330b) |
| 83 | World Hits | MP3 | 128 kbps (akış başlığı) | [Yayın](http://37.247.98.8/stream/34/) | [Kaynak](https://www.radio-browser.info/history/694b1013-4537-47b0-9ad9-93b8f63893e2) |
| 84 | Find a Cutie | MP3 | 128 kbps (akış başlığı) | [Yayın](https://stream.zeno.fm/lyl2uv7fcvovv) | [Kaynak](https://www.radio-browser.info/history/f91c177e-7f45-4156-9699-d745993c6831) |
| 85 | dinamo.fm Minimüzikhol | MP3 | 128 kbps (akış başlığı) | [Yayın](http://channels.dinamo.fm/mmradyo-mp3) | [Kaynak](https://www.radio-browser.info/history/127006d6-f378-42d0-97b6-1c253a2f3d3e) |
| 86 | Loops Radio Elektronica Station | MP3 | 128 kbps (akış başlığı) | [Yayın](https://ssl22.radyotelekom.com/8232/stream) | [Kaynak](https://www.radio-browser.info/history/5729f872-934d-4e28-856e-6b7ae1808863) |
| 87 | RadioTEDU Jazz | AAC (ADTS) | 192 kbps (akış başlığı) | [Yayın](https://stream.radiotedu.com/cazz) | [Kaynak](https://www.radio-browser.info/history/596f41f5-3665-448d-a8f1-75dd5012cd3f) |
| 88 | RadioTEDU Lo-Fi | AAC (ADTS) | 192 kbps (akış başlığı) | [Yayın](https://stream.radiotedu.com/lofi) | [Kaynak](https://www.radio-browser.info/history/e8aebee3-aeb9-4d6c-afea-a830bd006cac) |
| 89 | Loops Radio Techno | MP3 | 128 kbps (akış başlığı) | [Yayın](https://ip179.ozelip.com/7688//asistan/stream/) | [Kaynak](https://www.radio-browser.info/history/b4c64aa8-9bf1-4015-bc8d-7e10ff1edce9) |
| 90 | Radio Glamorize | MP3 | 128 kbps (akış başlığı) | [Yayın](https://live.radioglamorize.com/stream1) | [Kaynak](https://www.radio-browser.info/history/25fba87c-ddc8-409a-8474-4ffbac809c59) |
| 91 | Mix A Mix FM | MP3 | 128 kbps (akış başlığı) | [Yayın](https://radyo.yayin.com.tr:5998/stream/1/?icy=http) | [Kaynak](https://www.radio-browser.info/history/a42161db-0b63-4c66-8980-51208450d8dc) |
| 92 | Power Dance | MP3 | 250 kbps (akış başlığı) | [Yayın](https://listen.powerapp.com.tr/powerdance/mpeg/icecast.audio) | [Kaynak](https://www.radio-browser.info/history/bf106bfa-4b34-4b50-9955-f7a36d76c0da) |

## Haber, spor ve sohbet (15)

| No | İstasyon | Biçim | Bildirilen bit hızı | Adres | Kaynak |
|---:|---|---|---|---|---|
| 93 | Habertürk Radyo | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://haberturkradyo.radyotvonline.net/haberturkradyo) | [Kaynak](https://www.radio-browser.info/history/14db05f4-b028-40dc-a13f-c53d068d929e) |
| 94 | Radyo Sputnik | MP3 | 128 kbps (akış başlığı) | [Yayın](https://icecast-rian.cdnvideo.ru/voicestm) | [Kaynak](https://www.radio-browser.info/history/0e545fd4-cbd9-4836-81d2-e183bb02da79) |
| 95 | TRT Radyo 1 | AAC (ADTS) | 129 kbps (akış başlığı) | [Yayın](https://trt.radyotvonline.net/trt1) | [Kaynak](https://www.radio-browser.info/history/c1e2e152-71d0-458f-9c41-5fb54a33dbf8) |
| 96 | Radyo Spor | MP3 | 128 kbps (akış başlığı) | [Yayın](http://46.20.4.5:8170/) | [Kaynak](https://www.radio-browser.info/history/960e5bb2-0601-11e8-ae97-52543be04c81) |
| 97 | TRT Radyo Haber | AAC (ADTS) | 132 kbps (akış başlığı) | [Yayın](https://trt.radyotvonline.net/trthaber) | [Kaynak](https://www.radio-browser.info/history/9ed85a92-9fb4-4ae8-baca-59347adc782c) |
| 98 | TGRT FM | MP3 | 128 kbps (akış başlığı) | [Yayın](https://icecasttgrt.ihlasdigitalassets.com/tgrtfm) | [Kaynak](https://www.radio-browser.info/history/ff3b8372-1f59-4b18-a4fc-2240b8ab5417) |
| 99 | Radyo Trafik Ankara | AAC (ADTS) | 64 kbps (akış başlığı) | [Yayın](http://live2.radyotvonline.com:8000/) | [Kaynak](https://www.radio-browser.info/history/960e285b-0601-11e8-ae97-52543be04c81) |
| 100 | NTV Radyo | MP3 | 128 kbps (akış başlığı) | [Yayın](http://ntvrdwmp.radyotvonline.com/) | [Kaynak](https://www.radio-browser.info/history/ad09398c-3f3d-4311-9344-c76395e46805) |
| 101 | Meteorolojinin Sesi | MP3 | 128 kbps (akış başlığı) | [Yayın](http://radyomgm.canliyayinda.com/;stream.mp3) | [Kaynak](https://www.radio-browser.info/history/3acd808a-e801-43bb-bdb3-16a8f4761f0d) |
| 102 | CGTN Türk Radyo | MP3 | 128 kbps (akış başlığı) | [Yayın](https://criturk.radyotvonline.net/) | [Kaynak](https://www.radio-browser.info/history/ea534546-de6e-4d08-b4bf-447a1cbeee56) |
| 103 | Radyo Gol | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://edge1.radyotvonline.net/shoutcast/play/radyogol) | [Kaynak](https://www.radio-browser.info/history/87c44288-3030-4e9f-b5af-62fc254fa3f9) |
| 104 | Apaçık Radyo | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](http://stream.34bit.net/ar.aac) | [Kaynak](https://www.radio-browser.info/history/ce26d234-1413-4a97-8f7d-5a476bacf104) |
| 105 | 24 Radyo | MP3 | 128 kbps (akış başlığı) | [Yayın](https://radyo24.radyotvonline.net/radyo24) | [Kaynak](https://www.radio-browser.info/history/21df519d-9c3f-47e5-a8e1-32a80901398f) |
| 106 | Radyo Trafik İstanbul | MP3 | 128 kbps (akış başlığı) | [Yayın](http://37.247.100.118/stream/157/) | [Kaynak](https://www.radio-browser.info/history/32c900dc-1027-486b-8397-efd57ad5c2fb) |
| 107 | Lig Radyo | MP3 | 160 kbps (akış başlığı) | [Yayın](https://ligradyo.radyotvonline.net/ligradyo) | [Kaynak](https://www.radio-browser.info/history/55fe98c9-1ae8-4fd7-8d30-443ff052b3f0) |

## Dini ve manevi yayınlar (17)

| No | İstasyon | Biçim | Bildirilen bit hızı | Adres | Kaynak |
|---:|---|---|---|---|---|
| 108 | Gözyaşı FM | MP3 | 64 kbps (akış başlığı) | [Yayın](http://yayin1.canliyayin.org:8700/;*.mp3) | [Kaynak](https://www.radio-browser.info/history/775bea1d-8c92-11e9-ad01-52543be04c81) |
| 109 | Kur'an ve Türkçe Meal | MP3 | 64 kbps (akış başlığı) | [Yayın](http://82.145.41.8:19175/) | [Kaynak](https://www.radio-browser.info/history/c0aa78c7-8461-4c36-a85a-b651f2f5fa39) |
| 110 | Her An Kur'an | AAC (ADTS) | 32 kbps (akış başlığı) | [Yayın](https://radio.mihr.com/radio/8050/radio.mp3) | [Kaynak](https://www.radio-browser.info/history/70378409-2e51-403e-b7cd-20b8624fbea7) |
| 111 | Radyo Fıtrat | MP3 | 128 kbps (akış başlığı) | [Yayın](http://radyofitrat.radyotvonline.net/) | [Kaynak](https://www.radio-browser.info/history/7ea58b1d-e502-4e03-abb1-65b2408bab2d) |
| 112 | Bayram FM | AAC (ADTS) | 96 kbps (akış başlığı) | [Yayın](https://sslyayin.netyayin.net/3442/stream/;stream.mp3) | [Kaynak](https://www.radio-browser.info/history/cc9c0c3a-bcc7-4e54-9456-5cb125cf8f93) |
| 113 | Erkam Radyo | MP3 | 128 kbps (akış başlığı) | [Yayın](https://api-tv5.yayin.com.tr:8002/mp3) | [Kaynak](https://www.radio-browser.info/history/cef30f27-1f96-4005-8b7c-e161dba43ccf) |
| 114 | Radyo Hakkın Sesi | MP3 | 128 kbps (akış başlığı) | [Yayın](https://radio.mihr.com/radio/8010/hakkinsesi) | [Kaynak](https://www.radio-browser.info/history/9a83c49d-d17e-4fec-ba64-3db850788da4) |
| 115 | Şafak Radyo | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](http://safakradyo.com:2626/;) | [Kaynak](https://www.radio-browser.info/history/d9eb277b-f0d6-4c02-aeaa-273521944bec) |
| 116 | İhya FM | MP3 | 128 kbps (akış başlığı) | [Yayın](https://radyo.yayin.com.tr:2727/stream) | [Kaynak](https://www.radio-browser.info/history/caff715e-7b52-436d-be35-80e39906b2dd) |
| 117 | Miraç FM | MP3 | 128 kbps (akış başlığı) | [Yayın](https://anadolu.liderhost.com.tr:9416/stream) | [Kaynak](https://www.radio-browser.info/history/288483da-41bd-466a-8282-d3f5dcc4c2d0) |
| 118 | Nur Radyo | AAC (ADTS) | 32 kbps (akış başlığı) | [Yayın](https://canli.hidayetradyolari.com/listen/radyo_nur/radio.mp3) | [Kaynak](https://www.radio-browser.info/history/d9dc8607-d88d-48f2-bde0-1dc98c901f68) |
| 119 | Radyo Nida | MP3 | 112 kbps (akış başlığı) | [Yayın](https://anadolu.liderhost.com.tr/8106/stream) | [Kaynak](https://www.radio-browser.info/history/1020f3df-3507-476a-85ca-152e03cb75ff) |
| 120 | Konya İsra FM | MP3 | 128 kbps (akış başlığı) | [Yayın](https://yayin2.canliyayin.org:8210/;stream) | [Kaynak](https://www.radio-browser.info/history/2e728efe-5d69-410b-a5d2-e987ca66c09f) |
| 121 | Lalegül FM | MP3 | 256 kbps (akış başlığı) | [Yayın](https://icecast.netmedya.net/lalegulfm) | [Kaynak](https://www.radio-browser.info/history/d6a8ae1d-75fc-490f-a9fe-4fcaacaf9ba6) |
| 122 | Karadut FM | MP3 | 96 kbps (akış başlığı) | [Yayın](https://yayin5.radyohizmeti.com/9982/stream) | [Kaynak](https://www.radio-browser.info/history/869d6802-36b4-487f-900a-3b26cd9e5ee1) |
| 123 | Radyo Gül | MP3 | 192 kbps (akış başlığı) | [Yayın](http://yayin2.canliyayin.org:8866/;*.mp3) | [Kaynak](https://www.radio-browser.info/history/70d5625c-8c8e-11e9-ad01-52543be04c81) |
| 124 | Bizim İller Radyo | AAC (ADTS) | 64 kbps (akış başlığı) | [Yayın](http://yayin.yayindakiler.com:4078/;) | [Kaynak](https://www.radio-browser.info/history/8ec988d7-1eff-41d9-828a-fd53fc15eeb0) |

## Yerel ve üniversite radyoları (47)

| No | İstasyon | Biçim | Bildirilen bit hızı | Adres | Kaynak |
|---:|---|---|---|---|---|
| 125 | Radyo 35 | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://yayin.turkhosted.com/6012/stream) | [Kaynak](https://www.radio-browser.info/history/fcbf07d8-d3b3-4d9f-839a-3cfea3391b35) |
| 126 | Radio 44 | MP3 | 128 kbps (akış başlığı) | [Yayın](https://radyo44.80.yayin.com.tr/stream) | [Kaynak](https://www.radio-browser.info/history/57d6c252-a148-4e96-9602-39f3d6f22eb1) |
| 127 | Radyo Akdeniz | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://yayin.radyoakdeniz.com/9470/stream) | [Kaynak](https://www.radio-browser.info/history/14797ffb-2fdd-4a54-9748-e96486914dd2) |
| 128 | Radyo Deva Adana | AAC (ADTS) | 48 kbps (akış başlığı) | [Yayın](http://yayin.yayindakiler.com:3040/;stream.mp3) | [Kaynak](https://www.radio-browser.info/history/d8eac60e-47e4-4051-914c-24dc63b02c32) |
| 129 | Radyo Sarıkamış | MP3 | 128 kbps (akış başlığı) | [Yayın](https://live.radyositesihazir.com:8004/stream) | [Kaynak](https://www.radio-browser.info/history/1c8bd5a0-3cd3-4596-bb0a-47a777cbf0dd) |
| 130 | Aktif FM | MP3 | 112 kbps (akış başlığı) | [Yayın](https://radyo.yayindakiler.com:3164/stream) | [Kaynak](https://www.radio-browser.info/history/0767dea5-dd44-440a-b5bf-f4c58295ede4) |
| 131 | Radyo 13 | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://radyo.yayindakiler.com:3050/stream) | [Kaynak](https://www.radio-browser.info/history/1e84114e-ade9-4baf-9f85-89411162ccfe) |
| 132 | Köroğlu FM | AAC (ADTS) | 64 kbps (akış başlığı) | [Yayın](https://yayin.turkhosted.com:6068/stream?/;stream.mp3) | [Kaynak](https://www.radio-browser.info/history/1a1ade69-4796-4185-9793-ea9ca970ce02) |
| 133 | Tarsus FM | MP3 | 128 kbps (akış başlığı) | [Yayın](https://tarsusfm.80.yayin.com.tr//stream) | [Kaynak](https://www.radio-browser.info/history/ea36dc14-cf97-4e57-b0e0-9b21a52344e6) |
| 134 | Radyo Koç | MP3 | 125 kbps (akış başlığı) | [Yayın](https://stores.powerapp.com.tr/kocschool/mpeg/icecast.audio) | [Kaynak](https://www.radio-browser.info/history/fb984504-b551-4ef2-b919-45d2cc066e03) |
| 135 | Radyo Taksi | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://live.radyo.in/8006/stream) | [Kaynak](https://www.radio-browser.info/history/b5981132-7fdd-43cf-9433-eaa712b8ee21) |
| 136 | Kent FM | MP3 | 128 kbps (akış başlığı) | [Yayın](https://kentfm955.ozelip.com/9306/stream) | [Kaynak](https://www.radio-browser.info/history/c21d3998-f91b-48e4-9bb1-74fa12ffb9e5) |
| 137 | Radyo Venüs | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://stream2.rtvtakip.com:7005/;stream.mp3) | [Kaynak](https://www.radio-browser.info/history/cbd35624-d0ed-4676-b0c6-a18358987584) |
| 138 | Radio Life Kayseri | MP3 | 192 kbps (akış başlığı) | [Yayın](https://yayin2.canliyayin.org:10990/;*.mp3) | [Kaynak](https://www.radio-browser.info/history/26eb1f53-c037-43e8-8f92-3d90769f5972) |
| 139 | Radyo Trio | AAC (ADTS) | 64 kbps (akış başlığı) | [Yayın](https://radyotrio.radyotvonline.net/radyotrio) | [Kaynak](https://www.radio-browser.info/history/2484f562-2a83-4c6d-9f91-038644ceb11e) |
| 140 | Yıldız FM | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://radyo.yayin.com.tr:5940/) | [Kaynak](https://www.radio-browser.info/history/da574b9d-d86a-4cef-81a2-fde612759a0d) |
| 141 | Çınar FM | AAC (ADTS) | 64 kbps (akış başlığı) | [Yayın](https://sslyayin.netyayin.net:10992/) | [Kaynak](https://www.radio-browser.info/history/cdd35eda-263b-4475-81d7-6696024e097e) |
| 142 | Merkez FM Samsun | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](http://uydu.radyotvyayini.com:8016/) | [Kaynak](https://www.radio-browser.info/history/8456989c-06fb-445c-b4d9-9a4e84d608be) |
| 143 | Mega Radyo İskenderun | MP3 | 128 kbps (akış başlığı) | [Yayın](https://dinle.megaradyoiskenderun.com/8024/stream) | [Kaynak](https://www.radio-browser.info/history/664c2489-5a16-4fb0-8aaf-8b8b5306aa9c) |
| 144 | Espiye FM | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://anadolu.liderhost.com.tr/9028/stream) | [Kaynak](https://www.radio-browser.info/history/fd632252-faa7-4ec3-8f58-f497fdef2110) |
| 145 | Radyo 75 | MP3 | 128 kbps (akış başlığı) | [Yayın](https://anadolu.liderhost.com.tr/8126/stream) | [Kaynak](https://www.radio-browser.info/history/d8ed6337-bc5c-4d37-80ec-efe75881796b) |
| 146 | Radyo Sinerji | MP3 | 128 kbps (akış başlığı) | [Yayın](https://radio.guventechnology.com/9994/stream) | [Kaynak](https://www.radio-browser.info/history/0dc34aa4-d185-413f-8c04-5763bf2ec653) |
| 147 | Boğaz FM | MP3 | 128 kbps (akış başlığı) | [Yayın](https://radyo.yayin.com.tr:5938/) | [Kaynak](https://www.radio-browser.info/history/205d99bd-4e97-4373-a0d8-2658c0e75afe) |
| 148 | İmbat FM | AAC (ADTS) | 96 kbps (akış başlığı) | [Yayın](https://radyo.medyahost.com.tr/8034/stream) | [Kaynak](https://www.radio-browser.info/history/015ea1ab-f1dd-4f20-8dcb-edd95c3cca59) |
| 149 | ERT FM Elazığ | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://radyo.yayin.com.tr:8012/) | [Kaynak](https://www.radio-browser.info/history/70c634dc-e5a5-44ed-98c7-538e073a3237) |
| 150 | Serhat FM 91.5 | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](http://yayin.serhatfm.com:9150/stream) | [Kaynak](https://www.radio-browser.info/history/ddf94722-dd93-4fb5-ac19-ad42649a30e1) |
| 151 | Radyo Maksat | MP3 | 96 kbps (akış başlığı) | [Yayın](https://live.sinirsizhost.com.tr/8038/stream) | [Kaynak](https://www.radio-browser.info/history/f85ee60b-db9a-46ed-a403-d8755e159cfa) |
| 152 | OMÜ Radyo | AAC (ADTS) | 64 kbps (akış başlığı) | [Yayın](https://omuradyo.ozelip.com/8042/stream) | [Kaynak](https://www.radio-browser.info/history/da32fe6d-2c0d-4ffa-bf9d-b408e077417c) |
| 153 | Gelişim Radyo | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](http://radyo.yayindakiler.com:4008/stream) | [Kaynak](https://www.radio-browser.info/history/fa61e57b-cb22-4fcb-bdc9-57e1f1091605) |
| 154 | Romantik SES Sakarya | AAC (ADTS) | 64 kbps (akış başlığı) | [Yayın](https://radyo.romantikses.com/romantiksessakary/stream) | [Kaynak](https://www.radio-browser.info/history/78003d37-6c68-4f8d-bb0f-9d5adbd0c9c1) |
| 155 | Bizim FM Şile | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://stream.turkiyeradyolari.com/9300/stream?_t=1786859250066) | [Kaynak](https://www.radio-browser.info/history/4dc929fc-04c8-4c07-85df-4bbce06c89ab) |
| 156 | Ereğli FM | MP3 | 128 kbps (akış başlığı) | [Yayın](http://radyo.yayindakiler.com:3000/stream) | [Kaynak](https://www.radio-browser.info/history/1b6148c0-5582-418c-b836-0b7246064f55) |
| 157 | Seydişehir FM | MP3 | 128 kbps (akış başlığı) | [Yayın](https://radyo.yayin.com.tr:4142/strem) | [Kaynak](https://www.radio-browser.info/history/5ddd5562-7813-417d-80ce-2b1a336a3512) |
| 158 | BesteFM | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://stream.zeno.fm/tnvnovtgozstv) | [Kaynak](https://www.radio-browser.info/history/54e94804-4a95-4b4a-ba92-eb4c44d98842) |
| 159 | Radyo Atılım | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://listen.powerapp.com.tr/radyoatilim/mpeg/icecast.audio) | [Kaynak](https://www.radio-browser.info/history/0316f522-5a00-499c-bcf0-aceb9076f33c) |
| 160 | Elazığ Mavi Radyo | MP3 | 128 kbps (akış başlığı) | [Yayın](https://yayin.turkhosted.com/6066/stream) | [Kaynak](https://www.radio-browser.info/history/1fd7925e-09ac-42b3-b560-f4948fa8bfd1) |
| 161 | Star FM | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://anadolu.liderhost.com.tr/9078/stream) | [Kaynak](https://www.radio-browser.info/history/2a421ffc-d50e-43a1-8127-f931912fab17) |
| 162 | Yeniköy FM | MP3 | 128 kbps (akış başlığı) | [Yayın](https://anadolu.liderhost.com.tr:10924/stream) | [Kaynak](https://www.radio-browser.info/history/d4d626b7-5342-4a79-9d88-9d3620568e4a) |
| 163 | Radyo Can | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](http://yayin2.yayindakiler.com:4174/;stream.mp3) | [Kaynak](https://www.radio-browser.info/history/0c12afb8-cc4d-4291-9d0f-5dbedc1690a8) |
| 164 | BAU Radyo | MP3 | 128 kbps (akış başlığı) | [Yayın](https://ssl120.radyotelekom.com.tr/8106/stream.mp4) | [Kaynak](https://www.radio-browser.info/history/5b3ce092-af80-46ac-8e07-c97f6cb09423) |
| 165 | Kent Radyo Mersin | MP3 | 128 kbps (akış başlığı) | [Yayın](https://anadolu.liderhost.com.tr:10906/) | [Kaynak](https://www.radio-browser.info/history/7ae6b7bf-8629-4024-a9a8-ce62099b3b7e) |
| 166 | Radyo 16 | AAC (ADTS) | 80 kbps (akış başlığı) | [Yayın](https://radyo.yayin.com.tr:6038/) | [Kaynak](https://www.radio-browser.info/history/7db146b3-e03f-4d43-87bb-242b7a9dc81b) |
| 167 | Radyo Sevtek | MP3 | 128 kbps (akış başlığı) | [Yayın](https://sevtekfm.ozelip.com/8020/stream) | [Kaynak](https://www.radio-browser.info/history/da88eba7-1d35-4cb1-bc9a-ede056504bfa) |
| 168 | Derya FM | AAC (ADTS) | 64 kbps (akış başlığı) | [Yayın](https://yayin.turkhosted.com:6032/) | [Kaynak](https://www.radio-browser.info/history/ef7cf7f5-75de-4561-b7e3-fa8f04d4188a) |
| 169 | Hey Radyo | MP3 | 256 kbps (akış başlığı) | [Yayın](https://usa4.fastcast4u.com/proxy/radyoizmirfm/) | [Kaynak](https://www.radio-browser.info/history/341e06c4-da16-475d-a013-ba296d52db13) |
| 170 | Hemdem Pop Radyo | AAC (ADTS) | 64 kbps (akış başlığı) | [Yayın](https://yayin.hemdemradyo.com/) | [Kaynak](https://www.radio-browser.info/history/2e4ee5d9-18ef-4073-8d6e-5582808216ff) |
| 171 | Radyo Armoni | AAC (ADTS) | 128 kbps (akış başlığı) | [Yayın](https://radyo.yayindakiler.com:4084/stream) | [Kaynak](https://www.radio-browser.info/history/4a070d1f-2f41-42dc-adac-c2282788ddec) |

## Power ve PowerTürk tematik kanalları (19)

| No | İstasyon | Biçim | Bildirilen bit hızı | Adres | Kaynak |
|---:|---|---|---|---|---|
| 172 | Power FM | MP3 | 250 kbps (akış başlığı) | [Yayın](https://listen.powerapp.com.tr/powerfm/mpeg/icecast.audio) | [Kaynak](https://www.powerapp.com.tr/) |
| 173 | PowerTürk | AAC (ADTS) | 250 kbps (akış başlığı) | [Yayın](https://listen.powerapp.com.tr/powerturk/mpeg/icecast.audio) | [Kaynak](https://www.powerapp.com.tr/) |
| 174 | Power Plus | MP3 | 250 kbps (akış başlığı) | [Yayın](https://listen.powerapp.com.tr/powerplus/mpeg/icecast.audio) | [Kaynak](https://www.powerapp.com.tr/) |
| 175 | Power Pop | MP3 | 250 kbps (akış başlığı) | [Yayın](https://listen.powerapp.com.tr/powerpop/mpeg/icecast.audio) | [Kaynak](https://www.powerapp.com.tr/) |
| 176 | Power Love | MP3 | 250 kbps (akış başlığı) | [Yayın](https://listen.powerapp.com.tr/powerlove/mpeg/icecast.audio) | [Kaynak](https://www.powerapp.com.tr/) |
| 177 | Power Smooth | MP3 | 250 kbps (akış başlığı) | [Yayın](https://listen.powerapp.com.tr/powersmooth/mpeg/icecast.audio) | [Kaynak](https://www.powerapp.com.tr/) |
| 178 | Power XL | AAC (ADTS) | 250 kbps (akış başlığı) | [Yayın](https://listen.powerapp.com.tr/powerextralounge/mpeg/icecast.audio) | [Kaynak](https://www.powerapp.com.tr/) |
| 179 | Power Earth | MP3 | 250 kbps (akış başlığı) | [Yayın](https://listen.powerapp.com.tr/powerearth/mpeg/icecast.audio) | [Kaynak](https://www.powerapp.com.tr/) |
| 180 | Power Deep | AAC (ADTS) | 250 kbps (akış başlığı) | [Yayın](https://listen.powerapp.com.tr/powerdeep/mpeg/icecast.audio) | [Kaynak](https://www.powerapp.com.tr/) |
| 181 | PowerTürk Rap | MP3 | 250 kbps (akış başlığı) | [Yayın](https://listen.powerapp.com.tr/powerturkrap/mpeg/icecast.audio) | [Kaynak](https://www.powerapp.com.tr/) |
| 182 | PowerTürk Akustik | AAC (ADTS) | 250 kbps (akış başlığı) | [Yayın](https://listen.powerapp.com.tr/powerturkakustik/mpeg/icecast.audio) | [Kaynak](https://www.powerapp.com.tr/) |
| 183 | PowerTürk Taptaze | AAC (ADTS) | 250 kbps (akış başlığı) | [Yayın](https://listen.powerapp.com.tr/powerturktaptaze/mpeg/icecast.audio) | [Kaynak](https://www.powerapp.com.tr/) |
| 184 | PowerTürk Dans | MP3 | 250 kbps (akış başlığı) | [Yayın](https://listen.powerapp.com.tr/powerturkdans/mpeg/icecast.audio) | [Kaynak](https://www.powerapp.com.tr/) |
| 185 | PowerTürk Rock | MP3 | 250 kbps (akış başlığı) | [Yayın](https://listen.powerapp.com.tr/powerturkrock/mpeg/icecast.audio) | [Kaynak](https://www.powerapp.com.tr/) |
| 186 | Power Greece | MP3 | 250 kbps (akış başlığı) | [Yayın](https://listen.powerapp.com.tr/powergreece/mpeg/icecast.audio) | [Kaynak](https://www.powerapp.com.tr/) |
| 187 | Power R&B Hip-Hop | AAC (ADTS) | 250 kbps (akış başlığı) | [Yayın](https://listen.powerapp.com.tr/powerrbhiphop/mpeg/icecast.audio) | [Kaynak](https://www.powerapp.com.tr/) |
| 188 | Power Salsa | MP3 | 250 kbps (akış başlığı) | [Yayın](https://listen.powerapp.com.tr/powersalsa/mpeg/icecast.audio) | [Kaynak](https://www.powerapp.com.tr/) |
| 189 | Power Gold | AAC (ADTS) | 250 kbps (akış başlığı) | [Yayın](https://listen.powerapp.com.tr/powergold/mpeg/icecast.audio) | [Kaynak](https://www.powerapp.com.tr/) |
| 190 | Power Smooth Jazz | MP3 | 250 kbps (akış başlığı) | [Yayın](https://listen.powerapp.com.tr/powersmoothjazz/mpeg/icecast.audio) | [Kaynak](https://www.powerapp.com.tr/) |

## HLS deneme listesi (32)

| No | İstasyon | Biçim | Bildirilen bit hızı | Adres | Kaynak |
|---:|---|---|---|---|---|
| 191 | TRT Radyo 3 | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://rd-trtradyo3.medya.trt.com.tr/master.m3u8) | [Kaynak](https://radyo.trt.net.tr/player.js?v=3.2.1) |
| 192 | TRT Nağme | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://rd-trtnagme.medya.trt.com.tr/master.m3u8) | [Kaynak](https://radyo.trt.net.tr/player.js?v=3.2.1) |
| 193 | TRT Türkü | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://rd-trtturku.medya.trt.com.tr/master.m3u8) | [Kaynak](https://radyo.trt.net.tr/player.js?v=3.2.1) |
| 194 | TRT Antalya | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://radio-trtantalya.medya.trt.com.tr/master.m3u8) | [Kaynak](https://radyo.trt.net.tr/player.js?v=3.2.1) |
| 195 | TRT Çukurova | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://radio-trtcukurova.medya.trt.com.tr/master.m3u8) | [Kaynak](https://radyo.trt.net.tr/player.js?v=3.2.1) |
| 196 | TRT GAP Diyarbakır | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://radio-trtgap.medya.trt.com.tr/master.m3u8) | [Kaynak](https://radyo.trt.net.tr/player.js?v=3.2.1) |
| 197 | TRT Erzurum | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://radio-trterzurum.medya.trt.com.tr/master.m3u8) | [Kaynak](https://radyo.trt.net.tr/player.js?v=3.2.1) |
| 198 | TRT Trabzon | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://radio-trttrabzon.medya.trt.com.tr/master.m3u8) | [Kaynak](https://radyo.trt.net.tr/player.js?v=3.2.1) |
| 199 | TRT TSR | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://radio-trttsr.medya.trt.com.tr/master.m3u8) | [Kaynak](https://radyo.trt.net.tr/player.js?v=3.2.1) |
| 200 | Memleketim FM | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://radio-trtmemleketimfm.medya.trt.com.tr/master.m3u8) | [Kaynak](https://radyo.trt.net.tr/player.js?v=3.2.1) |
| 201 | Kral FM | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://dygedge2.radyotvonline.net/kralfm/playlist.m3u8) | [Kaynak](https://www.kralmuzik.com.tr/) |
| 202 | Kral Pop | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://dygedge.radyotvonline.net/kralpop/playlist.m3u8) | [Kaynak](https://www.kralmuzik.com.tr/) |
| 203 | Kafa Radyo | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://moondigitalmaster.radyotvonline.net/kafaradyo/playlist.m3u8) | [Kaynak](https://www.radio-browser.info/history/4dc09559-aa5c-4ecd-a8cf-4fc450471ae4) |
| 204 | Best FM | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://ssldyg.radyotvonline.com/best/bestfm.stream/playlist.m3u8) | [Kaynak](https://www.bestfm.com.tr/) |
| 205 | Number1 Türk | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://ssldyg2.radyotvonline.com/live/numberoneturk.stream/playlist.m3u8) | [Kaynak](https://www.numberone.com.tr/2015/12/17/nr1-turk-fm-dinle-canli-radyo-dinle/) |
| 206 | Number1 FM | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://ssldyg2.radyotvonline.com/live/numberonefm.stream/playlist.m3u8) | [Kaynak](https://www.numberone.com.tr/2015/12/17/number1-fm-dinle-canli-radyo-dinle/) |
| 207 | Best of Number1 | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://ssldyg2.radyotvonline.com/live/bestofyabanci.stream/playlist.m3u8) | [Kaynak](https://www.numberone.com.tr/2017/08/16/best-of-number1-canli-radyo-dinle/) |
| 208 | Number1 Türk Damar | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://ssldyg2.radyotvonline.com/live/damar.stream/playlist.m3u8) | [Kaynak](https://www.numberone.com.tr/2017/07/10/nr1-turk-damar-radyo-dinle/) |
| 209 | Number1 Slow | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://ssldyg2.radyotvonline.com/live/yabancislow.stream/playlist.m3u8) | [Kaynak](https://www.numberone.com.tr/2015/12/16/number1-slow-canli-radyo-dinle/) |
| 210 | Radyo Turkuvaz | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://trkvz-radyolar.ercdn.net/radyoturkuvaz/playlist.m3u8) | [Kaynak](https://www.radio-browser.info/history/7046d480-674a-415f-8736-395600b5c7fd) |
| 211 | A Haber Radyo | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://trkvz-radyolar.ercdn.net/ahaberradyo/playlist.m3u8) | [Kaynak](https://www.radio-browser.info/history/862444ed-dcfb-4c23-b2ad-b85d5c8f91ba) |
| 212 | Pal FM | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://ssl4.radyotvonline.com/radyohome/palfmhome.stream/playlist.m3u8) | [Kaynak](https://www.radio-browser.info/history/f4121c74-9b1a-4051-8864-9db00eab2848) |
| 213 | Pal Nostalji | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://moondigitaledge.radyotvonline.net/palnostalji/playlist.m3u8) | [Kaynak](https://www.radio-browser.info/history/520bfe3e-2556-49f9-9846-e2c610978b60) |
| 214 | A Spor Radyo | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://trkvz-radyolar.ercdn.net/asporradyo/chunklist.m3u8) | [Kaynak](https://www.radio-browser.info/history/2ca9849a-ddde-411a-a79c-2a0ac728a4b9) |
| 215 | Turkuvaz Nostalji | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://trkvz-radyolar.ercdn.net/turkuvaznostalji/playlist.m3u8) | [Kaynak](https://www.radio-browser.info/history/f1466973-55d2-440f-9c08-e7e8a218962c) |
| 216 | Turkuvaz Anadolu | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](http://trkvz-radyolar.ercdn.net/turkuvazanadolu/Playlist.m3u8) | [Kaynak](https://www.radio-browser.info/history/c83e6f0f-cfb3-48a1-9cb8-34d61645bd20) |
| 217 | Turkuvaz Musiki | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://trkvz-radyolar.ercdn.net/turkuvazmusiki/playlist.m3u8) | [Kaynak](https://www.radio-browser.info/history/3cb7e617-ead1-4a88-9239-802050b33a13) |
| 218 | Turkuvaz Romantik | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://trkvz-radyolar.ercdn.net/turkuvazromantik/playlist.m3u8) | [Kaynak](https://www.radio-browser.info/history/daec7fd1-956d-4291-bd83-05aa4cdd409a) |
| 219 | Turkuvaz Efsane | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://trkvz-radyolar.ercdn.net/turkuvazefsane/playlist.m3u8) | [Kaynak](https://www.radio-browser.info/history/a0e31faa-d78e-439e-ae23-9e937b4848e4) |
| 220 | Diyanet Radyo | HLS / MPEG-TS | Belirtilmedi | [Yayın](https://eustr73.mediatriple.net/videoonlylive/mtikoimxnztxlive/broadcast_5e3c1171d7d2a.smil/playlist.m3u8) | [Kaynak](https://www.radio-browser.info/history/6420aa7b-70b7-4239-aa43-e3fb17638b0b) |
| 221 | Diyanet Kur'an Radyo | HLS / MPEG-TS | Belirtilmedi | [Yayın](https://eustr73.mediatriple.net/videoonlylive/mtikoimxnztxlive/broadcast_5e3c14192aa92.smil/playlist.m3u8) | [Kaynak](https://www.radio-browser.info/history/6a02fc36-675e-422f-9f00-bab672a6233d) |
| 222 | Akra FM | HLS / AAC (ADTS) | Belirtilmedi | [Yayın](https://d3r5bwwuab2v60.cloudfront.net/akracanli2/_definst_/livestream_aac/playlist.m3u8) | [Kaynak](https://www.radio-browser.info/history/1263b2bf-8c95-11e9-ad01-52543be04c81) |

## Hazır listelere alınmayan kayıtlar

Aşağıdaki kayıtlar ölü yayın ilan edilmemiştir; yalnızca bu hazırlıktaki kontrol veya kimlik eşleştirmesi yeterli olmadığından çıkarılmıştır.

| Kayıt | Gerekçe |
|---|---|
| Best Slow | Kısa kontrolde doğrulanamadı: HTTPError: HTTP Error 404: Not Available |
| 102.1 Arabeskin Merkezi Fm | Yayın kimliği DamarTürk FM; dizindeki Arabeskin Merkezi adıyla uyuşmuyor. |
| DT Nostalji | Yayın kimliği Fakir FM; DT Nostalji adıyla uyuşmuyor. |
| İTÜ Klasik | İTÜ Klasik adıyla kayıtlı adres İTÜ Jazz/Blues bildiriyor. |
| MOOD ROCK | Kısa kontrolde doğrulanamadı: URLError: <urlopen error [WinError 10061] No connection could be made because the target machine actively refused it> |
| Erkam Radyo Alternatif | Erkam Radyo alternatif adresi; aynı istasyon zaten listede. |
| (93.0) IGDIR FM | Kısa kontrolde doğrulanamadı: URLError: <urlopen error [SSL: CERTIFICATE_VERIFY_FAILED] certificate verify failed: Hostname mismatch, certificate is not valid for 'yayin.yayindakiler.com'. (_ssl.c:1006)> |
| Göl Radyo | Göl Radyo adıyla kayıtlı adres Süper FM bildiriyor. |
| RADYO MEGA | Mega Radyo İskenderun alternatif adresi; aynı istasyon zaten listede. |
| Radyo 2000 İstanbul | Kısa kontrolde doğrulanamadı: HTTPError: HTTP Error 404: Not Found |
| Fırat FM | Kısa kontrolde doğrulanamadı: URLError: <urlopen error [SSL: CERTIFICATE_VERIFY_FAILED] certificate verify failed: Hostname mismatch, certificate is not valid for 'anadolu.liderhost.com.tr'. (_ssl.c:1006)> |
| Radyo Devrek | Radyo Devrek adıyla kayıtlı adres Radyo Kaptan bildiriyor. |
| Radyo 13 | Radyo 13 alternatif adresi; aynı istasyon zaten listede. |
| Doğan 22 | Doğan 22 adıyla kayıtlı adres Doğan 21 bildiriyor. |
| Mega Ask FM | Adres canlı radyo değil, Tawk sohbet bildirim sesi MP3 dosyası. |
| Giresun FM | Giresun FM adıyla kayıtlı adres Gimsa Radyo bildiriyor. |
| Edizfm | Yayın kimliği Edizfm Montreal; Türkiye kapsamına alınmadı. |
| Number1 Türk Rap | Number1 Türk Rap adıyla kayıtlı adres KissTürk Rap bildiriyor; kimlik belirsiz. |
| Karma Türk | Karma Türk adıyla kayıtlı adres PopTime bildiriyor; kimlik belirsiz. |
| Karma Türk Slow | Karma Türk Slow adıyla kayıtlı adres Slow Time bildiriyor; alternatif/kimlik belirsiz. |
| Number1 Türk Slow | HLS kontrolde doğrulanamadı: TimeoutError: The read operation timed out |

## Kaynaklar

- [Radio Browser](https://www.radio-browser.info/) — Türkiye dizini; tek başına doğrulama kabul edilmedi.
- [TRT Radyo kanalları](https://radyo.trt.net.tr/kanallar) ve resmi player.js kanal adresleri.
- [Karnaval radyoları](https://karnaval.com/radyolar).
- [PowerApp](https://www.powerapp.com.tr/). Power doğrudan mount adresleri resmi oynatıcıdaki kanal kimlikleri ve sağlayıcının doğrudan akış örüntüsüyle oluşturulup ayrıca ses çerçeveleriyle sınandı; resmi sayfadaki HLS adresinin aynısı oldukları iddia edilmez.
- [Kral Müzik](https://www.kralmuzik.com.tr/), [Best FM](https://www.bestfm.com.tr/), [Number1](https://www.numberone.com.tr/), Kafa Radyo resmi oynatıcı kodu ve doğrulanan alternatif dizin adresi.

Ham adres kontrol kayıtları proje içinde `display-test/turkiye-radyo/` altındadır. Cihaz yazılımı ve cihazdaki mevcut istasyon listesi değiştirilmedi.

Cihazda doğrulananlar: Radyo 7, Süper FM, TRT FM ve Radyo Spor; ayrıca 32 istasyonlu liste, 4. sayfa ve 32→1 dönüşü. [Cihaz test kaydı](CIHAZ-TESTLERI.md). Diğer yayınlar cihazda henüz doğrulanmadı.
