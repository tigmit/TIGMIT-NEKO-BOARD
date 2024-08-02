/**
 * ../lib/ShiftRegisterHandler.hpp
 * brief   : handles shift register tasks
 * created : 29.05.2024
 * creator : @tigmit Licence : opensource
 */
#pragma once
#include "../lib/Defines/shiftRegisterDef.hpp"

#include <Arduino.h>
#include <SPI.h>

class ShiftRegisterHandler {
public:
  ShiftRegisterHandler() = default;

  void init() {
    pinMode(SRCLK_latch, OUTPUT);
    // since we are using SPI we dont have to set this to output.
    pinMode(RCLK_clock, OUTPUT);
    pinMode(SER_data, OUTPUT);

    SPI.setBitOrder(MSBFIRST); // can be flipped... gotta experiment a bit.  |
                               // MSB - 0,0,0,0,0,0,0,1 - LSB
                               // shifting the MSB first. pin Col7 - Col1 = 0V
                               // and pin Col0 will be VCC = 3,3V
    SPI.setDataMode(SPI_MODE0);
    // SPI.setClockDivider(SPI_CLOCK_DIV128);
    SPI.begin();
    digitalWrite(SRCLK_latch, LOW);

    SPI.transfer16(0); // since we are using 16 bit shiftregister

    digitalWrite(SRCLK_latch, HIGH);
    digitalWrite(SRCLK_latch, LOW);
  }

  void latch() {
    digitalWrite(SRCLK_latch, HIGH);
    digitalWrite(SRCLK_latch, LOW);
  }

private:
};
