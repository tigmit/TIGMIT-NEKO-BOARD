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
    // Initialize row INPUT pin and set default state

    for (int i = 0; i < numRows; i++) {
      pinMode(rows[i], INPUT);
    }
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

    //_______________________________________________________scan rwos
    for (int colIdx = 0; colIdx < numCols; colIdx++) {
      SPI.transfer16((uint16_t(1 << colIdx))); // set one row to VCC
      pSrHandler_->latch();
      delayMicroseconds(100); // needed for the shiftregister to set the outputs
      //_____________________________________________________scan cols
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

          // for bongo mode
          keyPressToggle = !keyPressToggle;
          // for DSPL toggle: // for testing only. modeSet will be controlled
          // via the encoder in the future
          if (rowIdx == 5 && colIdx == 10) { // this is the FN key.
            // advance display mode.
            modeSet++;
            modeSet %= numModes;
          }

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

  void updateVolumeSlider() {
    readVolume = analogRead(SliderReadPin);
    // if (outOfBounds(readVolume, sliderbounds)) {
    //  // TODO: write volume to BLE output
    //}
  }

  bool getKeyPressToggle() const { return keyPressToggle; }

  // Dspl mode vareables
  int modeSet = 0;  // which mode is currently active?
  int numModes = 2; // how many modes are available
private:
  bool keyPressToggle = false;

  // NOTE: once i introduce Layers this is used to toggle these layers
  int layerIdx = 0;

  int currentVolume_ = 0;
  int readVolume = 0;
  int sliderbounds = 10;

  // dependecies
  ShiftRegisterHandler *pSrHandler_;
};
