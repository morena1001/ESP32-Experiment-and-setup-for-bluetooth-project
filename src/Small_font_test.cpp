// #include "small_font.h"

// #include <TFT_eSPI.h>

// TFT_eSPI tft = TFT_eSPI ();

// uint16_t bg = TFT_BLACK;
// uint16_t fg = TFT_WHITE;

// String text = "1aAあ雨한";

// void f_print (const uint8_t font[]);
// void f_println (const uint8_t font[]);

// void setup () {
//     Serial.begin (115200);
//     Serial.println ("Font size test");

//     tft.begin ();
//     tft.setRotation (1);

//     tft.fillScreen (bg);
//     tft.setTextColor (fg);

//     // SONG TITLE
//     tft.setCursor (10, 70);
//     tft.loadFont (font_high_q);
//     tft.println ("雨のシンデレラ");
//     tft.unloadFont ();
//     // f_println (font_40);

//     // SONG ARTIST
//     tft.setCursor (20, 110);
//     f_println (font_20);

//     // SONG ALBUM
//     tft.setCursor (20, 135);
//     f_println (font_20);
    
//     // FUNCTIONS
//     tft.setCursor (10, 220);
//     f_println (font_15);

//     tft.setCursor (85, 220);
//     f_println (font_15);

//     tft.setCursor (160, 220);
//     f_println (font_15);

//     tft.setCursor (235, 220);
//     f_println (font_15);
    
//     // PHONE TIME
//     tft.setCursor (235, 10);
//     tft.loadFont (font_30);
//     tft.println ("11111");
//     tft.unloadFont ();

//     // SONG TIME
//     tft.setCursor (20, 50);
//     f_println (font_15);
    
//     // tft.setCursor (0, 0);
    
//     // f_println (font_35);
//     // tft.println();
//     // f_println (font_40);
//     // tft.println();
//     // f_println (font_50);
    
//     // tft.loadFont (font_25);
//     // tft.println (text);
//     // tft.unloadFont ();
// }

// void loop () {

// }

// void f_print (const uint8_t font[]) {
//     tft.loadFont (font);
//     tft.print (text);
//     tft.unloadFont ();
// }

// void f_println (const uint8_t font[]) {
//     tft.loadFont (font);
//     tft.println (text);
//     tft.unloadFont ();
// }
