#include <SD.h>
#include <TFT_eSPI.h>

#define FONT_ERROR      0
#define latin           1
#define cyrillic_greek  2
#define ex_latin        3
#define japanese        4
#define hangul_1        5
#define hangul_2        6
#define hangul_3        7
#define hangul_3_1      8
#define hangul_3_2      9
#define hangul_4        10
#define hangul_4_1      11
#define hangul_4_2      12
#define hangul_5        13
#define hangul_5_1      14
#define hangul_5_2      15
#define cj_1            16
#define cj_2            17
#define cj_2_1          18
#define cj_2_2          19
#define cj_3            20
#define cj_4            21
#define cj_4_1          22
#define cj_4_2          23
#define cj_5            24
#define cj_5_1          25
#define cj_5_2          26
#define cj_6            27
#define cj_6_1          28
#define cj_6_2          29
#define cj_7            30
#define cj_7_1          31
#define cj_7_2          32
#define cj_8            33
#define cj_8_1          34
#define cj_8_2          35
#define cj_9            36
#define cj_10           37
#define cj_11           38
#define cj_12           39

#define file_latin           "Maintext_latin"
#define file_cyrillic_greek  "Maintext_cyrillic_greek"
#define file_ex_latin        "Maintext_ex_latin"
#define file_japanese        "Maintext_hiragana_katakana"
#define file_hangul_1        "Maintext_hangul_1"
#define file_hangul_2        "Maintext_hangul_2"
#define file_hangul_3_1      "Maintext_hangul_3_1"
#define file_hangul_3_2      "Maintext_hangul_3_2"
#define file_hangul_4_1      "Maintext_hangul_4_1"
#define file_hangul_4_2      "Maintext_hangul_4_2"
#define file_hangul_5_1      "Maintext_hangul_5_1"
#define file_hangul_5_2      "Maintext_hangul_5_2"
#define file_cj_1            "Maintext_cj_1"
#define file_cj_2_1          "Maintext_cj_2_1"
#define file_cj_2_2          "Maintext_cj_2_2"
#define file_cj_3            "Maintext_cj_3"
#define file_cj_4_1          "Maintext_cj_4_1"
#define file_cj_4_2          "Maintext_cj_4_2"
#define file_cj_5_1          "Maintext_cj_5_1"
#define file_cj_5_2          "Maintext_cj_5_2"
#define file_cj_6_1          "Maintext_cj_6_1"
#define file_cj_6_2          "Maintext_cj_6_2"
#define file_cj_7_1          "Maintext_cj_7_1"
#define file_cj_7_2          "Maintext_cj_7_2"
#define file_cj_8_1          "Maintext_cj_8_1"
#define file_cj_8_2          "Maintext_cj_8_2"
#define file_cj_9            "Maintext_cj_9"
#define file_cj_10           "Maintext_cj_10"
#define file_cj_11           "Maintext_cj_11"
#define file_cj_12           "Maintext_cj_12"

uint8_t Check_Location (uint32_t enc_byte);
void listDir (FS &fs, const char* dirname, uint8_t levels);

TFT_eSPI tft = TFT_eSPI ();

void setup () {
    Serial.begin (115200);

    while (!SD.begin (2)) {
        Serial.println ("Card mount failed");
    }
    uint8_t card_type = SD.cardType ();

    if (card_type == CARD_NONE) {
        Serial.println ("No SD card attached");
        return;
    }

    Serial.print ("SD Card Type: ");
    if (card_type == CARD_MMC) {
        Serial.println ("MMC");
    } else if (card_type == CARD_SD) {
        Serial.println ("SDSC");
    } else if (card_type == CARD_SDHC) {
        Serial.println ("SDHC");
    } else {
        Serial.println ("UNKOWN");
    }

    uint64_t card_size = SD.cardSize () / (1024 * 1024);
    Serial.printf ("SD Card Size: %llumMB\n", card_size);

    tft.init ();
    tft.setRotation (1);
    tft.fillScreen (TFT_BLACK);

    listDir (SD, "/", 0);

    Serial.println ("SD and TFT initialization done.");

    String title = "雨のシンデレラ";
    String artist = "RYUSENEKEI";
    String album = "TOKYO SNIPER";

    uint32_t utf8_val [32];
    uint8_t byte_counter = 0;
    bool in_encoding = false;
    uint8_t curr_byte = 0;
    uint8_t char_count = 0;

    for (unsigned char c : title) {
        curr_byte = static_cast <int> (c);
        // Serial.printf ("%X", curr_byte);
        utf8_val[char_count] += curr_byte;

        if (!in_encoding) {
            if (curr_byte >= 0xC0) {
                in_encoding = true;
                byte_counter = curr_byte >= 0xF0 ? 3 : (curr_byte >= 0xE0 ? 2 : 1);
            } 
        } else  byte_counter--;

        if (byte_counter == 0) {
            in_encoding = false;
            // Serial.printf ("| ");
            char_count++;
        } else  utf8_val[char_count] <<= 8;

        // Serial.println ();

        for (uint32_t val : utf8_val) {
            Serial.printf ("%X | %d", val, Check_Location (val));
        }
    }

    tft.setTextWrap (true, true);
    tft.setTextColor (TFT_WHITE, TFT_BLACK);
    tft.setCursor (10, 70);

    for (uint8_t i = 0, j = 0; i <= title.length (); j++) {
        uint32_t val = utf8_val[j];

        switch (Check_Location (val)) {
            case latin: {
                tft.loadFont (file_latin, SD);
                tft.print (title.substring (i, ++i));
                tft.unloadFont ();
            } break;

            case cyrillic_greek: {
                tft.loadFont (file_cyrillic_greek, SD);
                tft.print (title.substring (i, i + 2));
                tft.unloadFont ();
                i += 2;
            } break;

            case ex_latin: {
                tft.loadFont (file_ex_latin, SD);
                tft.print (title.substring (i, i + 2));
                tft.unloadFont ();
                i += 2;
            } break;

            case japanese: {
                tft.loadFont (file_japanese, SD);
                tft.print (title.substring (i, i + 3));
                tft.unloadFont ();
                i += 3;
            } break;

            case hangul_1: {
                tft.loadFont (file_hangul_1, SD);
                tft.print (title.substring (i, i + 3));
                tft.unloadFont ();
                i += 3;
            } break;

            case hangul_2: {
                tft.loadFont (file_hangul_2, SD);
                tft.print (title.substring (i, i + 3));
                tft.unloadFont ();
                i += 3;
            } break;

            case hangul_3_1: {
                tft.loadFont (file_hangul_3_1, SD);
                tft.print (title.substring (i, i + 3));
                tft.unloadFont ();
                i += 3;
            } break;

            case hangul_3_2: {
                tft.loadFont (file_hangul_3_2, SD);
                tft.print (title.substring (i, i + 3));
                tft.unloadFont ();
                i += 3;
            } break;

            case hangul_4_1: {
                tft.loadFont (file_hangul_4_1, SD);
                tft.print (title.substring (i, i + 3));
                tft.unloadFont ();
                i += 3;
            } break;
            
            case hangul_4_2: {
                tft.loadFont (file_hangul_4_2, SD);
                tft.print (title.substring (i, i + 3));
                tft.unloadFont ();
                i += 3;
            } break;

            case hangul_5_1: {
                tft.loadFont (file_hangul_5_1, SD);
                tft.print (title.substring (i, i + 3));
                tft.unloadFont ();
                i += 3;
            } break;

            case hangul_5_2: {
                tft.loadFont (file_hangul_5_2, SD);
                tft.print (title.substring (i, i + 3));
                tft.unloadFont ();
                i += 3;
            } break;

            case cj_1: {
                tft.loadFont (file_cj_1, SD);
                tft.print (title.substring (i, i + 3));
                tft.unloadFont ();
                i += 3;
            } break;

            case cj_2_1: {
                tft.loadFont (file_cj_2_1, SD);
                tft.print (title.substring (i, i + 3));
                tft.unloadFont ();
                i += 3;
            } break;

            case cj_2_2: {
                tft.loadFont (file_cj_2_2, SD);
                tft.print (title.substring (i, i + 3));
                tft.unloadFont ();
                i += 3;
            } break;

            case cj_3: {
                tft.loadFont (file_cj_3, SD);
                tft.print (title.substring (i, i + 3));
                tft.unloadFont ();
                i += 3;
            } break;

            case cj_4_1: {
                tft.loadFont (file_cj_4_1, SD);
                tft.print (title.substring (i, i + 3));
                tft.unloadFont ();
                i += 3;
            } break;

            case cj_4_2: {
                tft.loadFont (file_cj_4_2, SD);
                tft.print (title.substring (i, i + 3));
                tft.unloadFont ();
                i += 3;
            } break;

            case cj_5_1: {
                tft.loadFont (file_cj_5_1, SD);
                tft.print (title.substring (i, i + 3));
                tft.unloadFont ();
                i += 3;
            } break;

            case cj_5_2: {
                tft.loadFont (file_cj_5_2, SD);
                tft.print (title.substring (i, i + 3));
                tft.unloadFont ();
                i += 3;
            } break;

            case cj_6_1: {
                tft.loadFont (file_cj_6_1, SD);
                tft.print (title.substring (i, i + 3));
                tft.unloadFont ();
                i += 3;
            } break;

            case cj_6_2: {
                tft.loadFont (file_cj_6_2, SD);
                tft.print (title.substring (i, i + 3));
                tft.unloadFont ();
                i += 3;
            } break;

            case cj_7_1: {
                tft.loadFont (file_cj_7_1, SD);
                tft.print (title.substring (i, i + 3));
                tft.unloadFont ();
                i += 3;
            } break;

            case cj_7_2: {
                tft.loadFont (file_cj_7_2, SD);
                tft.print (title.substring (i, i + 3));
                tft.unloadFont ();
                i += 3;
            } break;

            case cj_8_1: {
                tft.loadFont (file_cj_8_1, SD);
                tft.print (title.substring (i, i + 3));
                tft.unloadFont ();
                i += 3;
            } break;

            case cj_8_2: {
                tft.loadFont (file_cj_8_2, SD);
                tft.print (title.substring (i, i + 3));
                tft.unloadFont ();
                i += 3;
            } break;

            case cj_9: {
                tft.loadFont (file_cj_9, SD);
                tft.print (title.substring (i, i + 3));
                tft.unloadFont ();
                i += 3;
            } break;

            case cj_10: {
                tft.loadFont (file_cj_10, SD);
                tft.print (title.substring (i, i + 3));
                tft.unloadFont ();
                i += 3;
            } break;

            case cj_11: {
                tft.loadFont (file_cj_11, SD);
                tft.print (title.substring (i, i + 4));
                tft.unloadFont ();
                i += 4;
            } break;

            case cj_12: {
                tft.loadFont (file_cj_12, SD);
                tft.print (title.substring (i, i + 4));
                tft.unloadFont ();
                i += 4;
            } break;
        }
    } 
    tft.println ();
}

void loop () {}

bool Between (uint32_t enc_byte, uint32_t low, uint32_t high) {
    return (enc_byte >= low && enc_byte <= high);
}

uint8_t Check_Location (uint32_t enc_byte) {
    if (enc_byte == 0)  return FONT_ERROR;
    if (Between (enc_byte, 0x0, 0x7F))              return latin;
    if (Between (enc_byte, 0xCDB0, 0xDEBF))         return cyrillic_greek;
    if (Between (enc_byte, 0xC280, 0xC98F))         return ex_latin;
    if (Between (enc_byte, 0xE38180, 0xE383BF))     return japanese;
    if (Between (enc_byte, 0xE18480, 0xE187BF))     return hangul_1;

    if (Between (enc_byte, 0xE18480, 0xE187BF))     return hangul_1;
    if (Between (enc_byte, 0xE18480, 0xE187BF))     return hangul_1;
    if (Between (enc_byte, 0xE18480, 0xE187BF))     return hangul_1;
    if (Between (enc_byte, 0xE18480, 0xE187BF))     return hangul_1;
    if (Between (enc_byte, 0xE18480, 0xE187BF))     return hangul_1;
    if (Between (enc_byte, 0xE18480, 0xE187BF))     return hangul_1;
    if (Between (enc_byte, 0xE18480, 0xE187BF))     return hangul_1;
    if (Between (enc_byte, 0xE18480, 0xE187BF))     return cj_1;
    if (Between (enc_byte, 0xE18480, 0xE187BF))     return cj_1;
    if (Between (enc_byte, 0xE18480, 0xE187BF))     return cj_1;
    if (Between (enc_byte, 0xE18480, 0xE187BF))     return cj_1;
    if (Between (enc_byte, 0xE18480, 0xE187BF))     return cj_1;
    if (Between (enc_byte, 0xE18480, 0xE187BF))     return cj_1;
    if (Between (enc_byte, 0xE18480, 0xE187BF))     return cj_1;
    if (Between (enc_byte, 0xE18480, 0xE187BF))     return cj_1;
    if (Between (enc_byte, 0xE18480, 0xE187BF))     return cj_1;
    if (Between (enc_byte, 0xE18480, 0xE187BF))     return cj_1;
    if (Between (enc_byte, 0xE18480, 0xE187BF))     return cj_1;
    if (Between (enc_byte, 0xE18480, 0xE187BF))     return cj_1;
    if (Between (enc_byte, 0xE18480, 0xE187BF))     return cj_1;
    if (Between (enc_byte, 0xE18480, 0xE187BF))     return cj_1;
    if (Between (enc_byte, 0xE18480, 0xE187BF))     return cj_1;
    if (Between (enc_byte, 0xE18480, 0xE187BF))     return cj_1;
    if (Between (enc_byte, 0xE18480, 0xE187BF))     return cj_1;
    if (Between (enc_byte, 0xE18480, 0xE187BF))     return cj_1;
}

void listDir (FS &fs, const char* dirname, uint8_t levels) {
    Serial.printf ("Listing directory: %S\n", dirname);

    File root = fs.open (dirname);
    if (!root) {
        Serial.println ("Failed to open directory");
        return;
    } 
    if (!root.isDirectory ()) {
        Serial.println ("Not a directory");
        return;
    }

    File file = root.openNextFile ();
    while (file) {
        if (file.isDirectory ()) {
            Serial.print ("  DIR : ");
            Serial.println (file.name ());
            if (levels) {
                listDir (fs, file.name (), levels - 1);
            }
        } else {
            Serial.print ("  FILE : ");
            Serial.print (file.name ());
            Serial.print ("  SIZE : ");
            Serial.println (file.size ());
        }
        file = root.openNextFile ();
    }
}
