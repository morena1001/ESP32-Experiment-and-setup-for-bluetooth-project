// #include <Arduino.h>
// #include <Wire.h>
// #include <Adafruit_Si4713.h>
// #include <Preferences.h>
// #include <BluetoothA2DPSink.h>
// #include <driver/dac.h>
// #include <SD.h>
// #include <TFT_eSPI.h>

// #include "main.h"

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
// const int func_1_pin = 36;
// const int func_2_pin = 39;
// const int func_3_pin = 34;
// const int func_4_pin = 35;

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

// uint16_t radio_station = 9390;
// uint8_t  phone_volume  = 127;
// uint8_t  screen_brightness = 127;

// String song_title = "";
// String song_artist = "";
// String song_album = "";
// unsigned long temp_text_timer[2] = {0, 0};
// uint8_t function;

// inline bool Between (uint32_t enc_byte, uint32_t low, uint32_t high);
// uint8_t Check_Location (uint32_t enc_byte);
// void Load_Intro_Screen ();
// void Print_Song_Data ();
// void Print_Temp_Text (String text, uint32_t ms = DEFAULT_TEMP_TEXT_TIME);
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

// void setup () {
//     Serial.begin (115200);

//     // SET UP RADIO
//     Serial.println ("Starting radio");

//     while (!radio.begin ())     Serial.println ("Could not find radio");
//     Serial.println ("Radio found");

//     radio.setTXpower (115);
//     Serial.printf ("Tuning into %d.%d\n", radio_station / 100, radio_station % 100);
//     radio.tuneFM (radio_station);
    
//     // SET UP FONTS IN SD CARD
//     while (!SD.begin (2))   Serial.println ("Card mount failed");
//     Serial.println ("Card mount success");

//     if (SD.cardType () == CARD_NONE) {
//         Serial.println ("No SD card attached");
//         return;
//     }

//     // SET UP SCREEN
//     tft.init ();
//     tft.setRotation (1);
//     tft.fillScreen (TFT_BLACK);
//     tft.setTextColor (TFT_WHITE);
//     Serial.println ("TFT initialization done");
//     Load_Intro_Screen ();
    

//     // SET UP buttons and switches
//     pinMode (pp_pin, INPUT);
//     pinMode (next_pin, INPUT);
//     pinMode (prev_pin, INPUT);
//     pinMode (func_1_pin, INPUT);
//     pinMode (func_2_pin, INPUT);
//     pinMode (func_3_pin, INPUT);
//     pinMode (func_4_pin, INPUT);

//     // SET UP A2DP SINK
//     a2dp_sink.set_avrc_metadata_attribute_mask (ESP_AVRC_MD_ATTR_TITLE | ESP_AVRC_MD_ATTR_ARTIST | ESP_AVRC_MD_ATTR_ALBUM );
//     a2dp_sink.set_avrc_metadata_callback (avrc_metadata_callback);
//     a2dp_sink.set_avrc_rn_playstatus_callback (avrc_rn_playstatus_callback);

//     a2dp_sink.start ("VS transmitter", true);
//     Serial.println ("Started bluetooth");

//     delay (1000);

//     // PRINT OUT FUNCTION ICONS AND NOT CONNECTED TEXT
//     tft.fillScreen (TFT_BLACK);
//     tft.setCursor (85, 220);
//     tft.loadFont (file_functions_symbols, SD);
//     tft.print (PLAY_PAUSE_ICON);

//     if (digitalRead (func_1_pin)) {
//         function = PLAYBACK_FUNCTION;
//         tft.setCursor (10, 220);
//         tft.print (PREVIOUS_ICON);
//         tft.setCursor (160, 220);
//         tft.print (NEXT_ICON);
//     }
//     else if (digitalRead (func_2_pin)) {
//         function = STATION_FUNCTION;
//         tft.setCursor (10, 220);
//         tft.print (LEFT_ICON);
//         tft.setCursor (160, 220);
//         tft.print (RIGHT_ICON);
//     }
//     else if (digitalRead (func_3_pin)) {
//         function = BRIGHTNESS_FUNCTION;
//         tft.setCursor (10, 220);
//         tft.print ("🌣");
//         tft.setCursor (160, 220);
//         tft.print ("☼");
//     }
//     else if (digitalRead (func_4_pin)) {
//         function = VOLUME_FUNCTION;
//         tft.setCursor (10, 220);
//         tft.print ("🔉");
//         tft.setCursor (160, 220);
//         tft.print ("🔊");
//     }
//     tft.unloadFont ();
//     Serial.println (function);

//     tft.loadFont (file_main_latin, SD);
//     tft.setCursor (10, 70);
//     tft.print ("NOT CONNECTED");
//     tft.unloadFont ();

//     preferences.begin ("Connections", true);
//     bool prev_init = preferences.isKey ("prev");
//     if (prev_init) {
//         esp_bd_addr_t prev_addr;
//         preferences.getBytes ("prev", prev_addr, ESP_BD_ADDR_LEN);
//         Serial.printf ("Checking to see if %s connects\n", a2dp_sink.to_str (prev_addr));
        
//         unsigned long start = millis ();
//         while (millis () - start <= 5000) {
//             if (a2dp_sink.connect_to (prev_addr)) {
//                 Serial.printf ("Reconnected to previously connected device: %s\n", a2dp_sink.to_str (prev_addr));
//                 a2dp_sink.set_volume (phone_volume);
//                 Print_Temp_Text ("Connected");
//                 break;
//             }
//         }
//         if (millis () - start > 5000) {
//             tft.loadFont (file_main_latin, SD);
//             tft.setCursor (10, 70);
//             tft.print ("                ");
//             tft.unloadFont ();
//         }
//     }
//     preferences.end ();
// }

// void loop () {
//     if (temp_text_timer[1] != 0 && millis () - temp_text_timer[0] > temp_text_timer[1]) {
//         Print_Song_Data ();
//         temp_text_timer[1] = 0;
//     }


//     // // Read from the input button
//     // int pp_reading = digitalRead (pp_pin);
//     // int next_reading = digitalRead (next_pin);
//     // int prev_reading = digitalRead (prev_pin);

//     // // If switch is changed due to noise or pressing, reset debounce timer
//     // if (pp_reading != pp_last_state)       pp_last_db_time = millis ();
//     // if (next_reading != next_last_state)   next_last_db_time = millis ();
//     // if (prev_reading != prev_last_state)   prev_last_db_time = millis ();

//     // // If debounce delay is reached, update the state of the button, and if high, send the message
//     // if ((millis () - pp_last_db_time) > pp_db_delay) {
//     //     if (pp_reading != pp_state) {
//     //     pp_state = pp_reading;
        
//     //     if (pp_state == HIGH) {
//     //         is_active = !is_active;
//     //         if (is_active) {
//     //         Serial.println ("play");
//     //         a2dp_sink.play ();
//     //         } else {
//     //         Serial.println ("pause");
//     //         a2dp_sink.pause ();
//     //         }
//     //     }
//     //     }
//     // }

//     // if ((millis () - next_last_db_time) > next_db_delay) {
//     //     if (next_reading != next_state) {
//     //     next_state = next_reading;
        
//     //     if (next_state == HIGH) {
//     //         Serial.println ("Next");
//     //         a2dp_sink.next ();
//     //     }
//     //     }
//     // }

//     // if ((millis () - prev_last_db_time) > prev_db_delay) {
//     //     if (prev_reading != prev_state) {
//     //     prev_state = prev_reading;
        
//     //     if (prev_state == HIGH) {
//     //         Serial.println ("Previous");
//     //         a2dp_sink.previous ();
//     //     }
//     //     }
//     // }
    
//     // // Update previous button state
//     // pp_last_state = pp_reading;
//     // next_last_state = next_reading;
//     // prev_last_state = prev_reading;

//     // // Send a device conntected message once if a device is connected
//     // if (a2dp_sink.is_connected () && !sent_connected_message) {
//     //     sent_connected_message = true;
//     //     sent_disconnected_message = false;
//     //     Serial.println ("Device connected");
//     //     a2dp_sink.set_volume (phone_volume);

//     //     preferences.begin ("connections", false);
//     //     preferences.putBytes ("prev", a2dp_sink.get_current_peer_address (), ESP_BD_ADDR_LEN);
//     //     Serial.printf ("Put address to memory: %s\n", a2dp_sink.to_str (*(a2dp_sink.get_current_peer_address ())));
//     //     preferences.end ();
//     // } else if (!a2dp_sink.is_connected () && !sent_disconnected_message) {
//     //     sent_disconnected_message = true;
//     //     sent_connected_message = false;
//     //     Serial.println ("Device disconnected");
//     // }
// }

// inline bool Between (uint32_t enc_byte, uint32_t low, uint32_t high) {
//     return (enc_byte >= low && enc_byte <= high);
// }

// uint8_t Check_Location (uint32_t enc_byte) {
//     if (enc_byte == 0)  return FONT_ERROR;
//     if (Between (enc_byte, 0x0, 0x7F))                  return latin;
//     if (Between (enc_byte, 0xCDB0, 0xDEBF))             return cyrillic_greek;
//     if (Between (enc_byte, 0xC280, 0xC98F))             return ex_latin;
//     if (Between (enc_byte, 0xE38180, 0xE383BF))         return japanese;
//     if (Between (enc_byte, 0xE18480, 0xE187BF))         return hangul_1;
//     if (Between (enc_byte, 0xE384B1, 0xE3868E))         return hangul_2;
//     if (Between (enc_byte, 0xEAB080, 0xEB8FBF))         return hangul_3_1;
//     if (Between (enc_byte, 0xEB9080, 0xEBAFBF))         return hangul_3_2;
//     if (Between (enc_byte, 0xEBB080, 0xEC8FBF))         return hangul_4_1;
//     if (Between (enc_byte, 0xEC9080, 0xECAFBF))         return hangul_4_2;
//     if (Between (enc_byte, 0xECB080, 0xED8FBF))         return hangul_5_1;
//     if (Between (enc_byte, 0xED9080, 0xED9EA3))         return hangul_5_2;
//     if (Between (enc_byte, 0xE38080, 0xE380BF))         return cj_1;
//     if (Between (enc_byte, 0xE39080, 0xE3AFBF))         return cj_2_1;
//     if (Between (enc_byte, 0xE3B080, 0xE48FBF))         return cj_2_2;
//     if (Between (enc_byte, 0xE49080, 0xE4B6BF))         return cj_3;
//     if (Between (enc_byte, 0xE4B880, 0xE597BF))         return cj_4_1;
//     if (Between (enc_byte, 0xE59880, 0xE5B7BF))         return cj_4_2;
//     if (Between (enc_byte, 0xE5B880, 0xE697BF))         return cj_5_1;
//     if (Between (enc_byte, 0xE69880, 0xE6B7BF))         return cj_5_2;
//     if (Between (enc_byte, 0xE6B880, 0xE797BF))         return cj_6_1;
//     if (Between (enc_byte, 0xE79880, 0xE7B7BF))         return cj_6_2;
//     if (Between (enc_byte, 0xE7B880, 0xE897BF))         return cj_7_1;
//     if (Between (enc_byte, 0xE89880, 0xE8B7BF))         return cj_7_2;
//     if (Between (enc_byte, 0xE8B880, 0xE997BF))         return cj_8_1;
//     if (Between (enc_byte, 0xE99880, 0xE9B7BF))         return cj_8_2;
//     if (Between (enc_byte, 0xE9B880, 0xE9BFBF))         return cj_9;
//     if (Between (enc_byte, 0xEFA480, 0xEFA9AD))         return cj_10;
//     if (Between (enc_byte, 0xF0A08080, 0xF0ADBFBF))     return cj_11;
//     if (Between (enc_byte, 0xF0B08080, 0xF0B1BFBF))     return cj_12;
//     return FONT_ERROR;
// }

// void Load_Intro_Screen () {
//     tft.loadFont (file_main_latin, SD);
//     tft.setCursor (51, 90);
//     tft.print ("Vandar Systems");
//     tft.setCursor (56, 120);
//     tft.print ("FM Transmitter");
//     tft.unloadFont ();
// }

// void Print_Song_Data () {
    
// }

// void Print_Temp_Text (String text, uint32_t ms = DEFAULT_TEMP_TEXT_TIME) {
//     tft.loadFont (file_main_latin, SD);
//     tft.setCursor (10, 70);
//     tft.print ("                ");
//     tft.setCursor (10, 70);
//     tft.print (text);
//     tft.unloadFont ();

//     temp_text_timer[0] = millis ();
//     temp_text_timer[1] = ms;
// }
