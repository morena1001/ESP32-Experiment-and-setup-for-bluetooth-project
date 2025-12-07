// #include <Arduino.h>
// #include <Wire.h>
// #include <Adafruit_Si4713.h>
// #include <Preferences.h>
// #include <BluetoothA2DPSink.h>
// #include <driver/dac.h>
// #include <SD.h>
// #include <TFT_eSPI.h>

#include <main.h>


Adafruit_Si4713 radio = Adafruit_Si4713 (RESETPIN);
AnalogAudioStream out;
BluetoothA2DPSink a2dp_sink (out);
Preferences preferences;
data_t data;

bool is_active = true; // CHECKING IF THE CONNECTED DEVICE PAUSED THE MUSIC
bool sent_connected_message = false; // FOR SENDING MESSAGE TO SCREEN
bool sent_disconnected_message = false; 

const int pp_pin = 32; // PAUSE / PLAY BUTTON
const int next_pin = 33; // NEXT BUTTON
const int prev_pin = 27; // PREVIOUS BUTTON
const int func_1_pin = 36; // PIN FOR PLAYBACK SWITCH OPTION
const int func_2_pin = 39; // PIN FOR STATION SWITCH OPTION
const int func_3_pin = 34; // PIN FOR BRIGHTNESS SWITCH OPTION
const int func_4_pin = 35; // PIN FOR VOLUME SWITCH OPTION

/*
 * HANDLERS AND FUNCTION PROTOTYPES FOR CONCURRENCY
 */
TaskHandle_t /*bt_task,*/ in_task;
// void Bt_Task_Handler (void* param);
void In_Task_Handler (void* param);


/*
 * TIMER BASED DEBOUNCED VARIABLES FOR BUTTONS 
 *
 * MIGHT SWITCH TO INTERRUPTS
 */
unsigned long pp_last_db_time = 0;
unsigned long next_last_db_time = 0;
unsigned long prev_last_db_time = 0;
unsigned long func_1_last_db_time = 0;
unsigned long func_2_last_db_time = 0;
unsigned long func_3_last_db_time = 0;
unsigned long func_4_last_db_time = 0;

unsigned long pp_db_delay = 50;
unsigned long next_db_delay = 50;
unsigned long prev_db_delay = 50;
unsigned long func_1_db_delay = 50;
unsigned long func_2_db_delay = 50;
unsigned long func_3_db_delay = 50;
unsigned long func_4_db_delay = 50;

int pp_state;            
int next_state;            
int prev_state;            
int func_1_state;
int func_2_state;
int func_3_state;
int func_4_state;

int pp_last_state = LOW;  
int next_last_state = LOW;  
int prev_last_state = LOW; 
int func_1_last_state = LOW;
int func_2_last_state = LOW;
int func_3_last_state = LOW;
int func_4_last_state = LOW;

void avrc_metadata_callback (uint8_t id, const uint8_t *text) {
    if (a2dp_sink.is_connected ()) {
        Serial.printf ("==> AVRC metadata rsp: attribute id 0x%x, %s\n", id, text);

        switch (id) {
            case ESP_AVRC_MD_ATTR_TITLE:
                if (data.song_title == String ((char *) text))    break;    
                data.song_title = String ((char*) text); 
                if (data.temp_text_duration == 0 && sent_connected_message)    Print_Text (data, TEXT_TYPE_SONG_TITLE);
            break;
            case ESP_AVRC_MD_ATTR_ARTIST:   
                if (data.song_artist == String ((char *) text))    break;  
                data.song_artist = String ((char*) text);
                if (data.temp_text_duration == 0 && sent_connected_message)    Print_Text (data, TEXT_TYPE_SONG_ARTIST);
            break;
            case ESP_AVRC_MD_ATTR_ALBUM:    
                if (data.song_album == String ((char *) text))    break;  
                data.song_album = String ((char*) text);
                if (data.temp_text_duration == 0 && sent_connected_message)    Print_Text (data, TEXT_TYPE_SONG_ALBUM);
            break;
        }
    }
}

void avrc_rn_playstatus_callback (esp_avrc_playback_stat_t playback) {
    switch (playback) {
    case esp_avrc_playback_stat_t::ESP_AVRC_PLAYBACK_PAUSED:
        Serial.println ("Stopped.");
        is_active = false;
        break;
    case esp_avrc_playback_stat_t::ESP_AVRC_PLAYBACK_PLAYING:
        Serial.println ("Playing.");
        is_active = true;
        break;
    default:
        Serial.printf ("Got unknown playback status %d\n", playback);
    }
}

// // Then somewhere in your sketch:
// void read_data_stream(const uint8_t *data, uint32_t length)
// {
//       int16_t *ptr16 = (int16_t*) data; 
//       uint32_t len16 = length/2;
//       for(int j=0;j<len16;j++){
//            ptr16[j] = 0.5 * ptr16[j];
//       }
// }


void setup () {
    Serial.begin (115200);

    // SET UP RADIO
    Serial.println ("Starting radio");

    while (!radio.begin ())     Serial.println ("Could not find radio");
    Serial.println ("Radio found");

    radio.setTXpower (115);
    Serial.printf ("Tuning into %d.%d\n", data.radio_station / 100, data.radio_station % 100);
    radio.tuneFM (data.radio_station);

    // SET UP FONTS IN SD CARD
    while (!SD.begin (2))   Serial.println ("Card mount failed");
    Serial.println ("Card mount success");

    // SET UP SCREEN;
    data.tft.init ();
    data.tft.setRotation (1);
    data.tft.fillScreen (TFT_BLACK);
    data.tft.setTextColor (TFT_WHITE);
    Serial.println ("TFT initialization done");
    Load_Intro_Screen (data);

    // GET SCREEN BRIGHTNESS
    data.screen_brightness = data.tft.readcommand8 (ILI9341_RDDISBV, 0);
    Serial.printf ("Initial screen brightness is: %d\n", data.screen_brightness);

    // SET UP buttons, switches, and interrupts
    pinMode (pp_pin, INPUT);
    pinMode (next_pin, INPUT);
    pinMode (prev_pin, INPUT);
    pinMode (func_1_pin, INPUT);
    pinMode (func_2_pin, INPUT);
    pinMode (func_3_pin, INPUT);
    pinMode (func_4_pin, INPUT);

    // attachInterrupt (pp_pin, Middle_Button, CHANGE);
    // attachInterrupt (next_pin, Left_Button, CHANGE);
    // attachInterrupt (prev_pin, Right_Button, CHANGE);
    // attachInterrupt (func_1_pin, Function_1_Switch, RISING);
    // attachInterrupt (func_2_pin, Function_2_Switch, RISING);
    // attachInterrupt (func_3_pin, Function_3_Switch, RISING);
    // attachInterrupt (func_4_pin, Function_4_Switch, RISING);

    // SET UP A2DP SINK
    a2dp_sink.set_avrc_metadata_attribute_mask (ESP_AVRC_MD_ATTR_TITLE | ESP_AVRC_MD_ATTR_ARTIST | ESP_AVRC_MD_ATTR_ALBUM);
    a2dp_sink.set_avrc_metadata_callback (avrc_metadata_callback);
    a2dp_sink.set_avrc_rn_playstatus_callback (avrc_rn_playstatus_callback);
    // a2dp_sink.set_raw_stream_reader(read_data_stream);

    a2dp_sink.start ("VS transmitter", true);
    Serial.println ("Started bluetooth");

    delay (1000);

    // PRINT OUT FUNCTION ICONS AND NOT CONNECTED TEXT
    data.tft.fillScreen (TFT_BLACK);
    data.tft.setCursor (85, 215);
    data.tft.loadFont (file_functions_symbols, SD);
    data.tft.print (PLAY_PAUSE_ICON);

    if (digitalRead (func_1_pin)) {
        data.function = PLAYBACK_FUNCTION;
        Serial.println ("Function 1");
        data.tft.setCursor (10, 215);
        data.tft.print (PREVIOUS_ICON);
        data.tft.setCursor (160, 215);
        data.tft.print (NEXT_ICON);
    }
    else if (digitalRead (func_2_pin)) {
        data.function = STATION_FUNCTION;
        Serial.println ("Function 2");
        data.tft.setCursor (10, 215);
        data.tft.print (LEFT_ICON);
        data.tft.setCursor (160, 215);
        data.tft.print (RIGHT_ICON);
    }
    else if (digitalRead (func_3_pin)) {
        data.function = BRIGHTNESS_FUNCTION;
        Serial.println ("Function 3");
        data.tft.setCursor (10, 215);
        data.tft.print (DIMMER_SCREEN_ICON);
        data.tft.setCursor (160, 215);
        data.tft.print (BRIGHTER_SCREEN_ICON);
    }
    else if (digitalRead (func_4_pin)) {
        data.function = VOLUME_FUNCTION;
        Serial.println ("Function 4");
        data.tft.setCursor (10, 215);
        data.tft.print (LOWER_VOL_ICON);
        data.tft.setCursor (160, 215);
        data.tft.print (HIGHER_VOL_ICON);
    }
    data.tft.unloadFont ();

    data.tft.loadFont (file_main_latin, SD);
    data.tft.setCursor (10, 70);
    data.tft.print ("NOT CONNECTED");
    data.tft.unloadFont ();  

    // CHECK TO SEE IF THERE ARE ANY PREVIOUSLY CONNECTED DEVICES TO TRY TO RECONNECT
    preferences.begin ("Connections", true);
    bool prev_init = preferences.isKey ("prev");
    if (prev_init) {
        esp_bd_addr_t prev_addr;
        preferences.getBytes ("prev", prev_addr, ESP_BD_ADDR_LEN);
        Serial.printf ("Checking to see if %s connects\n", a2dp_sink.to_str (prev_addr));
        
        unsigned long start = millis ();
        while (millis () - start <= 5000) {
            if (a2dp_sink.connect_to (prev_addr)) {
                Serial.printf ("Reconnected to previously connected device: %s\n", a2dp_sink.to_str (prev_addr));
                a2dp_sink.set_volume (data.phone_volume);
                Print_Temp_Text (data, "CONNECTED", DEFAULT_TEMP_TEXT_TIME);
                sent_connected_message = true;
                break;
            }
        }
    }
    preferences.end (); 

    // SET UP TASKS FOR CONCURRENCY
    if (data.mutex != NULL) {
        // xTaskCreatePinnedToCore (Bt_Task_Handler, "BT handler", 20000, NULL, 1, &bt_task, 1);
        xTaskCreatePinnedToCore (In_Task_Handler, "Input handler", 10000, NULL, 1, &in_task, 0);
    }
}

void loop () {
    // Remove temporary text once the timer finishes
    if (data.temp_text_duration != 0 && millis () - data.temp_text_start > data.temp_text_duration) {
        Print_Song_Data (data);
        data.temp_text_duration = 0;
    }

    // Send a device conntected message once if a device is connected
    if (a2dp_sink.is_connected () && !sent_connected_message) {
        sent_connected_message = true;
        sent_disconnected_message = false;
        Serial.println ("Device connected");
        Print_Temp_Text (data, "CONNECTED", DEFAULT_TEMP_TEXT_TIME);
        a2dp_sink.set_volume (data.phone_volume);

        preferences.begin ("connections", false);
        preferences.putBytes ("prev", a2dp_sink.get_current_peer_address (), ESP_BD_ADDR_LEN);
        Serial.printf ("Put address to memory: %s\n", a2dp_sink.to_str (*(a2dp_sink.get_current_peer_address ())));
        preferences.end ();
    } else if (!a2dp_sink.is_connected () && !sent_disconnected_message) {
        sent_disconnected_message = true;
        sent_connected_message = false;
        Print_Perma_Text (data, "NOT CONNECTED");
        Serial.println ("Device disconnected");
    }
}

void In_Task_Handler (void* param) {
    while (true) {
        delay (1);
        // Read from the input button
        int pp_reading = digitalRead (pp_pin);
        int next_reading = digitalRead (next_pin);
        int prev_reading = digitalRead (prev_pin);
        int func_1_reading = digitalRead (func_1_pin);
        int func_2_reading = digitalRead (func_2_pin);
        int func_3_reading = digitalRead (func_3_pin);
        int func_4_reading = digitalRead (func_4_pin);

        // If switch is changed due to noise or pressing, reset debounce timer
        if (pp_reading != pp_last_state)            pp_last_db_time = millis();
        if (next_reading != next_last_state)        next_last_db_time = millis();
        if (prev_reading != prev_last_state)        prev_last_db_time = millis();
        if (func_1_reading != func_1_last_state)    func_1_last_db_time = millis ();
        if (func_2_reading != func_2_last_state)    func_2_last_db_time = millis ();
        if (func_3_reading != func_3_last_state)    func_3_last_db_time = millis ();
        if (func_4_reading != func_4_last_state)    func_4_last_db_time = millis ();

        // If debounce delay is reached, update the state of the button, and if high, send the message
        if ((millis() - pp_last_db_time) > pp_db_delay) {
            if (pp_reading != pp_state) {
                pp_state = pp_reading;
                
                if (pp_state == HIGH) {
                    is_active = !is_active;
                    if (is_active) {
                        Serial.println ("play");
                        a2dp_sink.play ();
                    } else {
                        Serial.println ("pause");
                        a2dp_sink.pause ();
                    }
                }
            }
        }

        if ((millis() - next_last_db_time) > next_db_delay) {
            if (next_reading != next_state) {
                next_state = next_reading;
                
                if (next_state == HIGH) {
                    switch (data.function) {
                        case PLAYBACK_FUNCTION: {
                            Serial.println ("Next");
                            a2dp_sink.next ();
                        } break;
                        
                        case STATION_FUNCTION: {
                            if (data.radio_station < HIGHEST_RADIO_FREQUENCY)   data.radio_station += 20;
                            Serial.printf ("Tuning into %d.%d\n", data.radio_station / 100, data.radio_station % 100);
                            radio.tuneFM (data.radio_station);
                        } break;

                        case BRIGHTNESS_FUNCTION: {
                            if (data.screen_brightness < MAX_SCREEN_BRIGHTNESS)     data.screen_brightness += 16;
                            Serial.printf ("Brightness up to %d\n", data.screen_brightness);
                            data.tft.writecommand (ILI9341_WRDISBV);
                            data.tft.writedata (data.screen_brightness);
                        } break;

                        case VOLUME_FUNCTION: {
                            data.phone_volume = a2dp_sink.get_volume ();
                            if (data.phone_volume < MAX_PHONE_VOLUME)       data.phone_volume += 8; 
                            Serial.printf ("Setting phone volume to %d\n", data.phone_volume);
                            a2dp_sink.set_volume (data.phone_volume);
                        } break;
                    }
                }
            }
        }

        if ((millis() - prev_last_db_time) > prev_db_delay) {
            if (prev_reading != prev_state) {
                prev_state = prev_reading;
                
                if (prev_state == HIGH) {
                    switch (data.function) {
                        case PLAYBACK_FUNCTION: {
                            Serial.println ("Previous");
                            a2dp_sink.previous ();
                        } break;
                        
                        case STATION_FUNCTION: {
                            if (data.radio_station > LOWEST_RADIO_FREQUENCY)   data.radio_station -= 20;
                            Serial.printf ("Tuning into %d.%d\n", data.radio_station / 100, data.radio_station % 100);
                            radio.tuneFM (data.radio_station);
                        } break;

                        case BRIGHTNESS_FUNCTION: {
                            if (data.screen_brightness > MIN_SCREEN_BRIGHTNESS)     data.screen_brightness -= 16;
                            Serial.printf ("Brightness down to %d\n", data.screen_brightness);
                            data.tft.writecommand (ILI9341_WRDISBV);
                            data.tft.writedata (data.screen_brightness);
                        } break;

                        case VOLUME_FUNCTION: {
                            data.phone_volume = a2dp_sink.get_volume ();
                            if (data.phone_volume > MIN_PHONE_VOLUME)       data.phone_volume -= 8; 
                            Serial.printf ("Setting phone volume to %d\n", data.phone_volume);
                            a2dp_sink.set_volume (data.phone_volume);
                        } break;
                    }
                }
            }
        }

        if ((millis () - func_1_last_db_time) > func_1_db_delay) {
            if (func_1_reading != func_1_state) {
                func_1_state = func_1_reading;

                if (func_1_state == HIGH) {
                    Serial.println ("Function 1");
                    data.function = PLAYBACK_FUNCTION;
                    if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                        int16_t x = data.tft.getCursorX ();
                        int16_t y = data.tft.getCursorY ();
                        data.tft.loadFont (file_functions_symbols, SD);
                        data.tft.fillRect (10, 215, 30, 30, TFT_BLACK);
                        data.tft.setCursor (10, 215);
                        data.tft.print (PREVIOUS_ICON);
                        data.tft.fillRect (160, 215, 30, 30, TFT_BLACK);
                        data.tft.setCursor (160, 215);
                        data.tft.print (NEXT_ICON);
                        data.tft.unloadFont ();
                        data.tft.setCursor (x, y);
                        xSemaphoreGive (data.mutex);
                    }
                }
            }
        }

        if ((millis () - func_2_last_db_time) > func_2_db_delay) {
            if (func_2_reading != func_2_state) {
                func_2_state = func_2_reading;

                if (func_2_state == HIGH) {
                    Serial.println ("Function 2");
                    data.function = STATION_FUNCTION;
                    if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                        int16_t x = data.tft.getCursorX ();
                        int16_t y = data.tft.getCursorY ();
                        data.tft.loadFont (file_functions_symbols, SD);
                        data.tft.fillRect (10, 215, 30, 30, TFT_BLACK);
                        data.tft.setCursor (10, 215);
                        data.tft.print (LEFT_ICON);
                        data.tft.fillRect (160, 215, 30, 30, TFT_BLACK);
                        data.tft.setCursor (160, 215);
                        data.tft.print (RIGHT_ICON);
                        data.tft.unloadFont ();
                        data.tft.setCursor (x, y);
                        xSemaphoreGive (data.mutex);
                    }
                }
            }
        }

        if ((millis () - func_3_last_db_time) > func_3_db_delay) {
            if (func_3_reading != func_3_state) {
                func_3_state = func_3_reading;

                if (func_3_state == HIGH) {
                    Serial.println ("Function 3");
                    data.function = BRIGHTNESS_FUNCTION;
                    if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                        int16_t x = data.tft.getCursorX ();
                        int16_t y = data.tft.getCursorY ();
                        data.tft.loadFont (file_functions_symbols, SD);
                        data.tft.fillRect (10, 215, 30, 30, TFT_BLACK);
                        data.tft.setCursor (10, 215);
                        data.tft.print (DIMMER_SCREEN_ICON);
                        data.tft.fillRect (160, 215, 30, 30, TFT_BLACK);
                        data.tft.setCursor (160, 215);
                        data.tft.print (BRIGHTER_SCREEN_ICON);
                        data.tft.unloadFont ();
                        data.tft.setCursor (x, y);
                        xSemaphoreGive (data.mutex);
                    }
                }
            }
        }

        if ((millis () - func_4_last_db_time) > func_4_db_delay) {
            if (func_4_reading != func_4_state) {
                func_4_state = func_4_reading;

                if (func_4_state == HIGH) {
                    Serial.println ("Function 4");
                    data.function = VOLUME_FUNCTION;
                    if (xSemaphoreTake (data.mutex, portMAX_DELAY) == pdTRUE) {
                        int16_t x = data.tft.getCursorX ();
                        int16_t y = data.tft.getCursorY ();
                        data.tft.loadFont (file_functions_symbols, SD);
                        data.tft.fillRect (10, 215, 30, 30, TFT_BLACK);
                        data.tft.setCursor (10, 215);
                        data.tft.print (LOWER_VOL_ICON);
                        data.tft.fillRect (160, 215, 30, 30, TFT_BLACK);
                        data.tft.setCursor (160, 215);
                        data.tft.print (HIGHER_VOL_ICON);
                        data.tft.unloadFont ();
                        data.tft.setCursor (x, y);
                        xSemaphoreGive (data.mutex);
                    }
                }
            }
        }
        
        // Update previous button state
        pp_last_state = pp_reading;
        next_last_state = next_reading;
        prev_last_state = prev_reading;
        func_1_last_state = func_1_reading;
        func_2_last_state = func_2_reading;
        func_3_last_state = func_3_reading;
        func_4_last_state = func_4_reading;
    }
}
