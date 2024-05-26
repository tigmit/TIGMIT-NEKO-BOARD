/**
 * keyboardHandler.hpp
 * brief   : this class handles all the tasks of the keymatrix such as scanning
 *           inputs and initializing the array
 * created : 25.05.2024
 * creator : @tigmit Licence : opensource
 */
#pragma once
#include "debugSettings.hpp"
#include "hardwareDef.hpp"
#include "layout.hpp"
#include <BleKeyboard.h>

BleKeyboard kbd("tigmit_Board", "tigmit", 100);
class keyboardHandler {
public:
  keyboardHandler() = default; // default constructor

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
   * @brief initialize all the collum pins
   * @return void
   * @param none
   */
  void initCols() {
    for (int i = 0; i < numCols; i++) {
      pinMode(cols[i], INPUT_PULLDOWN);
    }
  }

  /**
   * @brief waiting for BT connection
   * @return void
   * @param none
   */
  void waitForConnection() {
    while (!kbd.isConnected()) {
      // do nothing and wait for a connection
    }
  }

  /**
   * @brief scanning each key and listening for keypresses
   * @return void
   * @param none
   */
  void updateKeyMatrix() {
    //_______________________________________________________scan rwos
    for (int rowIdx = 0; rowIdx < numRows; rowIdx++) {
      digitalWrite(rows[rowIdx], HIGH); // activate row to scan
      //_____________________________________________________scan cols
      for (int colIdx = 0; colIdx < numCols; colIdx++) {
        if (digitalRead(cols[colIdx]) && !pressed[layerIdx][rowIdx][colIdx]) {
          // key press detected
          kbd.press(layout1[layerIdx][rowIdx][colIdx]);
          pressed[layerIdx][rowIdx][colIdx] = ON;

#ifdef DEBUG_LVL_1
          Serial.print("key pressed: rowIDX : ");
          Serial.print(rowIdx);
          Serial.print(" | colIDX : ");
          Serial.print(colIdx);
          Serial.println(" ");
#endif

        } else if (!digitalRead(cols[colIdx]) &&
                   pressed[layerIdx][rowIdx][colIdx]) {
          // key was released
          kbd.release(layout1[layerIdx][rowIdx][colIdx]);
          pressed[layerIdx][rowIdx][colIdx] = OFF;
        }
        delay(scanDelay);
      }

      digitalWrite(rows[rowIdx], LOW); // deactivate current row
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
};
