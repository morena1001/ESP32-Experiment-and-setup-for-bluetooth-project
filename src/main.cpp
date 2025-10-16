#include <Arduino.h>
#include <BleKeyboard.h>

BleKeyboard bleKeyboard ("Media Control");

const int pp_pin = 25; // GPIO pin for the pause/play button
const int next_pin = 26; // GPIO pin for the next button
const int prev_pin = 27; // GPIO pin for the previous button

bool sent_connected_message = false;

void setup () {
  Serial.begin (115200);

  pinMode (pp_pin, INPUT);
  pinMode (next_pin, INPUT);
  pinMode (prev_pin, INPUT);

  Serial.println ("Starting BLE work!");
  bleKeyboard.begin ();
}

void loop () {
  if(bleKeyboard.isConnected () && !sent_connected_message) {
    sent_connected_message = true;

    Serial.println ("Device connected");
    bleKeyboard.print ("Device connected");

    // Serial.println ("Sending Play/Pause media key...");
    // bleKeyboard.write (KEY_MEDIA_PLAY_PAUSE);
  } else if (!bleKeyboard.isConnected ()) {
    sent_connected_message = false;
  }

  Serial.println (digitalRead (pp_pin));
}