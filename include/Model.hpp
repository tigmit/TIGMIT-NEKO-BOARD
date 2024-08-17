/**
 * Model.hpp
 * brief   : make shift model to share all handlers across the project
 * created : 25.07.2024
 * creator : @tigmit Licence : opensource
 */
#pragma once

#include "BatteryHandler.hpp"
#include "DisplayHandler.hpp"
#include "EncoderHandler.hpp"
#include "KeyBoardHandler.hpp"
#include "RgbHandler.hpp"
#include "ShiftRegisterHandler.hpp"
#include "SliderHandler.hpp"

ShiftRegisterHandler srHandler;
KeyboardHandler kbdHandler(&srHandler);
BatteryHandler batHandler;
EncoderHandler encHandler;
RgbHandler rgbHandler;
SliderHandler sliderHandler;
DisplayHandler dspHandler(&batHandler, &kbdHandler);
