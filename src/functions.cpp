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
// volatile uint8_t func_1_state = LOW;
// volatile uint8_t func_2_state = LOW;
// volatile uint8_t func_3_state = LOW;
// volatile uint8_t func_4_state = LOW;
// uint8_t func_1_last_state = LOW;
// uint8_t func_2_last_state = LOW;
// uint8_t func_3_last_state = LOW;
// uint8_t func_4_last_state = LOW;
// inline bool Between (uint32_t enc_byte, uint32_t low, uint32_t high);
// uint8_t Check_Location (uint32_t enc_byte);
// void Load_Intro_Screen ();
// void Print_Song_Data ();
// void Print_Perma_Text (String text);
// void Print_Text (uint8_t song_type);
// void Print_Temp_Text (String text, uint32_t ms);
// void Change_Function ();
// void Left_Button ();
// void Middle_Button ();
// void Right_Button ();
// void ARDUINO_ISR_ATTR Function_1_Switch ();
// void ARDUINO_ISR_ATTR Function_2_Switch ();
// void ARDUINO_ISR_ATTR Function_3_Switch ();
// void ARDUINO_ISR_ATTR Function_4_Switch ();

// void avrc_metadata_callback (uint8_t id, const uint8_t *text) {
//     if (a2dp_sink.is_connected ()) {
//         Serial.printf ("==> AVRC metadata rsp: attribute id 0x%x, %s\n", id, text);

//         switch (id) {
//             case ESP_AVRC_MD_ATTR_TITLE:
//                 if (song_title == String ((char *) text))    break;    
//                 song_title = String ((char*) text); 
//                 if (temp_text_timer[1] == 0 && sent_connected_message)    Print_Text (TEXT_TYPE_SONG_TITLE);
//             break;
//             case ESP_AVRC_MD_ATTR_ARTIST:   
//                 if (song_artist == String ((char *) text))    break;  
//                 song_artist = String ((char*) text);
//                 if (temp_text_timer[1] == 0 && sent_connected_message)    Print_Text (TEXT_TYPE_SONG_ARTIST);
//             break;
//             case ESP_AVRC_MD_ATTR_ALBUM:    
//                 if (song_album == String ((char *) text))    break;  
//                 song_album = String ((char*) text);
//                 if (temp_text_timer[1] == 0 && sent_connected_message)    Print_Text (TEXT_TYPE_SONG_ALBUM);
//             break;
//         }
//     }
// }

// void avrc_rn_playstatus_callback (esp_avrc_playback_stat_t playback) {
//     switch (playback) {
//     case esp_avrc_playback_stat_t::ESP_AVRC_PLAYBACK_PAUSED:
//         // Serial.println ("Stopped.");
//         is_active = false;
//         break;
//     case esp_avrc_playback_stat_t::ESP_AVRC_PLAYBACK_PLAYING:
//         // Serial.println ("Playing.");
//         is_active = true;
//         break;
//     default:
//         Serial.printf ("Got unknown playback status %d\n", playback);
//     }
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

//     // SET UP SCREEN
//     tft.init ();
//     tft.setRotation (1);
//     tft.fillScreen (TFT_BLACK);
//     tft.setTextColor (TFT_WHITE);
//     Serial.println ("TFT initialization done");
//     Load_Intro_Screen ();
    

//     // SET UP buttons, switches, and interrupts
//     pinMode (pp_pin, INPUT);
//     pinMode (next_pin, INPUT);
//     pinMode (prev_pin, INPUT);
//     pinMode (func_1_pin, INPUT);
//     pinMode (func_2_pin, INPUT);
//     pinMode (func_3_pin, INPUT);
//     pinMode (func_4_pin, INPUT);

//     // attachInterrupt (pp_pin, Middle_Button, CHANGE);
//     // attachInterrupt (next_pin, Left_Button, CHANGE);
//     // attachInterrupt (prev_pin, Right_Button, CHANGE);
//     attachInterrupt (func_1_pin, Function_1_Switch, RISING);
//     attachInterrupt (func_2_pin, Function_2_Switch, RISING);
//     attachInterrupt (func_3_pin, Function_3_Switch, RISING);
//     attachInterrupt (func_4_pin, Function_4_Switch, RISING);

//     // SET UP A2DP SINK
//     a2dp_sink.set_avrc_metadata_attribute_mask (ESP_AVRC_MD_ATTR_TITLE | ESP_AVRC_MD_ATTR_ARTIST | ESP_AVRC_MD_ATTR_ALBUM);
//     a2dp_sink.set_avrc_metadata_callback (avrc_metadata_callback);
//     a2dp_sink.set_avrc_rn_playstatus_callback (avrc_rn_playstatus_callback);

//     a2dp_sink.start ("VS transmitter", true);
//     Serial.println ("Started bluetooth");

//     delay (1000);

//     // PRINT OUT FUNCTION ICONS AND NOT CONNECTED TEXT
//     tft.fillScreen (TFT_BLACK);
//     tft.setCursor (85, 215);
//     tft.loadFont (file_functions_symbols, SD);
//     tft.print (PLAY_PAUSE_ICON);

//     if (digitalRead (func_1_pin)) {
//         function = PLAYBACK_FUNCTION;
//         func_1_state = HIGH;
//         tft.setCursor (10, 215);
//         tft.print (PREVIOUS_ICON);
//         tft.setCursor (160, 215);
//         tft.print (NEXT_ICON);
//     }
//     else if (digitalRead (func_2_pin)) {
//         function = STATION_FUNCTION;
//         func_2_state = HIGH;
//         tft.setCursor (10, 215);
//         tft.print (LEFT_ICON);
//         tft.setCursor (160, 215);
//         tft.print (RIGHT_ICON);
//     }
//     else if (digitalRead (func_3_pin)) {
//         function = BRIGHTNESS_FUNCTION;
//         func_3_state = HIGH;
//         tft.setCursor (10, 215);
//         tft.print (DIMMER_SCREEN_ICON);
//         tft.setCursor (160, 215);
//         tft.print (BRIGHTER_SCREEN_ICON);
//     }
//     else if (digitalRead (func_4_pin)) {
//         function = VOLUME_FUNCTION;
//         func_4_state = HIGH;
//         tft.setCursor (10, 215);
//         tft.print (LOWER_VOL_ICON);
//         tft.setCursor (160, 215);
//         tft.print (HIGHER_VOL_ICON);
//     }
//     tft.unloadFont ();

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
//                 Print_Temp_Text ("CONNECTED", DEFAULT_TEMP_TEXT_TIME);
//                 sent_connected_message = true;
//                 break;
//             }
//         }
//     }
//     preferences.end ();
// }

// void loop () {
//     if (temp_text_timer[1] != 0 && millis () - temp_text_timer[0] > temp_text_timer[1]) {
//         Print_Song_Data ();
//         temp_text_timer[1] = 0;
//     }

//     // if (function != previous_function)  prev_change = millis ();
//     // if ((millis () - prev_change) > prev_change_delay)  Change_Function ();

//     if ()
//     if (function == PLAYBACK_FUNCTION && previous_function != function) {
//         function = PLAYBACK_FUNCTION;
//         tft.loadFont (file_functions_symbols, SD);
//         tft.fillRect (10, 215, 30, 30, TFT_BLACK);
//         tft.setCursor (10, 215);
//         tft.print (PREVIOUS_ICON);
//         tft.fillRect (160, 215, 30, 30, TFT_BLACK);
//         tft.setCursor (160, 215);
//         tft.print (NEXT_ICON);
//         tft.unloadFont ();
//         Serial.println ("APPLE ");
//     }
//     else if (function == STATION_FUNCTION && previous_function != function) {
//         previous_function = function;
//         tft.loadFont (file_functions_symbols, SD);
//         tft.fillRect (10, 215, 30, 30, TFT_BLACK);
//         tft.setCursor (10, 215);
//         tft.print (LEFT_ICON);
//         tft.fillRect (160, 215, 30, 30, TFT_BLACK);
//         tft.setCursor (160, 215);
//         tft.print (RIGHT_ICON);
//         tft.unloadFont ();
//         Serial.println ("PIZZA");
//     }
//     else if (function == BRIGHTNESS_FUNCTION && previous_function != function) {
//         previous_function = function;
//         tft.loadFont (file_functions_symbols, SD);
//         tft.fillRect (10, 215, 30, 30, TFT_BLACK);
//         tft.setCursor (10, 215);
//         tft.print (DIMMER_SCREEN_ICON);
//         tft.fillRect (160, 215, 30, 30, TFT_BLACK);
//         tft.setCursor (160, 215);
//         tft.print (BRIGHTER_SCREEN_ICON);
//         tft.unloadFont ();
//         Serial.println ("LITERAL");
//     }
//     else if (function == VOLUME_FUNCTION && previous_function != function) {
//         previous_function = function;
//         tft.loadFont (file_functions_symbols, SD);
//         tft.fillRect (10, 215, 30, 30, TFT_BLACK);
//         tft.setCursor (10, 215);
//         tft.print (LOWER_VOL_ICON);
//         tft.fillRect (160, 215, 30, 30, TFT_BLACK);
//         tft.setCursor (160, 215);
//         tft.print (HIGHER_VOL_ICON);
//         tft.unloadFont ();
//         Serial.println ("WHYYYYY");
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

//     // Send a device conntected message once if a device is connected
//     if (a2dp_sink.is_connected () && !sent_connected_message) {
//         sent_connected_message = true;
//         sent_disconnected_message = false;
//         Serial.println ("Device connected");
//         Print_Temp_Text ("CONNECTED", DEFAULT_TEMP_TEXT_TIME);
//         a2dp_sink.set_volume (phone_volume);

//         preferences.begin ("connections", false);
//         preferences.putBytes ("prev", a2dp_sink.get_current_peer_address (), ESP_BD_ADDR_LEN);
//         Serial.printf ("Put address to memory: %s\n", a2dp_sink.to_str (*(a2dp_sink.get_current_peer_address ())));
//         preferences.end ();
//     } else if (!a2dp_sink.is_connected () && !sent_disconnected_message) {
//         sent_disconnected_message = true;
//         sent_connected_message = false;
//         Print_Perma_Text ("NOT CONNECTED");
//         Serial.println ("Device disconnected");
//     }
// }

// void Left_Button () {

// }

// void Middle_Button () {

// }

// void Right_Button () {

// }

// void ARDUINO_ISR_ATTR Function_1_Switch () {
//     func_1_state = HIGH;
// }

// void ARDUINO_ISR_ATTR Function_2_Switch () {
//     func_2_state = HIGH;
// }

// void ARDUINO_ISR_ATTR Function_3_Switch () {
//     func_3_state = HIGH;
// }

// void ARDUINO_ISR_ATTR Function_4_Switch () {
//     func_4_state = HIGH;
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
//     tft.fillRect (0, 20, 320, 115, TFT_BLACK);
//     Print_Text (TEXT_TYPE_SONG_TITLE);
//     Print_Text (TEXT_TYPE_SONG_ALBUM);
//     Print_Text (TEXT_TYPE_SONG_ARTIST);
// }

// void Print_Perma_Text (String text) {
//     tft.loadFont (file_main_latin, SD);
//     tft.fillRect (0, 70, 320, 65, TFT_BLACK);
//     tft.setCursor (10, 70);
//     tft.print (text);
//     tft.unloadFont ();
// }

// void Print_Temp_Text (String text, uint32_t ms) {
//     Serial.println ("STARTING TEMP TEXT");
//     tft.loadFont (file_main_latin, SD);
//     tft.fillRect (0, 70, 320, 65, TFT_BLACK);
//     tft.setCursor (10, 70);
//     tft.print (text);
//     tft.unloadFont ();

//     temp_text_timer[0] = millis ();
//     temp_text_timer[1] = ms;
// }

// void Print_Text (uint8_t song_type) {
//     String text = "";
//     bool maintext = false;

//     switch (song_type) {
//         case TEXT_TYPE_SONG_TITLE:
//             text = song_title;
//             tft.fillRect (0, 60, 320, 50, TFT_BLACK);
//             tft.setCursor (10, 70);
//             maintext = true;
//             break;
//         case TEXT_TYPE_SONG_ALBUM:
//             text = song_album;
//             tft.fillRect (0, 135, 320, 40, TFT_BLACK);
//             tft.setCursor (20, 135);
//             break;
            
//             case TEXT_TYPE_SONG_ARTIST:
//             text = song_artist;
//             tft.fillRect (0, 110, 320, 40, TFT_BLACK);
//             tft.setCursor (20, 110);
//             break;
//     }

//     uint32_t val = 0;
    
//     for (int i = 0; i < text.length (); ) {
//         val = (0xFF & static_cast <uint32_t> (text[i]));

//         if (val >= 0xF0) {
//             val <<= 24;
//             val += (0xFF0000 & (static_cast <uint32_t> (text[i + 1]) << 16)) + (0xFF00 & (static_cast <uint32_t> (text[i + 2]) << 8)) + (0xFF & static_cast <uint32_t> (text[i + 3]));
//         } else if (val >= 0xE0) {
//             val <<= 16;
//             val += (0xFF00 & (static_cast <uint32_t> (text[i + 1]) << 8)) + (0xFF & static_cast <uint32_t> (text[i + 2]));
//         } else if (val >= 0xC0) {
//             val <<= 2;
//             val += (0xFF & static_cast <uint32_t> (text[i + 1]));
//         }

//         switch (CHECK_LOCATION (val)) {
//             case latin: {
//                 tft.loadFont (maintext ? file_main_latin : file_sub_latin, SD);
//                 tft.print (text.substring (i, i + 1));
//                 tft.unloadFont ();
//                 i++;
//             } break;

//             case cyrillic_greek: {
//                 tft.loadFont (maintext ? file_main_cyrillic_greek : file_sub_cyrillic_greek, SD);
//                 tft.print (text.substring (i, i + 2));
//                 tft.unloadFont ();
//                 i += 2;
//             } break;

//             case ex_latin: {
//                 tft.loadFont (maintext ? file_main_ex_latin : file_sub_ex_latin, SD);
//                 tft.print (text.substring (i, i + 2));
//                 tft.unloadFont ();
//                 i += 2;
//             } break;

//             case japanese: {
//                 tft.loadFont (maintext ? file_main_japanese : file_sub_japanese, SD);
//                 tft.print (text.substring (i, i + 3));
//                 tft.unloadFont ();
//                 i += 3;
//             } break;

//             case hangul_1: {
//                 tft.loadFont (maintext ? file_main_hangul_1 : file_sub_hangul_1, SD);
//                 tft.print (text.substring (i, i + 3));
//                 tft.unloadFont ();
//                 i += 3;
//             } break;

//             case hangul_2: {
//                 tft.loadFont (maintext ? file_main_hangul_2 : file_sub_hangul_2, SD);
//                 tft.print (text.substring (i, i + 3));
//                 tft.unloadFont ();
//                 i += 3;
//             } break;

//             case hangul_3_1: {
//                 tft.loadFont (maintext ? file_main_hangul_3_1 : file_sub_hangul_3, SD);
//                 tft.print (text.substring (i, i + 3));
//                 tft.unloadFont ();
//                 i += 3;
//             } break;

//             case hangul_3_2: {
//                 tft.loadFont (maintext ? file_main_hangul_3_2 : file_sub_hangul_3, SD);
//                 tft.print (text.substring (i, i + 3));
//                 tft.unloadFont ();
//                 i += 3;
//             } break;

//             case hangul_4_1: {
//                 tft.loadFont (maintext ? file_main_hangul_4_1 : file_sub_hangul_4, SD);
//                 tft.print (text.substring (i, i + 3));
//                 tft.unloadFont ();
//                 i += 3;
//             } break;
            
//             case hangul_4_2: {
//                 tft.loadFont (maintext ? file_main_hangul_4_2 : file_sub_hangul_4, SD);
//                 tft.print (text.substring (i, i + 3));
//                 tft.unloadFont ();
//                 i += 3;
//             } break;

//             case hangul_5_1: {
//                 tft.loadFont (maintext ? file_main_hangul_5_1 : file_sub_hangul_5, SD);
//                 tft.print (text.substring (i, i + 3));
//                 tft.unloadFont ();
//                 i += 3;
//             } break;

//             case hangul_5_2: {
//                 tft.loadFont (maintext ? file_main_hangul_5_2 : file_sub_hangul_5, SD);
//                 tft.print (text.substring (i, i + 3));
//                 tft.unloadFont ();
//                 i += 3;
//             } break;

//             case cj_1: {
//                 tft.loadFont (maintext ? file_main_cj_1 : file_sub_cj_1, SD);
//                 tft.print (text.substring (i, i + 3));
//                 tft.unloadFont ();
//                 i += 3;
//             } break;

//             case cj_2_1: {
//                 tft.loadFont (maintext ? file_main_cj_2_1 : file_sub_cj_2, SD);
//                 tft.print (text.substring (i, i + 3));
//                 tft.unloadFont ();
//                 i += 3;
//             } break;

//             case cj_2_2: {
//                 tft.loadFont (maintext ? file_main_cj_2_2 : file_sub_cj_2, SD);
//                 tft.print (text.substring (i, i + 3));
//                 tft.unloadFont ();
//                 i += 3;
//             } break;

//             case cj_3: {
//                 tft.loadFont (maintext ? file_main_cj_3 : file_sub_cj_3, SD);
//                 tft.print (text.substring (i, i + 3));
//                 tft.unloadFont ();
//                 i += 3;
//             } break;

//             case cj_4_1: {
//                 tft.loadFont (maintext ? file_main_cj_4_1 : file_sub_cj_4, SD);
//                 tft.print (text.substring (i, i + 3));
//                 tft.unloadFont ();
//                 i += 3;
//             } break;

//             case cj_4_2: {
//                 tft.loadFont (maintext ? file_main_cj_4_2 : file_sub_cj_4, SD);
//                 tft.print (text.substring (i, i + 3));
//                 tft.unloadFont ();
//                 i += 3;
//             } break;

//             case cj_5_1: {
//                 tft.loadFont (maintext ? file_main_cj_5_1 : file_sub_cj_5, SD);
//                 tft.print (text.substring (i, i + 3));
//                 tft.unloadFont ();
//                 i += 3;
//             } break;

//             case cj_5_2: {
//                 tft.loadFont (maintext ? file_main_cj_5_2 : file_sub_cj_5, SD);
//                 tft.print (text.substring (i, i + 3));
//                 tft.unloadFont ();
//                 i += 3;
//             } break;

//             case cj_6_1: {
//                 tft.loadFont (maintext ? file_main_cj_6_1 : file_sub_cj_6, SD);
//                 tft.print (text.substring (i, i + 3));
//                 tft.unloadFont ();
//                 i += 3;
//             } break;

//             case cj_6_2: {
//                 tft.loadFont (maintext ? file_main_cj_6_2 : file_sub_cj_6, SD);
//                 tft.print (text.substring (i, i + 3));
//                 tft.unloadFont ();
//                 i += 3;
//             } break;

//             case cj_7_1: {
//                 tft.loadFont (maintext ? file_main_cj_7_1 : file_sub_cj_7, SD);
//                 tft.print (text.substring (i, i + 3));
//                 tft.unloadFont ();
//                 i += 3;
//             } break;

//             case cj_7_2: {
//                 tft.loadFont (maintext ? file_main_cj_7_2 : file_sub_cj_7, SD);
//                 tft.print (text.substring (i, i + 3));
//                 tft.unloadFont ();
//                 i += 3;
//             } break;

//             case cj_8_1: {
//                 tft.loadFont (maintext ? file_main_cj_8_1 : file_sub_cj_8, SD);
//                 tft.print (text.substring (i, i + 3));
//                 tft.unloadFont ();
//                 i += 3;
//             } break;

//             case cj_8_2: {
//                 tft.loadFont (maintext ? file_main_cj_8_2 : file_sub_cj_8, SD);
//                 tft.print (text.substring (i, i + 3));
//                 tft.unloadFont ();
//                 i += 3;
//             } break;

//             case cj_9: {
//                 tft.loadFont (maintext ? file_main_cj_9 : file_sub_cj_9, SD);
//                 tft.print (text.substring (i, i + 3));
//                 tft.unloadFont ();
//                 i += 3;
//             } break;

//             case cj_10: {
//                 tft.loadFont (maintext ? file_main_cj_10 : file_sub_cj_10, SD);
//                 tft.print (text.substring (i, i + 3));
//                 tft.unloadFont ();
//                 i += 3;
//             } break;

//             case cj_11: {
//                 tft.loadFont (maintext ? file_main_cj_11 : file_sub_cj_11, SD);
//                 tft.print (text.substring (i, i + 4));
//                 tft.unloadFont ();
//                 i += 4;
//             } break;

//             case cj_12: {
//                 tft.loadFont (maintext ? file_main_cj_12 : file_sub_cj_12, SD);
//                 tft.print (text.substring (i, i + 4));
//                 tft.unloadFont ();
//                 i += 4;
//             } break;
//         }
//     }

//     // uint32_t utf8_val [32] = { 0 };
//     // uint8_t byte_counter = 0;
//     // uint8_t curr_byte = 0;
//     // uint8_t char_count = 0;
//     // bool in_encoding = false;

//     // for (unsigned char c : text) {
//     //     curr_byte = static_cast <int> (c);
//     //     utf8_val[char_count] += curr_byte;

//     //     if (!in_encoding && curr_byte >= 0xC0) {
//     //         in_encoding = true;
//     //         byte_counter = curr_byte >= 0xF0 ? 3 : (curr_byte >= 0xE0 ? 2 : 1);
//     //     } 
//     //     else if (in_encoding)     byte_counter--;

//     //     if (byte_counter == 0) {
//     //         in_encoding = false;
//     //         char_count++;
//     //     } else utf8_val[char_count] <<= 8;
//     // }

//     // for (uint8_t i = 0, j = 0; i < text.length (); j++) {
//     //     switch (Check_Location (utf8_val[j])) {
//     //         case latin: {
//     //             tft.loadFont (maintext ? file_main_latin : file_sub_latin, SD);
//     //             tft.print (text.substring (i, i + 1));
//     //             tft.unloadFont ();
//     //             i++;
//     //         } break;

//     //         case cyrillic_greek: {
//     //             tft.loadFont (maintext ? file_main_cyrillic_greek : file_sub_cyrillic_greek, SD);
//     //             tft.print (text.substring (i, i + 2));
//     //             tft.unloadFont ();
//     //             i += 2;
//     //         } break;

//     //         case ex_latin: {
//     //             tft.loadFont (maintext ? file_main_ex_latin : file_sub_ex_latin, SD);
//     //             tft.print (text.substring (i, i + 2));
//     //             tft.unloadFont ();
//     //             i += 2;
//     //         } break;

//     //         case japanese: {
//     //             tft.loadFont (maintext ? file_main_japanese : file_sub_japanese, SD);
//     //             tft.print (text.substring (i, i + 3));
//     //             tft.unloadFont ();
//     //             i += 3;
//     //         } break;

//     //         case hangul_1: {
//     //             tft.loadFont (maintext ? file_main_hangul_1 : file_sub_hangul_1, SD);
//     //             tft.print (text.substring (i, i + 3));
//     //             tft.unloadFont ();
//     //             i += 3;
//     //         } break;

//     //         case hangul_2: {
//     //             tft.loadFont (maintext ? file_main_hangul_2 : file_sub_hangul_2, SD);
//     //             tft.print (text.substring (i, i + 3));
//     //             tft.unloadFont ();
//     //             i += 3;
//     //         } break;

//     //         case hangul_3_1: {
//     //             tft.loadFont (maintext ? file_main_hangul_3_1 : file_sub_hangul_3, SD);
//     //             tft.print (text.substring (i, i + 3));
//     //             tft.unloadFont ();
//     //             i += 3;
//     //         } break;

//     //         case hangul_3_2: {
//     //             tft.loadFont (maintext ? file_main_hangul_3_2 : file_sub_hangul_3, SD);
//     //             tft.print (text.substring (i, i + 3));
//     //             tft.unloadFont ();
//     //             i += 3;
//     //         } break;

//     //         case hangul_4_1: {
//     //             tft.loadFont (maintext ? file_main_hangul_4_1 : file_sub_hangul_4, SD);
//     //             tft.print (text.substring (i, i + 3));
//     //             tft.unloadFont ();
//     //             i += 3;
//     //         } break;
            
//     //         case hangul_4_2: {
//     //             tft.loadFont (maintext ? file_main_hangul_4_2 : file_sub_hangul_4, SD);
//     //             tft.print (text.substring (i, i + 3));
//     //             tft.unloadFont ();
//     //             i += 3;
//     //         } break;

//     //         case hangul_5_1: {
//     //             tft.loadFont (maintext ? file_main_hangul_5_1 : file_sub_hangul_5, SD);
//     //             tft.print (text.substring (i, i + 3));
//     //             tft.unloadFont ();
//     //             i += 3;
//     //         } break;

//     //         case hangul_5_2: {
//     //             tft.loadFont (maintext ? file_main_hangul_5_2 : file_sub_hangul_5, SD);
//     //             tft.print (text.substring (i, i + 3));
//     //             tft.unloadFont ();
//     //             i += 3;
//     //         } break;

//     //         case cj_1: {
//     //             tft.loadFont (maintext ? file_main_cj_1 : file_sub_cj_1, SD);
//     //             tft.print (text.substring (i, i + 3));
//     //             tft.unloadFont ();
//     //             i += 3;
//     //         } break;

//     //         case cj_2_1: {
//     //             tft.loadFont (maintext ? file_main_cj_2_1 : file_sub_cj_2, SD);
//     //             tft.print (text.substring (i, i + 3));
//     //             tft.unloadFont ();
//     //             i += 3;
//     //         } break;

//     //         case cj_2_2: {
//     //             tft.loadFont (maintext ? file_main_cj_2_2 : file_sub_cj_2, SD);
//     //             tft.print (text.substring (i, i + 3));
//     //             tft.unloadFont ();
//     //             i += 3;
//     //         } break;

//     //         case cj_3: {
//     //             tft.loadFont (maintext ? file_main_cj_3 : file_sub_cj_3, SD);
//     //             tft.print (text.substring (i, i + 3));
//     //             tft.unloadFont ();
//     //             i += 3;
//     //         } break;

//     //         case cj_4_1: {
//     //             tft.loadFont (maintext ? file_main_cj_4_1 : file_sub_cj_4, SD);
//     //             tft.print (text.substring (i, i + 3));
//     //             tft.unloadFont ();
//     //             i += 3;
//     //         } break;

//     //         case cj_4_2: {
//     //             tft.loadFont (maintext ? file_main_cj_4_2 : file_sub_cj_4, SD);
//     //             tft.print (text.substring (i, i + 3));
//     //             tft.unloadFont ();
//     //             i += 3;
//     //         } break;

//     //         case cj_5_1: {
//     //             tft.loadFont (maintext ? file_main_cj_5_1 : file_sub_cj_5, SD);
//     //             tft.print (text.substring (i, i + 3));
//     //             tft.unloadFont ();
//     //             i += 3;
//     //         } break;

//     //         case cj_5_2: {
//     //             tft.loadFont (maintext ? file_main_cj_5_2 : file_sub_cj_5, SD);
//     //             tft.print (text.substring (i, i + 3));
//     //             tft.unloadFont ();
//     //             i += 3;
//     //         } break;

//     //         case cj_6_1: {
//     //             tft.loadFont (maintext ? file_main_cj_6_1 : file_sub_cj_6, SD);
//     //             tft.print (text.substring (i, i + 3));
//     //             tft.unloadFont ();
//     //             i += 3;
//     //         } break;

//     //         case cj_6_2: {
//     //             tft.loadFont (maintext ? file_main_cj_6_2 : file_sub_cj_6, SD);
//     //             tft.print (text.substring (i, i + 3));
//     //             tft.unloadFont ();
//     //             i += 3;
//     //         } break;

//     //         case cj_7_1: {
//     //             tft.loadFont (maintext ? file_main_cj_7_1 : file_sub_cj_7, SD);
//     //             tft.print (text.substring (i, i + 3));
//     //             tft.unloadFont ();
//     //             i += 3;
//     //         } break;

//     //         case cj_7_2: {
//     //             tft.loadFont (maintext ? file_main_cj_7_2 : file_sub_cj_7, SD);
//     //             tft.print (text.substring (i, i + 3));
//     //             tft.unloadFont ();
//     //             i += 3;
//     //         } break;

//     //         case cj_8_1: {
//     //             tft.loadFont (maintext ? file_main_cj_8_1 : file_sub_cj_8, SD);
//     //             tft.print (text.substring (i, i + 3));
//     //             tft.unloadFont ();
//     //             i += 3;
//     //         } break;

//     //         case cj_8_2: {
//     //             tft.loadFont (maintext ? file_main_cj_8_2 : file_sub_cj_8, SD);
//     //             tft.print (text.substring (i, i + 3));
//     //             tft.unloadFont ();
//     //             i += 3;
//     //         } break;

//     //         case cj_9: {
//     //             tft.loadFont (maintext ? file_main_cj_9 : file_sub_cj_9, SD);
//     //             tft.print (text.substring (i, i + 3));
//     //             tft.unloadFont ();
//     //             i += 3;
//     //         } break;

//     //         case cj_10: {
//     //             tft.loadFont (maintext ? file_main_cj_10 : file_sub_cj_10, SD);
//     //             tft.print (text.substring (i, i + 3));
//     //             tft.unloadFont ();
//     //             i += 3;
//     //         } break;

//     //         case cj_11: {
//     //             tft.loadFont (maintext ? file_main_cj_11 : file_sub_cj_11, SD);
//     //             tft.print (text.substring (i, i + 4));
//     //             tft.unloadFont ();
//     //             i += 4;
//     //         } break;

//     //         case cj_12: {
//     //             tft.loadFont (maintext ? file_main_cj_12 : file_sub_cj_12, SD);
//     //             tft.print (text.substring (i, i + 4));
//     //             tft.unloadFont ();
//     //             i += 4;
//     //         } break;
//     //     }  
//     // }
// }

// void Change_Function () {
//     if (function == PLAYBACK_FUNCTION && previous_function != function) {
//         previous_function = function;
//         tft.loadFont (file_functions_symbols, SD);
//         tft.fillRect (10, 215, 30, 30, TFT_BLACK);
//         tft.setCursor (10, 215);
//         tft.print (PREVIOUS_ICON);
//         tft.fillRect (160, 215, 30, 30, TFT_BLACK);
//         tft.setCursor (160, 215);
//         tft.print (NEXT_ICON);
//         tft.unloadFont ();
//         Serial.println ("APPLE ");
//     }
//     else if (function == STATION_FUNCTION && previous_function != function) {
//         previous_function = function;
//         tft.loadFont (file_functions_symbols, SD);
//         tft.fillRect (10, 215, 30, 30, TFT_BLACK);
//         tft.setCursor (10, 215);
//         tft.print (LEFT_ICON);
//         tft.fillRect (160, 215, 30, 30, TFT_BLACK);
//         tft.setCursor (160, 215);
//         tft.print (RIGHT_ICON);
//         tft.unloadFont ();
//         Serial.println ("PIZZA");
//     }
//     else if (function == BRIGHTNESS_FUNCTION && previous_function != function) {
//         previous_function = function;
//         tft.loadFont (file_functions_symbols, SD);
//         tft.fillRect (10, 215, 30, 30, TFT_BLACK);
//         tft.setCursor (10, 215);
//         tft.print (DIMMER_SCREEN_ICON);
//         tft.fillRect (160, 215, 30, 30, TFT_BLACK);
//         tft.setCursor (160, 215);
//         tft.print (BRIGHTER_SCREEN_ICON);
//         tft.unloadFont ();
//         Serial.println ("LITERAL");
//     }
//     else if (function == VOLUME_FUNCTION && previous_function != function) {
//         previous_function = function;
//         tft.loadFont (file_functions_symbols, SD);
//         tft.fillRect (10, 215, 30, 30, TFT_BLACK);
//         tft.setCursor (10, 215);
//         tft.print (LOWER_VOL_ICON);
//         tft.fillRect (160, 215, 30, 30, TFT_BLACK);
//         tft.setCursor (160, 215);
//         tft.print (HIGHER_VOL_ICON);
//         tft.unloadFont ();
//         Serial.println ("WHYYYYY");
//     }
// }
