/**
 * batteryHandler.hpp
 * brief   : this class handles all the tasks required to handle the battery
 * created : 25.05.2024
 * creator : @tigmit Licence : opensource
 */
#pragma once

#include "Defines/hardwareDef.hpp"
#include "debugSettings.hpp"
#include "keyBoardHandler.hpp"
#include <Arduino.h>
#include <math.h>

class batteryHandler {
public:
  void init() {
    pinMode(BatterySense, INPUT);
    pinMode(BatteryVoltage, INPUT);
    updateBateryHandler();
  }

  bool isCharging() { return digitalRead(BatterySense); }

  void updateBateryHandler() {
    double average = 0;
    for (int i = 0; i < 20; i++) {
      average += analogRead(BatteryVoltage);
    }
    average /= 20;
    voltage = floorf(330 * average / 4095) / 100; // rounded to two decimals
    soc = 100.0 - ((voltage * (-1) + upperVoltage) / rate);
    alert = (soc < 20.0); // if soc below 20% please charge
    kbd.setBatteryLevel((uint8_t)soc);
  }

  float getSOC() {
    // returns the estimated state of charge (e.g. 79%)
    return soc;
  }

  float getVoltage() {
    // returns the estimated state of charge (e.g. 79%)
    return voltage;
  }

  bool getAlertStatus() {
    // resturns the charge me alert status
    return alert;
  }

private:
  float voltage = 0.0; // Variable to keep track of LiPo voltage
  int soc = 0;         // Variable to keep track of LiPo state-of-charge (SOC)
  bool alert = false;  // keep track of whether alert has been triggered

  // 50/50 voltage devider bounds
  float upperVoltage = 1.95; // translating to about 4.12V battry voltage
  float lowerVoltage = 1.53; // translating to about 3.30V battry voltage
  float rate = (upperVoltage - lowerVoltage) / 100.0;
};