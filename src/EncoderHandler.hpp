/**
 * EncoderHandler.hpp
 * brief   : this class handles all the tasks required to read the rotary
 * encoder created : 25.05.2024 creator : @tigmit Licence : opensource
 */
#pragma once

#include "Defines/hardwareDef.hpp"
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

  void updateVolume() {
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

  void printPosition() { // legacy code
    VolPosition = encoder.getCount() / 2;
    Serial.println(VolPosition);
    if (digitalRead(encBTN) == LOW) {
      Serial.println("Button pressed");
    }
  }

  int UpdateModeSelect() {

    // check if the button was pressed
    if (digitalRead(encBTN) == LOW && !ecnBtnPressed) {
      ecnBtnPressed = true;
      modeSelected = modeSet;
    } else if (digitalRead(encBTN) == HIGH && ecnBtnPressed) {
      ecnBtnPressed = false;
    }

    // checking encoder val state
    modeEncPosition = encoder.getCount() / 2;
    if (modeEncPosition == 0) {
      // do nothing
    } else if (modeEncPosition > 0) {
      modeSet++;
      modeSet %= numModes;
      encoder.clearCount();

    } else if (modeEncPosition < 0) {
      modeSet--;
      modeSet = (modeSet < 0) ? numModes - 1 : modeSet % numModes;
      encoder.clearCount();
    }
    return modeSet;
  }

  u_int32_t updateColorPosition() {
    VolPosition = (encoder.getCount() / 2) % 0xFFFFFF;
    return VolPosition;
  }

  int getModeSelected() { return modeSelected; }
  int getModeSet() { return modeSet; }

private:
  ESP32Encoder encoder;

  // Dspl mode vareables
  long modeEncPosition = 0;
  int modeSet = 0;      // which mode is currently active?
  int modeSelected = 0; // Mode selected via pushbutton. default 0 == mainScreen
  const int numModes = 5; // how many modes are available

  long VolPosition = 0;
  u_int32_t colorPosition = 0;
  bool ecnBtnPressed = false;
};
