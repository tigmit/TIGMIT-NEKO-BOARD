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
  void initRows() {
    // Initialize row output pin and set default state
    for (int i = 0; i < numRows; i++) {
      pinMode(rows[i], OUTPUT);
      digitalWrite(rows[i], LOW);
    }
  }

  /**
   * @brief waiting for BT connection
   * @return void
   * @param none
   */
  void waitForConnection() {
    Serial.println("waiting for connection....");
    while (!kbd.isConnected()) {
      // do nothing and wait for a connection
    }
    Serial.println("---> BT Connection established :3");
  }

  /**
   * @brief scanning each key and listening for keypresses
   * @return void
   * @param none
   */
  void updateKeyMatrix() {
    //_______________________________________________________scan rwos
    for (int colIdx = 0; colIdx < numCols; colIdx++) {
      SPI.transfer16(1 << colIdx);    // set one row to VCC
      digitalWrite(SRCLK_latch, LOW); // latch col to scan
      //_____________________________________________________scan cols
      for (int rowIdx = 0; rowIdx < numRows; rowIdx++) {
        if (digitalRead(rows[rowIdx]) && !pressed[layerIdx][rowIdx][colIdx]) {
          // pressed must be synchron to the scanned row.. might need to play
          // with the shift register here key press detected (MSBFIRST or
          // LSBFIRST)
          kbd.press(layout1[layerIdx][rowIdx][colIdx]);
          pressed[layerIdx][rowIdx][colIdx] = ON;

#ifdef DEBUG_LVL_1
          Serial.print("key pressed: rowIDX : ");
          Serial.print(rowIdx);
          Serial.print(" | colIDX : ");
          Serial.print(colIdx);
          Serial.println(" ");
#endif

        } else if (!digitalRead(rows[rowIdx]) &&
                   pressed[layerIdx][rowIdx][colIdx]) {
          // pressed must be synchron to the scanned row.. might need to play
          // with the shift register here key press detected
          // key was released
          kbd.release(layout1[layerIdx][rowIdx][colIdx]);
          pressed[layerIdx][rowIdx][colIdx] = OFF;
        }
        delay(scanDelay);
      }

      digitalWrite(SRCLK_latch, LOW);
    }
  }

  /**
   * @brief setting the delay between scanning EACH key
   * @return void
   * @param none
   */
  void setScanDelay(int milliSeconds) { scanDelay = milliSeconds; }

private:
  // TODO:: once we introduce Layers this needs to be itterated
  int layerIdx = 0;
  int scanDelay = 1;

  shiftRegisterHandler *pSrHandler_;
};
