/**
 * main.cpp
 * brief   : main compilation unit containing loop and setup
 * created : 25.05.2024
 * creator : @tigmit Licence : opensource
 */
#pragma once

#include "BatteryHandler.hpp"
#include "DisplayHandler.hpp"
#include "EncoderHandler.hpp"
#include "KeyBoardHandler.hpp"
#include "RgbHandler.hpp"
#include "ShiftRegisterHandler.hpp"

ShiftRegisterHandler srHandler;
KeyboardHandler kbdHandler(&srHandler);
BatteryHandler batHandler;
EncoderHandler encHandler;
RgbHandler rgbHandler;
DisplayHandler dspHandler(&batHandler, &kbdHandler);
