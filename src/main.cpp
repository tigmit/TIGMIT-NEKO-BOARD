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
  Serial.println("waiting for connection....");
  kbdHandler.waitForConnection();
  Serial.println("---> BT Connection established :3");
}

void loop() { kbdHandler.updateKeyMatrix(); }
