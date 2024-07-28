/**
 * EncoderHandler.hpp
 * brief   : this class handles all the tasks required to read the rotary
 * encoder created : 25.05.2024 creator : @tigmit Licence : opensource
 */
#pragma once

#include "Defines/hardwareDef.hpp"
#include "FsmStates.hpp"
#include "KeyboardHandler.hpp"
#include "debugSettings.hpp"
#include <Arduino.h>
#include <ESP32Encoder.h> // https://github.com/madhephaestus/ESP32Encoder.git
#include <math.h>

class EncoderHandler {
public:
  EncoderHandler() = default;

  void init() {
    encoder.attachHalfQuad(encDT, encCLK);
    encoder.setCount(0);
    pinMode(encBTN, INPUT);
  }

  void updateVolume() { // perhaps legacy code
    VolPosition = encoder.getCount() / 2;
    if (VolPosition > 0) {
      kbd.write(KEY_MEDIA_VOLUME_UP);
      encoder.clearCount();

    } else if (VolPosition < 0) {
      kbd.write(KEY_MEDIA_VOLUME_DOWN);
      encoder.clearCount();
    }

    // check if the button was pressed
    if (digitalRead(encBTN) == LOW && !ecnBtnPressed) {
      ecnBtnPressed = true;
      kbd.write(KEY_MEDIA_MUTE);
    } else {
      if (digitalRead(encBTN) == HIGH && ecnBtnPressed) {
        ecnBtnPressed = false;
      }
    }
  }

  void UpdateMainStateSelect(int &modeSet) {

    // checking encoder val state
    modeEncPosition = encoder.getCount() / 2;
    if (modeEncPosition == 0) {
      // do nothing
    } else if (modeEncPosition > 0) {
      modeSet++;
      modeSet %= numMainStates;
      encoder.clearCount();

    } else if (modeEncPosition < 0) {
      modeSet--;
      modeSet = (modeSet < 0) ? numMainStates - 1 : modeSet % numMainStates;
      encoder.clearCount();
    }
  }

  void UpdateRgbStateSelect(int &modeSet) {

    // checking encoder val state
    modeEncPosition = encoder.getCount() / 2;
    if (modeEncPosition == 0) {
      // do nothing
    } else if (modeEncPosition > 0) {
      modeSet++;
      modeSet %= numRgbStates;
      encoder.clearCount();

    } else if (modeEncPosition < 0) {
      modeSet--;
      modeSet = (modeSet < 0) ? numRgbStates - 1 : modeSet % numRgbStates;
      encoder.clearCount();
    }
  }

  bool UpdateColorPickerSelect(uint8_t &modeSet) {

    // checking encoder val state
    modeEncPosition = encoder.getCount() / 2;
    if (modeEncPosition == 0) {
      // do nothing
    } else if (modeEncPosition > 0) {
      modeSet += colorPickerIncrement;
      modeSet %= 255;
      encoder.clearCount();
      return true;

    } else if (modeEncPosition < 0) {
      modeSet -= colorPickerIncrement;
      modeSet = (modeSet < 0) ? 0 : modeSet % 255;
      encoder.clearCount();
      return true;
    }
    return false;
  }

  bool EncButtonPressed() {
    // check if the button was pressed
    if (digitalRead(encBTN) == LOW && !ecnBtnPressed) {
      delay(encButtonDebounce);
      ecnBtnPressed = true;
      return true;
    } else if (digitalRead(encBTN) == HIGH && ecnBtnPressed) {
      ecnBtnPressed = false;
    }
    return false;
  }

private:
  ESP32Encoder encoder;

  long modeEncPosition = 0;
  int encButtonDebounce = 15; // mS
  uint8_t colorPickerIncrement = 5;

  long VolPosition = 0;
  u_int32_t colorPosition = 0;
  bool ecnBtnPressed = false;
};
