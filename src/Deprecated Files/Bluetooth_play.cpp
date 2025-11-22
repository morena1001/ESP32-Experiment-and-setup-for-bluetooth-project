// #include <Arduino.h>
// #include "Preferences.h"

// #if __has_include ("BluetoothA2DP.h")

// #include "BluetoothA2DPSink.h"

// BluetoothA2DPSink a2dp_sink;
// Preferences preferences;

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

// void avrc_metadata_callback (uint8_t id, const uint8_t *text) {
//   Serial.printf ("==> AVRC metadata rsp: attribute id 0x%x, %s\n", id, text);
//   // if (id == ESP_AVRC_MD_ATTR_PLAYING_TIME) {
//   //   uint32_t playtime = String ((char*) text).toInt ();
//   //   Serial.printf ("==> Playing time is %d ms (%d seconds)\n", playtime, (int) round (playtime/1000.0));
//   // }
// }

// void avrc_rn_playstatus_callback (esp_avrc_playback_stat_t playback) {
//   switch (playback) {
//     case esp_avrc_playback_stat_t::ESP_AVRC_PLAYBACK_PAUSED:
//       Serial.println ("Stopped.");
//       is_active = false;
//       break;
//     case esp_avrc_playback_stat_t::ESP_AVRC_PLAYBACK_PLAYING:
//       Serial.println ("Playing.");
//       is_active = true;
//       break;
//     default:
//       Serial.printf ("Got unknown playback status %d\n", playback);
//   }
// }

// void setup () {
//   Serial.begin (115200);

//   pinMode (pp_pin, INPUT);
//   pinMode (next_pin, INPUT);
//   pinMode (prev_pin, INPUT);

//   a2dp_sink.set_avrc_metadata_attribute_mask (ESP_AVRC_MD_ATTR_TITLE | ESP_AVRC_MD_ATTR_ARTIST | ESP_AVRC_MD_ATTR_ALBUM );
//   a2dp_sink.set_avrc_metadata_callback (avrc_metadata_callback);
//   a2dp_sink.set_avrc_rn_playstatus_callback (avrc_rn_playstatus_callback);

//   a2dp_sink.start ("MyMusic", true);
//   Serial.println ("Started bluetooth");

//   preferences.begin ("connections", true);
//   bool prev_init = preferences.isKey ("prev");
//   if (prev_init) {
//     esp_bd_addr_t prev_address;
//     preferences.getBytes ("prev", prev_address, ESP_BD_ADDR_LEN);
//     Serial.print ("Checking to see if ");
//     Serial.print (a2dp_sink.to_str (prev_address));
//     Serial.println (" Connects");
//     if (a2dp_sink.connect_to (prev_address)) {
//       Serial.print ("Reconnected to previously connected device: ");
//       Serial.println (a2dp_sink.to_str (prev_address));
//     }
//   }
//   preferences.end ();
// }

// void loop () {
//   // Read from the input button
//   int pp_reading = digitalRead (pp_pin);
//   int next_reading = digitalRead (next_pin);
//   int prev_reading = digitalRead (prev_pin);

//   // If switch is changed due to noise or pressing, reset debounce timer
//   if (pp_reading != pp_last_state)       pp_last_db_time = millis();
//   if (next_reading != next_last_state)   next_last_db_time = millis();
//   if (prev_reading != prev_last_state)   prev_last_db_time = millis();

//   // If debounce delay is reached, update the state of the button, and if high, send the message
//   if ((millis() - pp_last_db_time) > pp_db_delay) {
//     if (pp_reading != pp_state) {
//       pp_state = pp_reading;
      
//       if (pp_state == HIGH) {
//         is_active = !is_active;
//         if (is_active) {
//           Serial.println ("play");
//           a2dp_sink.play ();
//         } else {
//           Serial.println ("pause");
//           a2dp_sink.pause ();
//         }
//         // bleKeyboard.write (KEY_MEDIA_PLAY_PAUSE);
//       }
//     }
//   }

//   if ((millis() - next_last_db_time) > next_db_delay) {
//     if (next_reading != next_state) {
//       next_state = next_reading;
      
//       if (next_state == HIGH) {
//         Serial.println ("Next");
//         a2dp_sink.next ();
//         // bleKeyboard.write (KEY_MEDIA_NEXT_TRACK);
//       }
//     }
//   }

//   if ((millis() - prev_last_db_time) > prev_db_delay) {
//     if (prev_reading != prev_state) {
//       prev_state = prev_reading;
      
//       if (prev_state == HIGH) {
//         Serial.println ("Previous");
//         a2dp_sink.previous ();
//         // bleKeyboard.write (KEY_MEDIA_PREVIOUS_TRACK);
//       }
//     }
//   }
  
//   // Update previous button state
//   pp_last_state = pp_reading;
//   next_last_state = next_reading;
//   prev_last_state = prev_reading;

//   // Send a device conntected message once if a device is connected
//   if (a2dp_sink.is_connected () && !sent_connected_message) {
//     sent_connected_message = true;
//     sent_disconnected_message = false;
//     Serial.println ("Device connected");

//     preferences.begin ("connections", false);
//     preferences.putBytes ("prev", a2dp_sink.get_current_peer_address (), ESP_BD_ADDR_LEN);
//     Serial.print ("Put address to memory: ");
//     Serial.println (a2dp_sink.to_str (*(a2dp_sink.get_current_peer_address ())));
//     preferences.end ();
//   } else if (!a2dp_sink.is_connected () && !sent_disconnected_message) {
//     sent_disconnected_message = true;
//     sent_connected_message = false;
//     Serial.println ("Device disconnected");
//   }
// }



// #else

// #include <BleKeyboard.h>

// BleKeyboard bleKeyboard ("Media Control");

// bool sent_connected_message = false;

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

// void setup () {
//   Serial.begin (115200);

//   pinMode (pp_pin, INPUT);
//   pinMode (next_pin, INPUT);
//   pinMode (prev_pin, INPUT);

//   Serial.println ("Starting BLE work!");
//   bleKeyboard.begin ();
// }

// void loop () {
//   // Read from the input button
//   int pp_reading = digitalRead (pp_pin);
//   int next_reading = digitalRead (next_pin);
//   int prev_reading = digitalRead (prev_pin);

//   // If switch is changed due to noise or pressing, reset debounce timer
//   if (pp_reading != pp_last_state)       pp_last_db_time = millis();
//   if (next_reading != next_last_state)   next_last_db_time = millis();
//   if (prev_reading != prev_last_state)   prev_last_db_time = millis();

//   // If debounce delay is reached, update the state of the button, and if high, send the message
//   if ((millis() - pp_last_db_time) > pp_db_delay) {
//     if (pp_reading != pp_state) {
//       pp_state = pp_reading;
      
//       if (pp_state == HIGH) {
//         Serial.println ("Sending Play/Pause media key...");
//         bleKeyboard.write (KEY_MEDIA_PLAY_PAUSE);
//       }
//     }
//   }

//   if ((millis() - next_last_db_time) > next_db_delay) {
//     if (next_reading != next_state) {
//       next_state = next_reading;
      
//       if (next_state == HIGH) {
//         Serial.println ("Sending Next media key");
//         bleKeyboard.write (KEY_MEDIA_NEXT_TRACK);
//       }
//     }
//   }

//   if ((millis() - prev_last_db_time) > prev_db_delay) {
//     if (prev_reading != prev_state) {
//       prev_state = prev_reading;
      
//       if (prev_state == HIGH) {
//         Serial.println ("Sending previous media key...");
//         // bleKeyboard.write ((uint8_t ) 'a'); for Onsong 
//         bleKeyboard.write (KEY_MEDIA_PREVIOUS_TRACK);
//       }
//     }
//   }
  
//   // Update previous button state
//   pp_last_state = pp_reading;
//   next_last_state = next_reading;
//   prev_last_state = prev_reading;

//   // Send a device conntected message once if a device is connected
//   if (bleKeyboard.isConnected () && !sent_connected_message) {
//     sent_connected_message = true;

//     Serial.println ("Device connected");
//     bleKeyboard.print ("Device connected");

//   } else if (!bleKeyboard.isConnected ()) {
//     if (sent_connected_message)   Serial.println ("Device disconnected");
//     sent_connected_message = false;
//   }
// }

// #endif
