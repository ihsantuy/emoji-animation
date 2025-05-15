/*MIT License

Copyright (c) [Tahun] [Nama Pemilik Hak Cipta]

Dengan ini diberikan izin secara cuma-cuma kepada siapa pun yang mendapatkan salinan
perangkat lunak ini dan file dokumentasi terkait ("Perangkat Lunak"), untuk berurusan
dengan Perangkat Lunak tanpa batasan, termasuk tanpa batasan hak untuk menggunakan,
menyalin, memodifikasi, menggabungkan, menerbitkan, mendistribusikan, mensublisensikan, dan/atau menjual
salinan Perangkat Lunak, dan untuk mengizinkan orang yang Perangkat Lunaknya
disediakan untuk melakukannya, tunduk pada persyaratan berikut:

Pemberitahuan hak cipta di atas dan pemberitahuan izin ini harus disertakan dalam semua
salinan atau bagian substansial dari Perangkat Lunak.

PERANGKAT LUNAK INI DISEDIAKAN "SEBAGAIMANA ADANYA", TANPA JAMINAN APA PUN, TERSURAT MAUPUN
TERSIRAT, TERMASUK NAMUN TIDAK TERBATAS PADA JAMINAN DIPERDAGANGKAN, KESESUAIAN UNTUK
TUJUAN TERTENTU DAN NON-PELANGGARAN. DALAM KEADAAN APA PUN PEMILIK HAK CIPTA ATAU
PEMEGANG HAK CIPTA BERTANGGUNG JAWAB ATAS KLAIM, KERUSAKAN ATAU TANGGUNG JAWAB LAIN, BAIK DALAM
TINDAKAN KONTRAK, KESALAHAN ATAU LAINNYA, YANG TIMBUL DARI, KELUAR DARI ATAU SEHUBUNGAN
DENGAN PERANGKAT LUNAK ATAU PENGGUNAAN ATAU URUSAN LAIN DALAM PERANGKAT LUNAK.*/
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Definisi bitmap untuk frame emoji senyum dengan mata terbuka
static const unsigned char PROGMEM smile_open_eyes[] = {
  B00000000, B00000000,
  B00111100, B11111100,
  B01000010, B00000010,
  B10000001, B00000001,
  B10000001, B00000001,
  B01000010, B00000010,
  B00111100, B11111100,
  B00000000, B00000000
};

// Definisi bitmap untuk frame emoji senyum dengan mata tertutup
static const unsigned char PROGMEM smile_closed_eyes[] = {
  B00000000, B00000000,
  B00111100, B11111100,
  B01111110, B00000010,
  B10111101, B00000001,
  B10111101, B00000001,
  B01111110, B00000010,
  B00111100, B11111100,
  B00000000, B00000000
};

// Definisi bitmap untuk frame emoji senyum dengan mata setengah tertutup
static const unsigned char PROGMEM smile_half_closed_eyes[] = {
  B00000000, B00000000,
  B00111100, B11111100,
  B01100010, B00000010,
  B10100001, B00000001,
  B10100001, B00000001,
  B01100010, B00000010,
  B00111100, B11111100,
  B00000000, B00000000
};

// Array yang berisi pointer ke setiap frame emoji untuk animasi berkedip
const unsigned char* blink_frames[] PROGMEM = {
  smile_open_eyes,
  smile_half_closed_eyes,
  smile_closed_eyes,
  smile_half_closed_eyes,
  smile_open_eyes
};
const int num_blink_frames = sizeof(blink_frames) / sizeof(blink_frames[0]);
int current_blink_frame = 0;
unsigned long last_blink_frame_time = 0;
const long blink_frame_delay = 150; // Delay antar frame kedipan dalam milisekon

void setup() {
  Serial.begin(115200);
  Wire.begin(5, 4); // Inisialisasi I2C dengan SDA di GPIO5 (D1) dan SCL di GPIO4 (D2)
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.display(); // Tampilkan layar kosong awal
}

void loop() {
  if (millis() - last_blink_frame_time > blink_frame_delay) {
    display.clearDisplay();
    display.draw