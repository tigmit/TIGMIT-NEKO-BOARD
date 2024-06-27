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
    newPosition = encoder.getCount() / 2;
    if (newPosition > 0) {
      kbd.write(KEY_MEDIA_VOLUME_UP);
      encoder.clearCount();

    } else if (newPosition < 0) {
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

  void printPosition() {
    newPosition = encoder.getCount() / 2;
    Serial.println(newPosition);
    if (digitalRead(encBTN) == LOW) {
      Serial.println("Button pressed");
    }
  }

private:
  ESP32Encoder encoder;
  long newPosition = 0;
  bool ecnBtnPressed = false;
};
