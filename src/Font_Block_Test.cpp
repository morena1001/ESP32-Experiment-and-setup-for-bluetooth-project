// #include <SD.h>
// #include <TFT_eSPI.h>

// #define Block_1_Main    "Block 1 - main"
// #define Block_1_Sub     "Block 1 - sub"
// #define Block_2_Main    "Block 2 - main"
// #define Block_2_Sub     "Block 2 - sub"
// #define Block_3_Main    "Block 3 - main"
// #define Block_3_Sub     "Block 3 - sub"
// #define Block_4_Main    "Block 4 - main"
// #define Block_4_Sub     "Block 4 - sub"
// #define Block_5_Main    "Block 5 - main"
// #define Block_5_Sub     "Block 5 - sub"
// #define Block_6_Main    "Block 6 - main"
// #define Block_6_Sub     "Block 6 - sub"
// #define Block_7_Main    "Block 7 - main"
// #define Block_7_Sub     "Block 7 - sub"
// #define Block_8_Main    "Block 8 - main"
// #define Block_8_Sub     "Block 8 - sub"
// #define Block_9_Main    "Block 9 - main"
// #define Block_9_Sub     "Block 9 - sub"
// #define Block_10_Main   "Block 10 - main"
// #define Block_10_Sub    "Block 10 - sub"
// #define Block_11_Main   "Block 11 - main"
// #define Block_11_Sub    "Block 11 - sub"
// #define Block_12_Main   "Block 12 - main"
// #define Block_12_Sub    "Block 12 - sub"
// #define Block_13_Main   "Block 13 - main"
// #define Block_13_Sub    "Block 13 - sub"
// #define Block_14_Main   "Block 14 - main"
// #define Block_14_Sub    "Block 14 - sub"
// #define Block_15_Main   "Block 15 - main"
// #define Block_15_Sub    "Block 15 - sub"

// void listDir (FS &fs, const char* dirname, uint8_t levels);

// TFT_eSPI tft = TFT_eSPI ();

// void setup () {
//     Serial.begin (115200);

//     while (!SD.begin (2))       Serial.println ("Card mount failed");

//     tft.init ();
//     tft.setRotation (1);
//     tft.fillScreen (TFT_BLACK);
//     tft.setTextColor (TFT_WHITE);
//     Serial.println ("SD and TFT inialization done");

//     // listDir (SD, "/", 0);
//     // Serial.println ();

//     tft.setCursor (10, 70);
//     tft.loadFont (Block_1_Main, SD);
//     tft.print ("HELLO WORLD");
//     delay (2000);
//     tft.fillRect (0, 0, 320, 240, TFT_BLACK);
//     tft.setCursor (10, 70);

//     String title = "雨のシンデレラ - RYUSENKEI";
//     String artist = "RYUSENEKEI";
//     String album = "TOKYO SNIPER";

//     String text = title;

//     uint8_t val = 0;
//     for (int i = 0; i <= text.length (); ) {
//         val = static_cast <uint8_t> (text[i]);
//         // Serial.println (val);

//         if (val < 0x80) {
//             // Block 1
//             tft.loadFont (Block_1_Main, SD);
//             tft.print (text.substring (i, i + 1));
//             tft.unloadFont ();
//             i++;
//         } else if (val < 0xE1) {
//             // Block 1
//             tft.loadFont (Block_1_Main, SD);
//             tft.print (text.substring (i, i + 2));
//             tft.unloadFont ();
//             i += 2;
//         } else if (val == 0xE1) {
//             // Block 2
//             tft.loadFont (Block_2_Main, SD);
//             tft.print (text.substring (i, i + 3));
//             tft.unloadFont ();
//             i += 3;
//         } else if (val == 0xE2) {
//             // Block 3
//             tft.loadFont (Block_3_Main, SD);
//             tft.print (text.substring (i, i + 3));
//             tft.unloadFont ();
//             i += 3;
//         } else if (val == 0xE3) {
//             // Block 4
//             tft.loadFont (Block_4_Main, SD);
//             tft.print (text.substring (i, i + 3));
//             tft.unloadFont ();
//             i += 3;
//         } else if (val == 0xE4) {
//             // Block 5
//             tft.loadFont (Block_5_Main, SD);
//             tft.print (text.substring (i, i + 3));
//             tft.unloadFont ();
//             i += 3;
//         } else if (val == 0xE5) {
//             // Block 6
//             tft.loadFont (Block_6_Main, SD);
//             tft.print (text.substring (i, i + 3));
//             tft.unloadFont ();
//             i += 3;
//         } else if (val == 0xE6) {
//             // Block 7
//             tft.loadFont (Block_7_Main, SD);
//             tft.print (text.substring (i, i + 3));
//             tft.unloadFont ();
//             i += 3;
//         } else if (val == 0xE7) {
//             // Block 8
//             tft.loadFont (Block_8_Main, SD);
//             tft.print (text.substring (i, i + 3));
//             tft.unloadFont ();
//             i += 3;
//         } else if (val == 0xE8) {
//             // Block 9
//             tft.loadFont (Block_9_Main, SD);
//             tft.print (text.substring (i, i + 3));
//             Serial.println (text.substring (i, i + 3));
//             tft.unloadFont ();
//             i += 3;
//         } else if (val == 0xE9) {
//             // Block 10
//             tft.loadFont (Block_10_Main, SD);
//             tft.print (text.substring (i, i + 3));
//             tft.unloadFont ();
//             i += 3;
//         } else if (val == 0xEA) {
//             // Block 11
//             tft.loadFont (Block_11_Main, SD);
//             tft.print (text.substring (i, i + 3));
//             tft.unloadFont ();
//             i += 3;
//         } else if (val == 0xEB) {
//             // Block 12
//             tft.loadFont (Block_12_Main, SD);
//             tft.print (text.substring (i, i + 3));
//             tft.unloadFont ();
//             i += 3;
//         } else if (val == 0xEC) {
//             // Block 13
//             tft.loadFont (Block_13_Main, SD);
//             tft.print (text.substring (i, i + 3));
//             tft.unloadFont ();
//             i += 3;
//         } else if (val == 0xED) {
//             // Block 14
//             tft.loadFont (Block_14_Main, SD);
//             tft.print (text.substring (i, i + 3));
//             tft.unloadFont ();
//             i += 3;
//         } else {
//             // Block 15
//             tft.loadFont (Block_15_Main, SD);
//             tft.print (text.substring (i, i + 3));
//             tft.unloadFont ();
//             i += 3;
//         }
//     }
// }

// void loop () {}

// void listDir (FS &fs, const char* dirname, uint8_t levels) {
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
