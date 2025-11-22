// #include <Arduino.h>
// #include <BleKeyboard.h>

// BleKeyboard bleKeyboard ("Media Control");

// bool sent_connected_message = false;

// const int pp_pin = 25; // GPIO pin for the pause/play button
// const int next_pin = 26; // GPIO pin for the next button
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

//   } else if (!bleKeyboard.isConnected ())    sent_connected_message = false;
// }