/**
 * ../lib/RgbHandler.hpp
 * brief   : this class handles all the tasks to controll the pixel LEDs.
 * created : 13.06.2024
 * creator : @tigmit Licence : opensource
 */

#pragma once

#include "../lib/EncoderHandler.hpp"
#include <FastLED.h>

#define NUM_LEDS 81
#define DATA_PIN 19

class RgbHandler {
public:
  RgbHandler() = default;

  void init() {
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(0xFF);
    // use this to limit the current draw from the lipo
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 800);

    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
    }
    FastLED.show();
  }

  void setConstColor(CRGB color) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = color;
    }
    FastLED.show();
  }

  void setBrightnes(u_int8_t val) {
    FastLED.setBrightness(val);
    FastLED.show();
  }

  void brightnessSlider() { // legacy but might wanna recycle for rgb settings
    FastLED.setBrightness(analogRead(SliderReadPin) * (255.0 / 4095.0));
    FastLED.show();
  }

  void startupSequence() {
    FastLED.setBrightness(currentBrightness);
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Purple;
      FastLED.show();
      delay(12);
    }
    setConstColor(currentRGBValue);
  }

  bool rgbIsOn() const { return rgbOn; }

  void turnRgbOff() {
    setConstColor(CRGB::Black);
    rgbOn = false;
  }
  void turnRgbOn() {
    setConstColor(currentRGBValue);
    rgbOn = true;
  }

  void pushCurrentRGBValues() {
    currentRGBValue[0] = rVal;
    currentRGBValue[1] = gVal;
    currentRGBValue[2] = bVal;
    setConstColor(currentRGBValue);
  }

  void pushcurrentBrightness() {
    FastLED.setBrightness(currentBrightness);
    FastLED.show();
  }

  uint8_t &getRval() { return rVal; }
  uint8_t &getGval() { return gVal; }
  uint8_t &getBval() { return bVal; }
  uint8_t &getCurrentBrightness() { return currentBrightness; }
  uint8_t getMaxRgbBrightness() const { return maxRgbBrightness; }

private:
  bool rgbOn = true;

  CRGB currentRGBValue = CRGB::Aqua;
  uint8_t currentBrightness = 15;
  const uint8_t maxRgbBrightness = 45;

  uint8_t rVal = currentRGBValue[0];
  uint8_t gVal = currentRGBValue[1];
  uint8_t bVal = currentRGBValue[2];

  // Define the array of leds
  CRGB leds[NUM_LEDS];
};
