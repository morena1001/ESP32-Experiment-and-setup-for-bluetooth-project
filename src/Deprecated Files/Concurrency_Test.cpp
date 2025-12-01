// #include <Arduino.h>

// TaskHandle_t task1;
// TaskHandle_t task2;

// void Task1_Code (void* param);
// void Task2_Code (void* param);

// void setup () {
//     Serial.begin (115200);

//     xTaskCreatePinnedToCore (Task1_Code, "Task 1", 10000, NULL, 1, &task1, 0);
//     xTaskCreatePinnedToCore (Task2_Code, "Task 2", 10000, NULL, 1, &task2, 1);
// }

// void loop () {}

// void Task1_Code (void* param) {
//     Serial.printf ("Task 1 running on core: %d\n", xPortGetCoreID ());
//     for (;;) {
//         Serial.println ("Task 1: HIGH");
//         delay (1000);
//         Serial.println ("Task 1: LOW");
//         delay (1000);
//     }
// }

// void Task2_Code (void* param) {
//     Serial.printf ("Task 2 running on core: %d\n", xPortGetCoreID ());
//     for (;;) {
//         Serial.println ("Task 2: HIGH");
//         delay (700);
//         Serial.println ("Task 2: LOW");
//         delay (700);
//     }
// }
