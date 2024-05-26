/**
 * main.cpp
 * brief   : main compilation unit containing loop and setup
 * created : 25.05.2024
 * creator : @tigmit Licence : opensource
 */

#include "keyboardHandler.hpp"
#include "layout.hpp"
#include <Arduino.h>
#include <BleKeyboard.h>

keyboardHandler kbdHandler;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  kbdHandler.initRows();
  kbdHandler.initCols();

  kbd.begin();
  kbdHandler.setScanDelay(1);
  kbdHandler.waitForConnection();
}

void loop() { kbdHandler.updateKeyMatrix(); }
