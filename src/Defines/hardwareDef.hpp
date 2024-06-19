/**
 * hardwareDef.hpp
 * brief   : defining the pins for the key matrix
 * created : 25.05.2024
 * creator : @tigmit
 * Licence : opensource
 */
#pragma once

// Define on and off values
#define OFF 0
#define ON 1

// Pin Definitions
#define row0 34 // TODO: gotta set these accoarding to schematics
#define row1 35
#define row2 32
#define row3 33
#define row4 25
#define row5 26

// volume Slider
#define SliderReadPin 12 // TODO: gotta set this accoarding to schematics

// battery is charging indicator
#define BatterySense 14

// reading the battery voltage through 33k / 100k voltage devider
#define BatteryVoltage 27

// Matrix setup
#define numRows 6
#define numCols 16
#define numLayers 1

// i2C adress for display ESP32
#define SlaveEspAdress 9

// pins for rotary Encoder
#define encCLK 17 // CLK ENCODER
#define encDT 16  // DT ENCODER
#define encBTN 39 // encoder button