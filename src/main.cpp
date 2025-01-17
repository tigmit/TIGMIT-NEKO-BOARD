/**
 * main.cpp
 * brief   : main compilation unit containing loop and setup
 * created : 25.05.2024
 * creator : @tigmit Licence : opensource
 */

#include "FSM.hpp"
#include "Model.hpp"
#include <Arduino.h>

#ifdef NEKO_SHELL
#include "NekoShell.hpp"
#endif

// dual core param
TaskHandle_t Loop0;       // loop running on core 0
TaskHandle_t Loop1;       // loop running on core 1 (default core)
void Loop0_(void *param); // forward declaration Loop0
void Loop1_(void *param); // forward declaration Loop1

FSM fsm; // start with default state -> pStartUp

#ifdef NEKO_SHELL
NekoShell nekoShell;
#endif

void setup() {
  Serial.begin(BAUD_RATE);

  // ----------init display
  dspHandler.init();
  // ----------init keyboard matrix
  kbdHandler.init();
  // ----------init slider Handler
  sliderHandler.init();
  // ----------init battery handler
  batHandler.init();
  // ----------init RGB handler
  rgbHandler.init();
  // ----------init Rotary Encoder
  encHandler.init();
#ifdef NEKO_SHELL
  // ----------init NEKO SHELL
  nekoShell.init();
#endif

  // creating loop on core 1 (default core)
  xTaskCreatePinnedToCore(Loop0_, "Loop0", 10000, NULL, 0, &Loop0, 0);
  // creating loop on core 0 with prio 1
  xTaskCreatePinnedToCore(Loop1_, "Loop1", 10000, NULL, 1, &Loop1, 1);
}

void Loop0_(void *param) {
  // setup section for loop0:

  //__________________RUN Loop0
  while (true) {
    fsm.update(); // running statemachine
  }
}

void Loop1_(void *param) { // explicitly for scanning the key matrix
  // setup section for loop1:
  //__________________RUN Loop1 (default core)
  while (true) {
    kbdHandler.updateKeyMatrix();
  }
}

// nothing to do here
void loop() {}
