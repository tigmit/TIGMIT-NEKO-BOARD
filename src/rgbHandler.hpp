/**
 * RgbHandler.hpp
 * brief   : this class handles all the tasks to controll the pixel LEDs.
 * created : 13.06.2024
 * creator : @tigmit Licence : opensource
 */

#pragma once

#include "EncoderHandler.hpp"
#include <FastLED.h>

#define NUM_LEDS 81
#define DATA_PIN 19

class RgbHandler {
public:
  RgbHandler(EncoderHandler *pEncHandler) : pEncHandler_(pEncHandler) { ; }

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

  void brightnessSlider() {
    FastLED.setBrightness(analogRead(SliderReadPin) * (255.0 / 4095.0));
    FastLED.show();
  }

  void randomRainbowMode() {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = random(0xFFFFFF);
    }
    FastLED.show();
    delay(100); // obviously this mode is bullshit XD
  }

  void startupSequence() {
    FastLED.setBrightness(defaultBrightnes);
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
    currentRGBValue[0] = (u_int8_t)rVal;
    currentRGBValue[1] = (u_int8_t)gVal;
    currentRGBValue[2] = (u_int8_t)bVal;
    setConstColor(currentRGBValue);
  }

  uint8_t &getRval() { return rVal; }
  uint8_t &getGval() { return gVal; }
  uint8_t &getBval() { return bVal; }

private:
  bool rgbOn = true;

  CRGB currentRGBValue = CRGB::Aqua;

  uint8_t rVal = currentRGBValue[0];
  uint8_t gVal = currentRGBValue[1];
  uint8_t bVal = currentRGBValue[2];

  // Define the array of leds
  CRGB leds[NUM_LEDS];
  u_int8_t defaultBrightnes = 15;
  EncoderHandler *pEncHandler_ = nullptr;
};

// NOTE BY ME TIGMIT
// i tested this script using an arduino pro micro... and the library works for
// the esp32 i will need a step up converter to get 5V VCC and also a signal
// level converter to step up the 3.3V level signals coming from the esp
// controller... hmmm lots of stuff but we can do it heheh i used THESE :
// https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf
// LEDs to test.