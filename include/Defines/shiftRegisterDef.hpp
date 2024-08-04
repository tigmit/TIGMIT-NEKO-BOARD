/**
 * shiftRegisterDef.hpp
 * brief   : defines for shift registers
 * created : 29.05.2024
 * creator : @tigmit Licence : opensource
 */

#pragma once

static constexpr int SRCLK_latch{13}; // pin 12 on chip
static constexpr int RCLK_clock{18};  // pin 11 on shift chip
static constexpr int SER_data{23};    // pin 14 on chip (esp32 SPI pin)