#pragma once
#include <Arduino.h>

enum MainStates {
  startUp = 100,
  mainMenu = 0,
  bongoCat = 2,
  rgbMenu = 3,
  mainUndef = 255
};
enum RgbMenuStates {
  rgbSelectScreen = 6,
  colorPickerRED = 0,
  colorPickerGREEN = 1,
  colorPickerBLUE = 2,
  onOff = 3,
  brignessSetting = 4,
  acceptSettings = 5,
  rgbUndef = 255
};

constexpr int numMainStates{5};
constexpr int numRgbStates{6};
