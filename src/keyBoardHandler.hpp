/**
 * KeyboardHandler.hpp
 * brief   : this class handles all the tasks of the keymatrix such as scanning
 *           inputs and initializing the array
 * created : 25.05.2024
 * creator : @tigmit Licence : opensource
 */
#pragma once
#include "Defines\hardwareDef.hpp"
#include "ShiftRegisterHandler.hpp"
#include "debugSettings.hpp"
#include "helpers.hpp"
#include "layout.hpp"
#include <BleKeyboard.h>

BleKeyboard kbd("tigmit_Board", "tigmit", 100);
class KeyboardHandler {
public:
  KeyboardHandler(ShiftRegisterHandler *srh) : pSrHandler_(srh){};

  /**
   * @brief initialize all the row pins
   * @return void
   * @param none
   */
  void init() {
    // make sure dependecies arent NULL!
    assert(pSrHandler_);

    // Initialize I/Os and set default state
    for (int i = 0; i < numRows; i++) {
      pinMode(rows[i], INPUT);
    }
    pSrHandler_->init();

    // setup Volume read pin:
    pinMode(SliderReadPin, INPUT);
    currentVolume_ = analogRead(SliderReadPin);

    // setup BLE connection indicator LED
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
  }

  /**
   * @brief scanning each key and listening for keypresses
   * @return void
   * @param none
   */
  void updateKeyMatrix() {
    // if we are not connected we don't do anything here
    if (!kbd.isConnected())
      return;

    //_______________________________________________________scan colls
    for (int colIdx = 0; colIdx < numCols; colIdx++) {
      SPI.transfer16((uint16_t(1 << colIdx))); // set one Coll to VCC
      pSrHandler_->latch();
      delayMicroseconds(100); // needed for the shiftregister to set the outputs
      //_____________________________________________________scan Rows
      for (int rowIdx = 0; rowIdx < numRows; rowIdx++) {
        if (digitalRead(rows[rowIdx]) && !pressed[layerIdx][rowIdx][colIdx]) {
#ifndef DISABLE_BLE_OUTPUT
          kbd.press(layout1[layerIdx][rowIdx][colIdx]);
#endif
          pressed[layerIdx][rowIdx][colIdx] = ON;

#ifdef DEBUG_LVL_1
          Serial.print("key pressed : rowIDX : ");
          Serial.print(rowIdx);
          Serial.print(" | colIDX : ");
          Serial.print(colIdx);
          Serial.print(" | pressed : ");
          Serial.print(pressed[layerIdx][rowIdx][colIdx]);
          Serial.println(" ");
#endif

          // caps lock.
          if (layout1[layerIdx][rowIdx][colIdx] == KEY_CAPS_LOCK) {
            capslockStatus = !capslockStatus;
          }

          // for bongo mode
          keyPressToggle = !keyPressToggle;

        } else if (!digitalRead(rows[rowIdx]) &&
                   pressed[layerIdx][rowIdx][colIdx]) {

#ifndef DISABLE_BLE_OUTPUT
          kbd.release(layout1[layerIdx][rowIdx][colIdx]);
#endif
          pressed[layerIdx][rowIdx][colIdx] = OFF;
#ifdef DEBUG_LVL_1
          Serial.print("key RELEASED: rowIDX : ");
          Serial.print(rowIdx);
          Serial.print(" | colIDX : ");
          Serial.print(colIdx);
          Serial.print(" | pressed : ");
          Serial.print(pressed[layerIdx][rowIdx][colIdx]);
          Serial.println(" ");
#endif
        }
      }
    }
  }

  bool getCapslockStatus() { return capslockStatus; }

  bool getKeyPressToggle() const { return keyPressToggle; }

private:
  bool keyPressToggle = false;

  bool capslockStatus = false;

  // NOTE: once i introduce Layers this is used to toggle these layers
  int layerIdx = 0;

  int currentVolume_ = 0;
  int readVolume = 0;
  int sliderbounds = 10;

  // dependecies
  ShiftRegisterHandler *pSrHandler_;
};