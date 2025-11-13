//  Created by Bodmer 24th Jan 2017

// The latest Arduino IDE versions support UTF-8 encoding of Unicode characters
// within sketches:
// https://playground.arduino.cc/Code/UTF-8

/*
  The library expects strings to be in UTF-8 encoded format:
  https://www.fileformat.info/info/unicode/utf8.htm

  Creating variables needs to be done with care when using character arrays:
  char c = 'µ';          // Wrong
  char bad[4] = "5µA";   // Wrong
  char good[] = "5µA";   // Good
  String okay = "5µA";   // Good

  This is because UTF-8 characters outside the basic Latin set occupy more than
  1 byte per character! A 16-bit Unicode character occupies 3 bytes!

*/

//  The fonts are stored in arrays within the sketch tabs.

//  A processing sketch to create new fonts can be found in the Tools folder of TFT_eSPI
//  https://github.com/Bodmer/TFT_eSPI/tree/master/Tools/Create_Smooth_Font/Create_font

#include "Latin_Hiragana_24.h"
#include "Unicode_Test_72.h"

// #include "Unifont_Latin.h"
// #include "Unifont_CJK_1.h"
// #include "Unifont_CJK_2.h"
// #include "Unifont_CJK_3.h"
// #include "Unifont_CJK_4.h"
// #include "Unifont_CJK_5.h"
// #include "Unifont_CJK_6.h"

//====================================================================================
//                                  Libraries
//====================================================================================

// #include "Arduino.h"
#include <TFT_eSPI.h>      // Hardware-specific library

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

uint16_t bg = TFT_BLACK;
uint16_t fg = TFT_WHITE;

void Check_Location (uint32_t enc_byte) {
    if (enc_byte >= 0xECB080 && enc_byte <= 0xED9FBF)       Serial.printf ("CJK 11\r\n");
    else if (enc_byte >= 0xEBB080 && enc_byte <= 0xECB080)  Serial.printf ("CJK 10\r\n");
    else if (enc_byte >= 0xEAB080 && enc_byte <= 0xEBB080)  Serial.printf ("CJK 9\r\n");
    else if (enc_byte >= 0xE99080 && enc_byte <= 0xE9BFBF)  Serial.printf ("CJK 8\r\n");
    else if (enc_byte >= 0xE89080 && enc_byte <= 0xE99080)  Serial.printf ("CJK 7\r\n");
    else if (enc_byte >= 0xE79080 && enc_byte <= 0xE89080)  Serial.printf ("CJK 6\r\n");
    else if (enc_byte >= 0xE69080 && enc_byte <= 0xE79080)  Serial.printf ("CJK 5\r\n");
    else if (enc_byte >= 0xE59080 && enc_byte <= 0xE69080)  Serial.printf ("CJK 4\r\n");
    else if (enc_byte >= 0xE49080 && enc_byte <= 0xE59080)  Serial.printf ("CJK 3\r\n");
    else if (enc_byte >= 0xE39080 && enc_byte <= 0xE49080)  Serial.printf ("CJK 2\r\n");
    else if (enc_byte >= 0xE2BA80 && enc_byte <= 0xE387BF)  Serial.printf ("CJK 1\r\n");

    else if (enc_byte >= 0xE1B880 && enc_byte <= 0xE1BBB9)  Serial.printf ("Latin 2\r\n");
    else if (enc_byte <= 0x00D4AF)                          Serial.printf ("Latin 1\r\n");
    else                                                    Serial.printf ("COULD NOT FIND VALID LOCATION\r\n");
}


//====================================================================================
//                                    Setup
//====================================================================================
void setup()
{
  Serial.begin(115200); // Used for messages and the C array generator

  Serial.println("Font test!");

  tft.begin();
  tft.setRotation(0);  // portrait

  fg = TFT_WHITE;
  bg = TFT_BLACK;

  String rain = "p雨Àのr";
  uint32_t utf8_val[10];
  uint8_t byte_counter = 0;
  bool in_encoding = false;
  uint8_t curr_byte = 0;
  uint8_t char_count = 0;

  for (unsigned char c : rain) {
    curr_byte = static_cast<int> (c);
    Serial.printf ("%X ", curr_byte);
    utf8_val[char_count] += curr_byte;

    if (!in_encoding) {
      if (curr_byte >= 0xC0) {
        in_encoding = true;
        byte_counter = curr_byte >= 0xE0 ? 2 : 1;
      }
    } else    byte_counter--;

    if (byte_counter == 0) {
      in_encoding = false;
      Serial.printf ("| ");
      char_count++;
    } else    utf8_val[char_count] <<= 8;
  }
  Serial.println ();

  for (uint32_t val : utf8_val) {
    Serial.printf ("%X | ", val);
    Check_Location (val);
  }

  Serial.println ();

  
//   tft.setTextColor(fg, bg);
//   String test2 = "雨"; // Unicode 0x4EDD, 0x5000
//   tft.loadFont(Unicode_Test_72);
//   tft.setRotation(1);
//   // Show all characters on screen with 2 second (2000ms) delay between screens
// //   tft.showFont(2000); // Note: This function moves the cursor position!
//   tft.fillScreen(bg);
//   tft.setCursor(0,0);
//   tft.setTextColor(TFT_CYAN, bg);
//   tft.println(test2);
// //   tft.println ("\nRYUSENKEI");
//   tft.unloadFont();
}

//====================================================================================
//                                    Loop
//====================================================================================
void loop()
{
  tft.setTextColor(fg, bg);

  //----------------------------------------------------------------------------
  // We can have any random mix of characters in the font

  String test2 = "仝倀";//"雨のシンデレラ"; // Unicode 0x4EDD, 0x5000

//   tft.loadFont(small_sample);
  tft.loadFont(Unicode_Test_72);

  tft.setRotation(1);

// //   // Show all characters on screen with 2 second (2000ms) delay between screens
  tft.showFont(2000); // Note: This function moves the cursor position!

  tft.fillScreen(bg);
  tft.setCursor(0,0);

  tft.setTextColor(TFT_CYAN, bg);
  tft.println(test2);
  tft.unloadFont();
//   tft.loadFont (Latin_Hiragana_24);
//   tft.println ("\nRYUSENKEI");

// //   tft.setTextColor(TFT_YELLOW, bg);
// //   tft.println("12:00pm");

// //   tft.setTextColor(TFT_MAGENTA, bg);
// //   tft.println("1000Ω");

// //   Remove font parameters from memory to recover RAM
//   tft.unloadFont();

  delay(2000);

  //----------------------------------------------------------------------------
  // Latin and Hiragana font mix

//   String test3 = "こんにちは";
    
//   tft.loadFont(Latin_Hiragana_24);

//   tft.setRotation(0);
  
//   // Show all characters on screen with 2 second (2000ms) delay between screens
//   tft.showFont(2000); // Note: This function moves the cursor position!

//   tft.fillScreen(bg);
//   tft.setTextColor(TFT_GREEN, bg);
//   tft.setCursor(0,0);

//   tft.println("Konnichiwa");
//   tft.println(test3);
//   tft.println();
//   tft.println("Sayonara");
//   tft.println("さようなら"); // Sayonara

//   // Remove font parameters from memory to recover RAM
//   tft.unloadFont();

//   delay(2000);
  //
  //----------------------------------------------------------------------------
}
//====================================================================================