// #include <main.h>

// data_t data;

// const int next_pin = 33; // NEXT BUTTON
// const int prev_pin = 27; // PREVIOUS BUTTON
// unsigned long next_last_db_time = 0;
// unsigned long prev_last_db_time = 0;
// unsigned long next_db_delay = 50;
// unsigned long prev_db_delay = 50;
// int next_state;            
// int prev_state;  
// int next_last_state = LOW;  
// int prev_last_state = LOW; 

// void setup () {
//     Serial.begin (115200);

//     // SET UP FONTS IN SD CARD
//     while (!SD.begin (2))   Serial.println ("Card mount failed");
//     Serial.println ("Card mount success");

//     // SET UP SCREEN;
//     data.tft.init ();
//     data.tft.setRotation (1);
//     data.tft.fillScreen (TFT_BLACK);
//     data.tft.setTextColor (TFT_WHITE);
//     Serial.println ("TFT initialization done");
//     Load_Intro_Screen (data);

//     // GET SCREEN BRIGHTNESS
//     data.screen_brightness = data.tft.readcommand8 (ILI9341_RDDISBV, 0);
//     Serial.printf ("Initial screen brightness is: %d\n", data.screen_brightness);

//     pinMode (next_pin, INPUT);
//     pinMode (prev_pin, INPUT);

//     data.tft.fillScreen (TFT_BLACK);
//     data.tft.setCursor (85, 215);
//     data.tft.loadFont (file_functions_symbols, SD);
//     data.tft.print (PLAY_PAUSE_ICON);
//     data.function = BRIGHTNESS_FUNCTION;
//     Serial.println ("Function 3");
//     data.tft.setCursor (10, 215);
//     data.tft.print (DIMMER_SCREEN_ICON);
//     data.tft.setCursor (160, 215);
//     data.tft.print (BRIGHTER_SCREEN_ICON);
//     data.tft.unloadFont ();
// }

// void loop () {
//     int next_reading = digitalRead (next_pin);
//     int prev_reading = digitalRead (prev_pin);

//     if (next_reading != next_last_state)        next_last_db_time = millis();
//     if (prev_reading != prev_last_state)        prev_last_db_time = millis();

//     if ((millis() - next_last_db_time) > next_db_delay) {
//         if (next_reading != next_state) {
//             next_state = next_reading;
            
//             if (next_state == HIGH) {
//                 if (data.screen_brightness < MAX_SCREEN_BRIGHTNESS)     data.screen_brightness += 16;
//                 Serial.printf ("Brightness up to %d\n", data.screen_brightness);
//                 data.tft.writecommand (ILI9341_WRDISBV);
//                 data.tft.writedata (data.screen_brightness);
//             }
//         }
//     }

//     if ((millis() - prev_last_db_time) > prev_db_delay) {
//         if (prev_reading != prev_state) {
//             prev_state = prev_reading;
            
//             if (prev_state == HIGH) {
//                 if (data.screen_brightness > MIN_SCREEN_BRIGHTNESS)     data.screen_brightness -= 16;
//                 Serial.printf ("Brightness down to %d\n", data.screen_brightness);
//                 data.tft.writecommand (ILI9341_WRDISBV);
//                 data.tft.writedata (data.screen_brightness);
//             }
//         }
//     }
// }
