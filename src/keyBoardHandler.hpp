/**
 * keyboardHandler.hpp
 * brief   : this class handles all the tasks of the keymatrix such as scanning
 *           inputs and initializing the array
 * created : 25.05.2024
 * creator : @tigmit Licence : opensource
 */
#pragma once
#include "Defines\hardwareDef.hpp"
#include "debugSettings.hpp"
#include "helpers.hpp"
#include "layout.hpp"
#include "shiftRegisterHandler.hpp"
#include <BleKeyboard.h>

BleKeyboard kbd("tigmit_Board", "tigmit", 100);
class keyboardHandler {
public:
  keyboardHandler(shiftRegisterHandler *srh)
      : pSrHandler_(srh){

        }; // default constructor

  /**
   * @brief initialize all the row pins
   * @return void
   * @param none
   */
  void init() {
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
   * @brief waiting for BT connection
   * @return void
   * @param none
   */
  void waitForConnection() {
    Serial.println("waiting for connection....");
    while (!kbd.isConnected()) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(BleConnectionBlinkDelay);
      digitalWrite(LED_BUILTIN, LOW);
      delay(BleConnectionBlinkDelay);
    }
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("---> BT Connection established :3");
  }

  /**
   * @brief scanning each key and listening for keypresses
   * @return void
   * @param none
   */
  void updateKeyMatrix() {
    if (!kbd.isConnected())
      return;

    //_______________________________________________________scan rwos
    for (int colIdx = 0; colIdx < numCols; colIdx++) {
      SPI.transfer16((uint16_t(1 << colIdx))); // set one row to VCC
      digitalWrite(SRCLK_latch, HIGH);         // latch col to scan
      digitalWrite(SRCLK_latch, LOW);
      delayMicroseconds(100); // needed
      //_____________________________________________________scan cols
      for (int rowIdx = 0; rowIdx < numRows; rowIdx++) {
        if (digitalRead(rows[rowIdx]) && !pressed[layerIdx][rowIdx][colIdx]) {
// pressed must be synchron to the scanned row.. might need to play
// with the shift register here key press detected (MSBFIRST or
// LSBFIRST)
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
          // bongo end

        } else if (!digitalRead(rows[rowIdx]) &&
                   pressed[layerIdx][rowIdx][colIdx]) {
          // pressed must be synchron to the scanned row.. might need to play
          // with the shift register here key press detected
          // key was released

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
        // delay(scanDelay); // probably dont wanna use this
      }
    }
  }

  void updateVolumeSlider() {
    readVolume = analogRead(SliderReadPin);
    // if (outOfBounds(readVolume, sliderbounds)) {
    //  // TODO: write volume to BLE output
    //}
  }

  /**
   * @brief setting the delay between scanning EACH key
   * @return void
   * @param none
   */
  void setScanDelay(int milliSeconds) { scanDelay = milliSeconds; }

  bool getKeyPressToggle() { return keyPressToggle; }

private:
  // TODO:: once we introduce Layers this needs to be itterated
  bool keyPressToggle = false;

  int layerIdx = 0;
  int scanDelay = 1000;
  int BleConnectionBlinkDelay = 500;

  int currentVolume_ = 0;
  int readVolume = 0;
  int sliderbounds = 10;
  shiftRegisterHandler *pSrHandler_;
};
