/**
 * hardwareDef.hpp
 * brief   : defining the pins for the key matrix
 * created : 25.05.2024
 * creator : @tigmit
 * Licence : opensource
 */
#pragma once
#include <Arduino.h>

// baud rate
static constexpr unsigned long BAUD_RATE{115200};

// Define on and off values
static constexpr uint8_t OFF{0};
static constexpr uint8_t ON{1};

// Pin Definitions
static constexpr int row0{34}; // TODO: gotta set these accoarding to schematics
static constexpr int row1{35};
static constexpr int row2{32};
static constexpr int row3{33};
static constexpr int row4{25};
static constexpr int row5{26};

// volume Slider
static constexpr int SliderReadPin{12};

// battery is charging indicator
static constexpr int BatterySense{14};

// reading the battery voltage through 33k / 100k voltage devider
static constexpr int BatteryVoltage{27};

// Matrix setup
static constexpr int numRows{6};
static constexpr int numCols{15};
static constexpr int numLayers{1};

// i2C adress for display ESP32
static constexpr int SlaveEspAdress{9};

// pins for rotary Encoder
static constexpr int encCLK{17}; // CLK ENCODER
static constexpr int encDT{16};  // DT ENCODER
static constexpr int encBTN{39}; // encoder button
