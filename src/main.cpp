/**
 * main.cpp
 * brief   : main compilation unit containing loop and setup
 * created : 25.05.2024
 * creator : @tigmit Licence : opensource
 */

#include "Defines/hardwareDef.hpp"
#include "keyboardHandler.hpp"
#include "layout.hpp"
#include "shiftRegisterHandler.hpp"
#include <Arduino.h>
#include <BleKeyboard.h>

#include "displayHandler.hpp" // for testing display

shiftRegisterHandler srHandler;
keyboardHandler kbdHandler(&srHandler);
displayHandler dspHandler;

// setup Task handles
TaskHandle_t Loop0; // loop running on core 0
TaskHandle_t Loop1; // loop running on core 1 (default core)
void Loop1_(void *param);
void Loop0_(void *param);

void setup() {
  // setCpuFrequencyMhz(160); //not tested but i am assuming that the scanning
  // will be to slow.. i want to reach at least 1/1ms = 1kHz matrix scan
  // frequency. if the -O2 setting doesnt work i can try setting the cpu freq
  // higher.. at the cost of battery life tho.... damn
  Serial.begin(115200);
  dspHandler.init();
  dspHandler.startScreen();
  Serial.println("Starting BLE work!");

  kbdHandler.init();
  // srHandler.init(); for final HW version

  kbd.begin();
  kbdHandler.setScanDelay(0);

  // creating loop on core 1 (default core)
  xTaskCreatePinnedToCore(Loop0_, "Loop0", 10000, NULL, 0, &Loop0, 0);
  // creating loop on core 0
  xTaskCreatePinnedToCore(Loop1_, "Loop1", 10000, NULL, 1, &Loop1, 1);
}

void Loop0_(void *param) {
  // setup section for loop0:
  dspHandler.waitForConnectionScreen();
  kbdHandler.waitForConnection();
  //__________________RUN Loop0
  while (true) {
    dspHandler.bongoPlay();
  }
}

void Loop1_(void *param) {
  // setup section for loop1:
  //__________________RUN Loop1 (default core)
  while (true) {

    unsigned long dt = micros();
    kbdHandler.updateKeyMatrix();
    Serial.print(micros() - dt);
    Serial.println("micro Seconds for scan");
  }
}

// nothing to do here
void loop() {}
