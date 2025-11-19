// #include <SD.h>
// #include <TFT_eSPI.h>

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

// uint8_t Check_Location (uint32_t enc_byte);
// void listDir(fs::FS &fs, const char * dirname, uint8_t levels);

// TFT_eSPI tft = TFT_eSPI ();

// void setup () { 
//     Serial.begin (115200);

//     while (!SD.begin (2)) {
//         Serial.println ("Card Mount Failed");
//     }
//     uint8_t card_type = SD.cardType ();

//     if (card_type == CARD_NONE) {
//         Serial.println ("No SD card attached");
//         return;
//     }

//     Serial.print ("SD Card Type: ");
//     if (card_type == CARD_MMC) {
//         Serial.println ("MMC");
//     } else if (card_type == CARD_SD) {
//         Serial.println ("SDSC");
//     } else if (card_type == CARD_SDHC) {
//         Serial.println ("SDHC");
//     } else {
//         Serial.println ("UNKOWN");
//     }

//     uint64_t card_size = SD.cardSize () / (1024 * 1024);
//     Serial.printf ("SD Card Size: %llumMB\n", card_size);

//     tft.init ();
//     tft.setRotation (1);
//     tft.fillScreen (TFT_BLACK);

//     listDir (SD, "/", 0);

//     Serial.println ("SD and TFT initialization done.");


//     //   // String rain = "あp雨Ḅのr";
//     String rain = "あp";
//     uint32_t utf8_val[10];
//     uint8_t byte_counter = 0;
//     bool in_encoding = false;
//     uint8_t curr_byte = 0;
//     uint8_t char_count = 0;

//     for (unsigned char c : rain) {
//         curr_byte = static_cast<int> (c);
//         Serial.printf ("%X ", curr_byte);
//         utf8_val[char_count] += curr_byte;

//         if (!in_encoding) {
//             if (curr_byte >= 0xC0) {
//             in_encoding = true;
//             byte_counter = curr_byte >= 0xE0 ? 2 : 1;
//             }
//         } else    byte_counter--;

//         if (byte_counter == 0) {
//             in_encoding = false;
//             Serial.printf ("| ");
//             char_count++;
//         } else    utf8_val[char_count] <<= 8;
//         }
//         Serial.println ();

//         for (uint32_t val : utf8_val) {
//         Serial.printf ("%X | %d", val, Check_Location (val));
//     }



//     tft.setTextWrap (true, true);
//     String file_latin_1 = "Unifont_Latin_1";
//     String file_cjk_1 = "Unifont_CJK_1";
//     tft.setTextColor (TFT_WHITE, TFT_BLACK);
//     tft.setCursor (0, 0);
//     // tft.showFont (1000);

//     for (uint8_t i = 0, j = 0; i <= rain.length (); j++) {
//         uint32_t val = utf8_val[j];
//         if (Check_Location (val) == LAT1) {
//             tft.loadFont (file_latin_1, SD);
//             tft.println (rain.substring (i, i + (val > 256 ? 2 : 1)));
//             tft.unloadFont ();
//             i += (val > 256 ? 2 : 1);
//         } else if (Check_Location (val) == CJK1) {
//             tft.loadFont (file_cjk_1, SD);
//             tft.println (rain.substring (i, i + 3));
//             tft.unloadFont ();
//             i += 3;
//         }
//     }
    
//     // tft.loadFont (file_latin_1, SD);
//     // tft.setCursor (0, 0);
//     // tft.println ("Hello World!");
//     // tft.unloadFont ();

//     delay (2000);
// }

// void loop () {
//     // tft.setTextWrap (true, true);
//     // String file_name = "Unifont_Latin_1";
//     // tft.setTextColor (TFT_WHITE, TFT_BLACK);
//     // tft.loadFont (file_name, SD);
//     // tft.showFont (1000);

//     // tft.setCursor (0, 0);
//     // tft.println ("Hello World!");

//     // tft.unloadFont ();

//     // delay (2000);
// }

// uint8_t Check_Location (uint32_t enc_byte) {
//   if (enc_byte == 0)  return FONT_ERROR;
//   if (enc_byte >= 0xECB080 && enc_byte <= 0xED9FBF)       return CJK11;
//   else if (enc_byte >= 0xEBB080 && enc_byte <= 0xECB080)  return CJK10;
//   else if (enc_byte >= 0xEAB080 && enc_byte <= 0xEBB080)  return CJK9;
//   else if (enc_byte >= 0xE99080 && enc_byte <= 0xE9BFBF)  return CJK8;
//   else if (enc_byte >= 0xE89080 && enc_byte <= 0xE99080)  return CJK7;
//   else if (enc_byte >= 0xE79080 && enc_byte <= 0xE89080)  return CJK6;
//   else if (enc_byte >= 0xE69080 && enc_byte <= 0xE79080)  return CJK5;
//   else if (enc_byte >= 0xE59080 && enc_byte <= 0xE69080)  return CJK4;
//   else if (enc_byte >= 0xE49080 && enc_byte <= 0xE59080)  return CJK3;
//   else if (enc_byte >= 0xE39080 && enc_byte <= 0xE49080)  return CJK2;
//   else if (enc_byte >= 0xE2BA80 && enc_byte <= 0xE387BF)  return CJK1;

//   else if (enc_byte >= 0xE1B880 && enc_byte <= 0xE1BBB9)  return LAT2;
//   else if (enc_byte <= 0x00D4AF)                          return LAT1;
  
//   return FONT_ERROR;
// }

// void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
//     Serial.printf ("Listing directory: %S\n", dirname);

//     File root = fs.open (dirname);
//     if (!root) {
//         Serial.println ("Failed to open directory");
//         return;
//     } 
//     if (!root.isDirectory ()) {
//         Serial.println ("Not a directory");
//         return;
//     }

//     File file = root.openNextFile ();
//     while (file) {
//         if (file.isDirectory ()) {
//             Serial.print ("  DIR : ");
//             Serial.println (file.name ());
//             if (levels) {
//                 listDir (fs, file.name (), levels - 1);
//             }
//         } else {
//             Serial.print ("  FILE : ");
//             Serial.print (file.name ());
//             Serial.print ("  SIZE : ");
//             Serial.println (file.size ());
//         }
//         file = root.openNextFile ();
//     }
// }