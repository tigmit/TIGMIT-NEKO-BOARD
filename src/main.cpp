/**
 * main.cpp
 * brief   : main compilation unit containing loop and setup
 * created : 25.05.2024
 * creator : @tigmit Licence : opensource
 */

#include "../lib/BatteryHandler.hpp"
#include "../lib/Defines/hardwareDef.hpp"
#include "../lib/DisplayHandler.hpp"
#include "../lib/EncoderHandler.hpp"
#include "../lib/FSM.hpp"
#include "../lib/KeyBoardHandler.hpp"
#include "../lib/RgbHandler.hpp"
#include "../lib/ShiftRegisterHandler.hpp"
#include "../lib/debugSettings.hpp"
#include "../lib/layout.hpp"

#include <Arduino.h>

ShiftRegisterHandler srHandler;
KeyboardHandler kbdHandler(&srHandler);
BatteryHandler batHandler;
EncoderHandler encHandler;
RgbHandler rgbHandler;
DisplayHandler dspHandler(&batHandler, &kbdHandler);
FSM fsm(&batHandler, &kbdHandler, &rgbHandler, &encHandler, &dspHandler);

// setup Task handles
TaskHandle_t Loop0; // loop running on core 0
TaskHandle_t Loop1; // loop running on core 1 (default core)
void Loop1_(void *param);
void Loop0_(void *param);

void setup() {
  Serial.begin(115200);

  // ----------init display
  dspHandler.init();

  // ----------init keyboard matrix
  Serial.println("Starting BLE work!");
  kbdHandler.init();
  kbd.begin();

  // ----------init battery handler
  batHandler.init();

  // ----------init RGB handler
  rgbHandler.init();

  // ----------init Rotary Encoder
  encHandler.init();

  // creating loop on core 1 (default core)
  xTaskCreatePinnedToCore(Loop0_, "Loop0", 10000, NULL, 0, &Loop0, 0);
  // creating loop on core 0
  xTaskCreatePinnedToCore(Loop1_, "Loop1", 10000, NULL, 1, &Loop1, 1);
}

void Loop0_(void *param) {
  // setup section for loop0:

  //__________________RUN Loop0
  while (true) {
    fsm.mainFSM(); // calling statemachine
  }
}

void Loop1_(void *param) { // explicitly for scanning the btn matrix
  // setup section for loop1:
  //__________________RUN Loop1 (default core)
  while (true) {
#ifdef DEBUG_LVL_2
    unsigned long dt = micros();
#endif
    kbdHandler.updateKeyMatrix();
#ifdef DEBUG_LVL_2
    Serial.print(micros() - dt);
    Serial.println("micro Seconds for scan");
#endif
  }
}

// nothing to do here
void loop() {}
