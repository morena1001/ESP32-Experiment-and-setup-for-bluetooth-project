#include <main.h>

void Load_Intro_Screen (data_t &data) {
    if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
        data.tft.loadFont (file_main_latin, SD);
        data.tft.setCursor (51, 90);
        data.tft.print ("Vandar Systems");
        data.tft.setCursor (56, 120);
        data.tft.print ("FM Transmitter");
        data.tft.unloadFont ();
        xSemaphoreGive (data.mutex);
    }
}

void Print_Song_Data (data_t &data) {
    if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
        data.tft.fillRect (0, 20, 320, 115, TFT_BLACK);
        xSemaphoreGive (data.mutex);
    }
    Print_Text (data, TEXT_TYPE_SONG_TITLE);
    Print_Text (data, TEXT_TYPE_SONG_ALBUM);
    Print_Text (data, TEXT_TYPE_SONG_ARTIST);
}

void Print_Perma_Text (data_t &data, String text) {
    if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
        data.tft.loadFont (file_main_latin, SD);
        data.tft.fillRect (0, 70, 320, 65, TFT_BLACK);
        data.tft.setCursor (10, 70);
        data.tft.print (text);
        data.tft.unloadFont ();
        xSemaphoreGive (data.mutex);
    }
}

void Print_Temp_Text (data_t &data, String text, uint32_t ms) {
    if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
        data.tft.loadFont (file_main_latin, SD);
        data.tft.fillRect (0, 70, 320, 65, TFT_BLACK);
        data.tft.setCursor (10, 70);
        data.tft.print (text);
        data.tft.unloadFont ();
        xSemaphoreGive (data.mutex);
    }

    data.temp_text_start = millis ();
    data.temp_text_duration = ms;
}

void Print_Text (data_t &data, uint8_t song_type) {
    String text = "";
    bool maintext = false;

    switch (song_type) {
        case TEXT_TYPE_SONG_TITLE:
            text = data.song_title;
            if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                data.tft.fillRect (0, 60, 320, 50, TFT_BLACK);
                data.tft.setCursor (10, 70);
                xSemaphoreGive (data.mutex);
            }
            maintext = true;
            break;
        case TEXT_TYPE_SONG_ALBUM:
            text = data.song_album;
            if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                data.tft.fillRect (0, 135, 320, 40, TFT_BLACK);
                data.tft.setCursor (20, 135);
                xSemaphoreGive (data.mutex);
            }
            break;
            
            case TEXT_TYPE_SONG_ARTIST:
            text = data.song_artist;
            if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                data.tft.fillRect (0, 110, 320, 40, TFT_BLACK);
                data.tft.setCursor (20, 110);
                xSemaphoreGive (data.mutex);
            }
            break;
    }

    uint32_t val = 0;
    
    for (int i = 0; i < text.length (); ) {
        switch (song_type) {
            case TEXT_TYPE_SONG_TITLE:   if (text != data.song_title)       return;
            case TEXT_TYPE_SONG_ALBUM:   if (text != data.song_album)       return;
            case TEXT_TYPE_SONG_ARTIST:  if (text != data.song_artist)      return;
        }
        
        val = (0xFF & static_cast <uint32_t> (text[i]));

        if (val >= 0xF0) {
            val <<= 24;
            val += (0xFF0000 & (static_cast <uint32_t> (text[i + 1]) << 16)) + (0xFF00 & (static_cast <uint32_t> (text[i + 2]) << 8)) + (0xFF & static_cast <uint32_t> (text[i + 3]));
        } else if (val >= 0xE0) {
            val <<= 16;
            val += (0xFF00 & (static_cast <uint32_t> (text[i + 1]) << 8)) + (0xFF & static_cast <uint32_t> (text[i + 2]));
        } else if (val >= 0xC0) {
            val <<= 2;
            val += (0xFF & static_cast <uint32_t> (text[i + 1]));
        }

        switch (CHECK_LOCATION (val)) {
            case latin: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_latin : file_sub_latin, SD);
                    data.tft.print (text.substring (i, i + 1));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i++;
            } break;

            case cyrillic_greek: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_cyrillic_greek : file_sub_cyrillic_greek, SD);
                    data.tft.print (text.substring (i, i + 2));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 2;
            } break;

            case ex_latin: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_ex_latin : file_sub_ex_latin, SD);
                    data.tft.print (text.substring (i, i + 2));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 2;
            } break;

            case japanese: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_japanese : file_sub_japanese, SD);
                    data.tft.print (text.substring (i, i + 3));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 3;
            } break;

            case hangul_1: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_hangul_1 : file_sub_hangul_1, SD);
                    data.tft.print (text.substring (i, i + 3));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 3;
            } break;

            case hangul_2: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_hangul_2 : file_sub_hangul_2, SD);
                    data.tft.print (text.substring (i, i + 3));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 3;
            } break;

            case hangul_3_1: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_hangul_3_1 : file_sub_hangul_3, SD);
                    data.tft.print (text.substring (i, i + 3));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 3;
            } break;

            case hangul_3_2: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_hangul_3_2 : file_sub_hangul_3, SD);
                    data.tft.print (text.substring (i, i + 3));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 3;
            } break;

            case hangul_4_1: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_hangul_4_1 : file_sub_hangul_4, SD);
                    data.tft.print (text.substring (i, i + 3));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 3;
            } break;
            
            case hangul_4_2: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_hangul_4_2 : file_sub_hangul_4, SD);
                    data.tft.print (text.substring (i, i + 3));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 3;
            } break;

            case hangul_5_1: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_hangul_5_1 : file_sub_hangul_5, SD);
                    data.tft.print (text.substring (i, i + 3));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 3;
            } break;

            case hangul_5_2: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_hangul_5_2 : file_sub_hangul_5, SD);
                    data.tft.print (text.substring (i, i + 3));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 3;
            } break;

            case cj_1: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_cj_1 : file_sub_cj_1, SD);
                    data.tft.print (text.substring (i, i + 3));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 3;
            } break;

            case cj_2_1: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_cj_2_1 : file_sub_cj_2, SD);
                    data.tft.print (text.substring (i, i + 3));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 3;
            } break;

            case cj_2_2: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_cj_2_2 : file_sub_cj_2, SD);
                    data.tft.print (text.substring (i, i + 3));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 3;
            } break;

            case cj_3: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_cj_3 : file_sub_cj_3, SD);
                    data.tft.print (text.substring (i, i + 3));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 3;
            } break;

            case cj_4_1: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_cj_4_1 : file_sub_cj_4, SD);
                    data.tft.print (text.substring (i, i + 3));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 3;
            } break;

            case cj_4_2: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_cj_4_2 : file_sub_cj_4, SD);
                    data.tft.print (text.substring (i, i + 3));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 3;
            } break;

            case cj_5_1: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_cj_5_1 : file_sub_cj_5, SD);
                    data.tft.print (text.substring (i, i + 3));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 3;
            } break;

            case cj_5_2: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_cj_5_2 : file_sub_cj_5, SD);
                    data.tft.print (text.substring (i, i + 3));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 3;
            } break;

            case cj_6_1: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_cj_6_1 : file_sub_cj_6, SD);
                    data.tft.print (text.substring (i, i + 3));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 3;
            } break;

            case cj_6_2: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_cj_6_2 : file_sub_cj_6, SD);
                    data.tft.print (text.substring (i, i + 3));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 3;
            } break;

            case cj_7_1: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_cj_7_1 : file_sub_cj_7, SD);
                    data.tft.print (text.substring (i, i + 3));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 3;
            } break;

            case cj_7_2: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_cj_7_2 : file_sub_cj_7, SD);
                    data.tft.print (text.substring (i, i + 3));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 3;
            } break;

            case cj_8_1: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_cj_8_1 : file_sub_cj_8, SD);
                    data.tft.print (text.substring (i, i + 3));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 3;
            } break;

            case cj_8_2: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_cj_8_2 : file_sub_cj_8, SD);
                    data.tft.print (text.substring (i, i + 3));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 3;
            } break;

            case cj_9: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_cj_9 : file_sub_cj_9, SD);
                    data.tft.print (text.substring (i, i + 3));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 3;
            } break;

            case cj_10: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_cj_10 : file_sub_cj_10, SD);
                    data.tft.print (text.substring (i, i + 3));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 3;
            } break;

            case cj_11: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_cj_11 : file_sub_cj_11, SD);
                    data.tft.print (text.substring (i, i + 4));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }   
                i += 4;
            } break;

            case cj_12: {
                if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                    data.tft.loadFont (maintext ? file_main_cj_12 : file_sub_cj_12, SD);
                    data.tft.print (text.substring (i, i + 4));
                    data.tft.unloadFont ();
                    xSemaphoreGive (data.mutex);
                }
                i += 4;
            } break;
        }
    }
}

void Change_Function (data_t &data) {
    if (data.function == PLAYBACK_FUNCTION && data.previous_function != data.function) {
        data.previous_function = data.function;
        data.tft.loadFont (file_functions_symbols, SD);
        data.tft.fillRect (10, 215, 30, 30, TFT_BLACK);
        data.tft.setCursor (10, 215);
        data.tft.print (PREVIOUS_ICON);
        data.tft.fillRect (160, 215, 30, 30, TFT_BLACK);
        data.tft.setCursor (160, 215);
        data.tft.print (NEXT_ICON);
        data.tft.unloadFont ();
    }
    else if (data.function == STATION_FUNCTION && data.previous_function != data.function) {
        data.previous_function = data.function;
        data.tft.loadFont (file_functions_symbols, SD);
        data.tft.fillRect (10, 215, 30, 30, TFT_BLACK);
        data.tft.setCursor (10, 215);
        data.tft.print (LEFT_ICON);
        data.tft.fillRect (160, 215, 30, 30, TFT_BLACK);
        data.tft.setCursor (160, 215);
        data.tft.print (RIGHT_ICON);
        data.tft.unloadFont ();
    }
    else if (data.function == BRIGHTNESS_FUNCTION && data.previous_function != data.function) {
        data.previous_function = data.function;
        data.tft.loadFont (file_functions_symbols, SD);
        data.tft.fillRect (10, 215, 30, 30, TFT_BLACK);
        data.tft.setCursor (10, 215);
        data.tft.print (DIMMER_SCREEN_ICON);
        data.tft.fillRect (160, 215, 30, 30, TFT_BLACK);
        data.tft.setCursor (160, 215);
        data.tft.print (BRIGHTER_SCREEN_ICON);
        data.tft.unloadFont ();
    }
    else if (data.function == VOLUME_FUNCTION && data.previous_function != data.function) {
        data.previous_function = data.function;
        data.tft.loadFont (file_functions_symbols, SD);
        data.tft.fillRect (10, 215, 30, 30, TFT_BLACK);
        data.tft.setCursor (10, 215);
        data.tft.print (LOWER_VOL_ICON);
        data.tft.fillRect (160, 215, 30, 30, TFT_BLACK);
        data.tft.setCursor (160, 215);
        data.tft.print (HIGHER_VOL_ICON);
        data.tft.unloadFont ();
    }
}

