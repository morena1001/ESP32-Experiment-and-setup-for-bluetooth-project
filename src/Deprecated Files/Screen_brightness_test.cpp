// #include <main.h>

// data_t data;

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
//     // Load_Intro_Screen (data);

//     data.tft.setCursor (10, 70);
//     data.tft.loadFont (file_main_cj_8_2, SD);
//     data.tft.print ("雨");
//     data.tft.unloadFont ();

//     // data.tft.fillScreen (TFT_BLACK);
//     // data.tft.setCursor (85, 215);
//     // data.tft.loadFont (file_functions_symbols, SD);
//     // data.tft.print (PLAY_PAUSE_ICON);
//     // data.function = BRIGHTNESS_FUNCTION;
//     // Serial.println ("Function 3");
//     // data.tft.setCursor (10, 215);
//     // data.tft.print (DIMMER_SCREEN_ICON);
//     // data.tft.setCursor (160, 215);
//     // data.tft.print (BRIGHTER_SCREEN_ICON);
//     // data.tft.unloadFont ();
// }

// void loop () {
// }
