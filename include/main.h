#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Si4713.h>
#include <Preferences.h>
#include <BluetoothA2DPSink.h>
// #include <driver/dac.h>
#include <SD.h>
#include <TFT_eSPI.h>

struct data_t {
    // SCREEN CONTROLLER OBJECT
    TFT_eSPI tft = TFT_eSPI ();

    // SWITCH FUNCTION VARIABLES
    uint8_t function;
    uint8_t previous_function;
    
    
    // DEFAULT VALUES FOR THE STATION, VOLUME, AND BRIGHTNESS
    uint16_t radio_station = 9390;
    uint8_t phone_volume = 127;
    uint8_t screen_brightness = 127;
    
    // VARIABLES FOR SONG METADATA
    String song_title = "";
    String song_artist = "";
    String song_album = "";
    
    // TEMPORARY TEXT TIMER VARIABLES
    unsigned long temp_text_start = 0;
    unsigned long temp_text_duration = 0;

    SemaphoreHandle_t mutex = xSemaphoreCreateMutex ();
};

/*
 * FUNCTION PROTOTYPES
 */
void Load_Intro_Screen (data_t &data);
void Print_Song_Data (data_t &data);
void Print_Perma_Text (data_t &data, String text);
void Print_Temp_Text (data_t &data, String text, uint32_t ms);
void Print_Text (data_t &data, uint8_t song_type);
void Change_Function (data_t &data);

#define RESETPIN    13

#define PLAYBACK_FUNCTION       1
#define STATION_FUNCTION        2
#define BRIGHTNESS_FUNCTION     3
#define VOLUME_FUNCTION         4

#define DEFAULT_TEMP_TEXT_TIME  2000

#define HIGHEST_RADIO_FREQUENCY 10790
#define LOWEST_RADIO_FREQUENCY  8810
#define MAX_PHONE_VOLUME        127
#define MIN_PHONE_VOLUME        0
#define MAX_SCREEN_BRIGHTNESS   255
#define MIN_SCREEN_BRIGHTNESS   0

#define PLAY_PAUSE_ICON         "\u0700"
#define PREVIOUS_ICON           "\u0701"
#define NEXT_ICON               "\u0702"
#define LEFT_ICON               "\u0703"
#define RIGHT_ICON              "\u0704"
#define DIMMER_SCREEN_ICON      "\u0705"
#define BRIGHTER_SCREEN_ICON    "\u0706"
#define LOWER_VOL_ICON          "\u0707"
#define HIGHER_VOL_ICON         "\u0708"

#define TEXT_TYPE_SONG_TITLE    1
#define TEXT_TYPE_SONG_ARTIST   2
#define TEXT_TYPE_SONG_ALBUM    3
#define TEXT_TYPE_SONG_TIME     4
#define TEXT_TYPE_PHONE_TIME    5

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

#define BETWEEN_LATIN(val)              (val >= 0x0 && val <= 0x7F) 
#define BETWEEN_CYRILLIC_GREEK(val)     (val >= 0xCDB0 && val <= 0xDEBF)
#define BETWEEN_EX_LATIN(val)           (val >= 0xC280 && val <= 0xC98F) 
#define BETWEEN_JAPANESE(val)           (val >= 0xE38180 && val <= 0xE383BF)
#define BETWEEN_HANGUL_1(val)           (val >= 0xE18480 && val <= 0xE187BF)
#define BETWEEN_HANGUL_2(val)           (val >= 0xE384B1 && val <= 0xE3868E)
#define BETWEEN_HANGUL_3_1(val)         (val >= 0xEAB080 && val <= 0xEB8FBF)
#define BETWEEN_HANGUL_3_2(val)         (val >= 0xEB9080 && val <= 0xEBAFBF)
#define BETWEEN_HANGUL_4_1(val)         (val >= 0xEBB080 && val <= 0xEC8FBF)
#define BETWEEN_HANGUL_4_2(val)         (val >= 0xEC9080 && val <= 0xECAFBF)
#define BETWEEN_HANGUL_5_1(val)         (val >= 0xECB080 && val <= 0xED8FBF)
#define BETWEEN_HANGUL_5_2(val)         (val >= 0xED9080 && val <= 0xED9EA3)
#define BETWEEN_CJ_1(val)               (val >= 0xE38080 && val <= 0xE380BF)
#define BETWEEN_CJ_2_1(val)             (val >= 0xE39080 && val <= 0xE3AFBF)
#define BETWEEN_CJ_2_2(val)             (val >= 0xE3B080 && val <= 0xE48FBF)
#define BETWEEN_CJ_3(val)               (val >= 0xE49080 && val <= 0xE4B6BF)
#define BETWEEN_CJ_4_1(val)             (val >= 0xE4B880 && val <= 0xE597BF)
#define BETWEEN_CJ_4_2(val)             (val >= 0xE59880 && val <= 0xE5B7BF)
#define BETWEEN_CJ_5_1(val)             (val >= 0xE5B880 && val <= 0xE697BF)
#define BETWEEN_CJ_5_2(val)             (val >= 0xE69880 && val <= 0xE6B7BF)
#define BETWEEN_CJ_6_1(val)             (val >= 0xE6B880 && val <= 0xE797BF)
#define BETWEEN_CJ_6_2(val)             (val >= 0xE79880 && val <= 0xE7B7BF)
#define BETWEEN_CJ_7_1(val)             (val >= 0xE7B880 && val <= 0xE897BF)
#define BETWEEN_CJ_7_2(val)             (val >= 0xE89880 && val <= 0xE8B7BF)
#define BETWEEN_CJ_8_1(val)             (val >= 0xE8B880 && val <= 0xE997BF)
#define BETWEEN_CJ_8_2(val)             (val >= 0xE99880 && val <= 0xE9B7BF)
#define BETWEEN_CJ_9(val)               (val >= 0xE9B880 && val <= 0xE9BFBF)
#define BETWEEN_CJ_10(val)              (val >= 0xEFA480 && val <= 0xEFA9AD)
#define BETWEEN_CJ_11(val)              (val >= 0xF0A08080 && val <= 0xF0ADBFBF)
#define BETWEEN_CJ_12(val)              (val >= 0xF0B08080 && val <= 0xF0B1BFBF)

#define CHECK_LOCATION(val)             (BETWEEN_LATIN (val) ? latin : (BETWEEN_CYRILLIC_GREEK (val) ? cyrillic_greek : (BETWEEN_EX_LATIN (val) ? ex_latin : (BETWEEN_JAPANESE (val) ? japanese : (BETWEEN_HANGUL_1 (val) ? hangul_1 : (BETWEEN_HANGUL_2 (val) ? hangul_2 : (BETWEEN_HANGUL_3_1 (val) ? hangul_3_1 : (BETWEEN_HANGUL_3_2 (val) ? hangul_3_2 : (BETWEEN_HANGUL_4_1 (val) ? hangul_4_1 : (BETWEEN_HANGUL_4_2 (val) ? hangul_4_2 : (BETWEEN_HANGUL_5_1 (val) ? hangul_5_1 : (BETWEEN_HANGUL_5_2 (val) ? hangul_5_2 : (BETWEEN_CJ_1 (val) ? cj_1 : (BETWEEN_CJ_2_1 (val) ? cj_2_1 : (BETWEEN_CJ_2_2 (val) ? cj_2_2 : (BETWEEN_CJ_3 (val) ? cj_3 : (BETWEEN_CJ_4_1 (val) ? cj_4_1 : (BETWEEN_CJ_4_2 (val) ? cj_4_2 : (BETWEEN_CJ_5_1 (val) ? cj_5_1 : (BETWEEN_CJ_5_2 (val) ? cj_5_2 : (BETWEEN_CJ_6_1 (val) ? cj_6_1 : (BETWEEN_CJ_6_2 (val) ? cj_6_2 : (BETWEEN_CJ_7_1 (val) ? cj_7_1 : (BETWEEN_CJ_7_2 (val) ? cj_7_2 : (BETWEEN_CJ_8_1 (val) ? cj_8_1 : (BETWEEN_CJ_8_2 (val) ? cj_8_2 : (BETWEEN_CJ_9 (val) ? cj_9 : (BETWEEN_CJ_10 (val) ? cj_10 : (BETWEEN_CJ_11 (val) ? cj_11 : (BETWEEN_CJ_12 (val) ? cj_12 : FONT_ERROR))))))))))))))))))))))))))))))

#define file_functions_symbols      "Functions_symbols_20"
#define file_functions              "Functions"

#define file_main_latin           "Maintext_latin"
#define file_main_cyrillic_greek  "Maintext_cyrillic_greek"
#define file_main_ex_latin        "Maintext_ex_latin"
#define file_main_japanese        "Maintext_hiragana_katakana"
#define file_main_hangul_1        "Maintext_hangul_1"
#define file_main_hangul_2        "Maintext_hangul_2"
#define file_main_hangul_3_1      "Maintext_hangul_3_1"
#define file_main_hangul_3_2      "Maintext_hangul_3_2"
#define file_main_hangul_4_1      "Maintext_hangul_4_1"
#define file_main_hangul_4_2      "Maintext_hangul_4_2"
#define file_main_hangul_5_1      "Maintext_hangul_5_1"
#define file_main_hangul_5_2      "Maintext_hangul_5_2"
#define file_main_cj_1            "Maintext_cj_1"
#define file_main_cj_2_1          "Maintext_cj_2_1"
#define file_main_cj_2_2          "Maintext_cj_2_2"
#define file_main_cj_3            "Maintext_cj_3"
#define file_main_cj_4_1          "Maintext_cj_4_1"
#define file_main_cj_4_2          "Maintext_cj_4_2"
#define file_main_cj_5_1          "Maintext_cj_5_1"
#define file_main_cj_5_2          "Maintext_cj_5_2"
#define file_main_cj_6_1          "Maintext_cj_6_1"
#define file_main_cj_6_2          "Maintext_cj_6_2"
#define file_main_cj_7_1          "Maintext_cj_7_1"
#define file_main_cj_7_2          "Maintext_cj_7_2"
#define file_main_cj_8_1          "Maintext_cj_8_1"
#define file_main_cj_8_2          "Maintext_cj_8_2"
#define file_main_cj_9            "Maintext_cj_9"
#define file_main_cj_10           "Maintext_cj_10"
#define file_main_cj_11           "Maintext_cj_11"
#define file_main_cj_12           "Maintext_cj_12"

#define file_sub_latin           "Subtext_latin"
#define file_sub_cyrillic_greek  "Subtext_cyrillic_greek"
#define file_sub_ex_latin        "Subtext_ex_latin"
#define file_sub_japanese        "Subtext_hiragana_katakana"
#define file_sub_hangul_1        "Subtext_hangul_1"
#define file_sub_hangul_2        "Subtext_hangul_2"
#define file_sub_hangul_3        "Subtext_hangul_3"
#define file_sub_hangul_4        "Subtext_hangul_4"
#define file_sub_hangul_5        "Subtext_hangul_5"
#define file_sub_cj_1            "Subtext_cj_1"
#define file_sub_cj_2            "Subtext_cj_2"
#define file_sub_cj_3            "Subtext_cj_3"
#define file_sub_cj_4            "Subtext_cj_4"
#define file_sub_cj_5            "Subtext_cj_5"
#define file_sub_cj_6            "Subtext_cj_6"
#define file_sub_cj_7            "Subtext_cj_7"
#define file_sub_cj_8            "Subtext_cj_8"
#define file_sub_cj_9            "Subtext_cj_9"
#define file_sub_cj_10           "Subtext_cj_10"
#define file_sub_cj_11           "Subtext_cj_11"
#define file_sub_cj_12           "Subtext_cj_12"

#define file_time_15        "Time_15"
#define file_time_30        "Time_30"
