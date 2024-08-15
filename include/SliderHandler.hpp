/**
 * SliderHandler.hpp
 * brief   : handles shift register tasks
 * created : 16.08.2024
 * creator : @tigmit Licence : opensource
 */
#pragma once
#include "Defines/hardwareDef.hpp"

#include <Arduino.h>

class SliderHandler {
public:
  SliderHandler() = default;

  void init() { pinMode(SliderReadPin, INPUT); }

  int updateSlider(int range = 255) {
    return ((analogRead(SliderReadPin) * range) / 4095);
  }

private:
};
