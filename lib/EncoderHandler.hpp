/**
 * ../lib/EncoderHandler.hpp
 * brief   : this class handles all the tasks required to read the rotary
 * encoder created : 25.05.2024 creator : @tigmit Licence : opensource
 */
#pragma once

#include "../lib/Defines/hardwareDef.hpp"
#include "../lib/FsmStates.hpp"
#include "../lib/KeyBoardHandler.hpp"
#include "../lib/debugSettings.hpp"

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

  void UpdateMainStateSelect(int &modeSet) {

    // checking encoder val state
    encoderPosition = encoder.getCount() / 2;
    if (encoderPosition == 0) {
      // do nothing
    } else if (encoderPosition > 0) {
      modeSet++;
      modeSet %= numMainStates;
      encoder.clearCount();

    } else if (encoderPosition < 0) {
      modeSet--;
      modeSet = (modeSet < 0) ? numMainStates - 1 : modeSet % numMainStates;
      encoder.clearCount();
    }
  }

  void UpdateRgbStateSelect(int &modeSet) {

    // checking encoder val state
    encoderPosition = encoder.getCount() / 2;
    if (encoderPosition == 0) {
      // do nothing
    } else if (encoderPosition > 0) {
      modeSet++;
      modeSet %= numRgbStates;
      encoder.clearCount();

    } else if (encoderPosition < 0) {
      modeSet--;
      modeSet = (modeSet < 0) ? numRgbStates - 1 : modeSet % numRgbStates;
      encoder.clearCount();
    }
  }

  bool UpdateColorPickerSelect(uint8_t &modeSet) {

    // checking encoder val state
    encoderPosition = encoder.getCount() / 2;
    if (encoderPosition == 0) {
      // do nothing
    } else if (encoderPosition > 0) {
      modeSet += colorPickerIncrement;
      modeSet %= 255;
      encoder.clearCount();
      return true;

    } else if (encoderPosition < 0) {
      modeSet -= colorPickerIncrement;
      modeSet = (modeSet < 0) ? 0 : modeSet % 255;
      encoder.clearCount();
      return true;
    }
    return false;
  }

  bool updateRgbBrightnessSelect(uint8_t &brightness,
                                 uint8_t maxBrightness = 0xFF) {
    // checking encoder val state
    encoderPosition = encoder.getCount() / 2;
    if (encoderPosition == 0) {
      // do nothing
    } else if (encoderPosition > 0) {
      brightness += brightnessIncrement;
      brightness = (brightness >= maxBrightness) ? maxBrightness : brightness;
      encoder.clearCount();
      return true;

    } else if (encoderPosition < 0) {
      brightness -= brightnessIncrement;
      brightness = (brightness <= 0) ? 0 : brightness;
      encoder.clearCount();
      return true;
    }
    return false;
  }

  bool EncButtonPressed() {
    // check if the button was pressed
    int reading = digitalRead(encBTN);

    if (reading != lastButtonState) {
      lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
      if (reading != buttonState) {
        buttonState = reading;
        if (buttonState == LOW) {
          return true;
        }
      }
    }
    lastButtonState = reading;
    return false;
  }

private:
  ESP32Encoder encoder;

  long encoderPosition = 0;
  uint8_t colorPickerIncrement = 5;
  uint8_t brightnessIncrement = 1;

  int buttonState;
  int lastButtonState = HIGH;

  unsigned long lastDebounceTime = 0;
  unsigned long debounceDelay = 50;
};
