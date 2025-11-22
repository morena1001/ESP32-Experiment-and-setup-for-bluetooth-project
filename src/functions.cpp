// #include <Arduino.h>
// #include <Wire.h>
// #include <Adafruit_Si4713.h>
// #include <Preferences.h>
// #include <BluetoothA2DPSink.h>
// #include <driver/dac.h>
// #include <SD.h>
// #include <TFT_eSPI.h>

// #define RESETPIN 13
// #define FONT_ERROR  0
// #define CJK11       1
// #define CJK10       2
// #define CJK9        3
// #define CJK8        4
// #define CJK7        5
// #define CJK6        6
// #define CJK5        7
// #define CJK4        8
// #define CJK3        9
// #define CJK2        10
// #define CJK1        11
// #define LAT1        12
// #define LAT2        13

// Adafruit_Si4713 radio = Adafruit_Si4713 (RESETPIN);
// AnalogAudioStream out;
// BluetoothA2DPSink a2dp_sink (out);
// Preferences preferences;
// TFT_eSPI tft = TFT_eSPI ();

// bool is_active = true;
// bool sent_connected_message = false;
// bool sent_disconnected_message = false;

// const int pp_pin = 32; // GPIO pin for the pause/play button
// const int next_pin = 33; // GPIO pin for the next button
// const int prev_pin = 27; // GPIO pin for the previous button

// unsigned long pp_last_db_time = 0;
// unsigned long next_last_db_time = 0;
// unsigned long prev_last_db_time = 0;

// unsigned long pp_db_delay = 50;
// unsigned long next_db_delay = 50;
// unsigned long prev_db_delay = 50;

// int pp_state;            
// int next_state;            
// int prev_state;            

// int pp_last_state = LOW;  
// int next_last_state = LOW;  
// int prev_last_state = LOW; 

// uint8_t Check_Location (uint32_t enc_byte);
// void avrc_metadata_callback (uint8_t id, const uint8_t *text) {
//   Serial.printf ("==> AVRC metadata rsp: attribute id 0x%x, %s\n", id, text);
// }

// void avrc_rn_playstatus_callback (esp_avrc_playback_stat_t playback) {
//   switch (playback) {
//     case esp_avrc_playback_stat_t::ESP_AVRC_PLAYBACK_PAUSED:
//       // Serial.println ("Stopped.");
//       is_active = false;
//       break;
//     case esp_avrc_playback_stat_t::ESP_AVRC_PLAYBACK_PLAYING:
//       // Serial.println ("Playing.");
//       is_active = true;
//       break;
//     default:
//       Serial.printf ("Got unknown playback status %d\n", playback);
//   }
// }
