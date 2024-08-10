/**
 * TimerHandler.hpp
 * brief   : this class handles all timer related tasks
 * created : 07.08.2024
 * creator : @tigmit Licence : opensource
 */
#pragma once
#include "Arduino.h"
#include "Model.hpp"

class TimerHandler {
public:
  TimerHandler() = default;

  void init() {
    // INIT RGB TIMER
    // use timer 0 (esp has 4 HW timers)
    // prescaler at 80 which gives us 80Mhz / 80 = 1MHz = 1/1000000Seconds
    // which allows for time def in mili Seconds
    // true for upCounter
    pRgbSleepTimer = timerBegin(0, 80, true);
    timerAttachInterrupt(pRgbSleepTimer, &timerRGBOff, true); // attatch ISR

    // INIT OTHER TIMERS
    //....... tbd
  }

  void RGBSleepTimerStartNew(uint64_t timeParam = 0) {
    rgbHandler.setRgbSleepFlag(false); // tell rgb Handler to turn RGB on

    uint64_t time = (timeParam) ? timeParam : rgbSleepTime;
    timerAlarmWrite(pRgbSleepTimer, time, false); // no auto reload
    timerRestart(pRgbSleepTimer);                 // set timer to 0
    timerAlarmEnable(pRgbSleepTimer);             // start the timer
  }

  void RGBSleepTimerRefreshRunning() {
    // sets counter back to 0 (USE BEFORE interrupt has fired)
    timerRestart(pRgbSleepTimer);
  }

  bool RGBSleepTimerisRunning() { return timerStarted(pRgbSleepTimer); }

private:
  static void timerRGBOff() { rgbHandler.setRgbSleepFlag(true); } // ISR

  const uint64_t rgbSleepTime{10000000}; // default
  hw_timer_t *pRgbSleepTimer = nullptr;
} timerHandler;