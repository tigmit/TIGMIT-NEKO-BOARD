/**
 * FSM.hpp
 * brief   : this is the statemachine that powers all the display states
 * created : 27.07.2024
 * creator : @tigmit Licence : opensource
 */

#pragma once

#include "BatteryHandler.hpp"
#include "EncoderHandler.hpp"
#include "FsmStates.hpp"
#include "KeyboardHandler.hpp"
#include "RgbHandler.hpp"
#include "debugSettings.hpp"
#include "displayHandler.hpp"
#include "helpers.hpp"
#include "immages/allImmages.hpp"
#include <Arduino.h>

struct FSM {
public:
  FSM(BatteryHandler *pBatHandler, KeyboardHandler *pKbdHandler,
      RgbHandler *pRgbHandler, EncoderHandler *encHandler, DisplayHandler *pDsp)
      : pBatHandler_(pBatHandler), pKbdHandler_(pKbdHandler),
        pRgbHandler_(pRgbHandler), pEncHandler_(encHandler), pDsp_(pDsp) {
    ;
  }

  /***************************************************************************
   *                                                                         *
   *                        MAIN FSM                                         *
   *                                                                         *
   **************************************************************************/
  void mainFSM() {
    // calling State enter functions
    switch (currentMainState) {
    case startUp:
      startUpEnter();
      break;
    case mainMenu:
      mainMenuEnter();
      break;
    case bongoCat:
      BongoCatStateEnter();
      break;
    case rgbMenu:
      break;

    // default fallback state
    default:
      mainMenuEnter();
      break;
    }

    // calling State run functions
    while (currentMainState == nextMainState) {
      switch (currentMainState) {
      case startUp:
        startUpRun();
        break;
      case mainMenu:
        mainMenuRun();
        break;
      case bongoCat:
        BongoCatStateRun();
        break;
      case rgbMenu:
        rgbFSM(); // calling RGB substatemachine
        break;

      // default fallback state
      default:
        mainMenuRun();
        break;
      }
    }

    // enter next state on change
    mainFsmTransition();
  }

  /***************************************************************************
   *                                                                         *
   *                        RGB FSM                                          *
   *                                                                         *
   **************************************************************************/
  void rgbFSM() {
    // calling State enter functions
    switch (currentRgbState) {
    case rgbSelectScreen:
      rgbSelectScreenEnter();
      break;
    case colorPickerRED:
      colorPickerRedEnter();
      break;
    case colorPickerGREEN:
      colorPickerGreenEnter();
      break;
    case colorPickerBLUE:
      colorPickerBlueEnter();
      break;
    case onOff:
      onOffEnter();
      break;
    case brignessSetting:
      brignessSettingEnter();
      break;
    case acceptSettings:
      // for now do nothing. in the future perhaps save settings to flash
      acceptSettingsEnter();
      break;

    // default state
    default:
      rgbSelectScreenEnter();
      break;
    }

    // calling State run functions
    while (currentRgbState == nextRgbState) {
      switch (currentRgbState) {
      case rgbSelectScreen:
        rgbSelectScreenRun();
        break;
      case colorPickerRED:
        colorPickerRedRun();
        break;
      case colorPickerGREEN:
        colorPickerGreenRun();
        break;
      case colorPickerBLUE:
        colorPickerBlueRun();
        break;
      case onOff:
        onOffRun();
        break;
      case brignessSetting:
        brignessSettingRun();
        break;
      case acceptSettings:
        acceptSettingsRun();
        break;

      // default state
      default:
        rgbSelectScreenRun();
        break;
      }
    }

    // enter next state on change
    rgbFsmTransition();
  }

private:
  // set next state for both FSM
  void setMainState(MainStates state) { nextMainState = state; }
  void setRgbState(RgbMenuStates rgbState) { nextRgbState = rgbState; }

  void mainFsmTransition() { currentMainState = nextMainState; }
  void rgbFsmTransition() { currentRgbState = nextRgbState; }

  MainStates nextMainState = startUp;
  MainStates currentMainState = startUp;

  RgbMenuStates nextRgbState = rgbSelectScreen;
  RgbMenuStates currentRgbState = rgbSelectScreen;

  // refferences
  BatteryHandler *pBatHandler_ = nullptr;
  KeyboardHandler *pKbdHandler_ = nullptr;
  RgbHandler *pRgbHandler_ = nullptr;
  EncoderHandler *pEncHandler_ = nullptr;
  DisplayHandler *pDsp_ = nullptr;

  /***************************************************************************
   *                                                                         *
   *                             MAIN STATES                                 *
   *                                                                         *
   **************************************************************************/
  /**
   * Start / boot State this is the initial default state
   */
  void startUpEnter() {
#ifdef FSM_PRINTS_ENABLED
    Serial.println("State Transition -> State : Start");
#endif
  }

  void startUpRun() {
    pDsp_->printPNG(startscreen, sizeof(startscreen), 0, 0);
    pRgbHandler_->setBrightnes(15); // full brightness = 0xFF
    pRgbHandler_->startupSequence();

    // set next state
    setMainState(mainMenu);
  }

  /**
   *******************************************************Main Screen State
   */

  void mainMenuEnter() {
    pDsp_->resetDspdrawflags();
    pDsp_->resetMainScreenWheel();
#ifdef FSM_PRINTS_ENABLED
    Serial.println("State Transition -> State : MainScreen");
#endif
  }

  void mainMenuRun() {
    pDsp_->drawMainScreenWallpaper();

    // update the SOC and update the charge icon.
    // dont do this so often please
    // TODO: utalize esp32 Timer?
    pBatHandler_->updateBateryHandler();

    // update rotary encoder select
    pEncHandler_->UpdateMainStateSelect(pDsp_->getMainItemSelect());
    pDsp_->drawMainScreenSelect(); // selection wheel

    // update icons
    pDsp_->updateChargeIcon(90, 6);
    pDsp_->updateBLEIcon(10, 64, kbd.isConnected());
    pDsp_->updateCapslockIcon(170, 64, pKbdHandler_->getCapslockStatus());

    if (pEncHandler_->EncButtonPressed()) {
      setMainState((MainStates)pDsp_->getMainItemSelect());
    }
  }

  /**
   *******************************************************Bongo Cat State
   *  */
  void BongoCatStateEnter() {
    pDsp_->resetBongoMode();
#ifdef FSM_PRINTS_ENABLED
    Serial.println("State Transition -> State : BongoCat");
#endif
  }

  void BongoCatStateRun() {
    pDsp_->bongoMode();
    if (pEncHandler_->EncButtonPressed()) {
      setMainState(mainMenu);
    }
  }

  /***************************************************************************
   *                                                                         *
   *                             RGB FSM STATES                              *
   *                                                                         *
   **************************************************************************/

  /**
   *******************************************************rgbSelectScreen State
   */
  void rgbSelectScreenEnter() {
    pDsp_->resetRgbScreenFlags();
#ifdef FSM_PRINTS_ENABLED
    Serial.println("State Transition -> State : rgbSelectScreen");
#endif
  }

  void rgbSelectScreenRun() {

    pDsp_->drawRgbScreenWallpaper();

    // current mode should be reset to 0?
    pEncHandler_->UpdateRgbStateSelect(pDsp_->getRgbItemSelect());
    pDsp_->drawRgbScreenSelect();

    if (pEncHandler_->EncButtonPressed()) {
      setRgbState((RgbMenuStates)pDsp_->getRgbItemSelect());
    }
  }

  /**
   *******************************************************RED Color Picker State
   */
  void colorPickerRedEnter() {
#ifdef FSM_PRINTS_ENABLED
    Serial.println("State Transition -> State : ColorPicker RED");
#endif
    pDsp_->drawRgbConfigWheel(pRgbHandler_->getRval(), TFT_RED);
  }

  void colorPickerRedRun() {
    if (pEncHandler_->UpdateColorPickerSelect(pRgbHandler_->getRval())) {
      pDsp_->drawRgbConfigWheel(pRgbHandler_->getRval(), TFT_RED);
      pRgbHandler_->pushCurrentRGBValues();
    }

    if (pEncHandler_->EncButtonPressed()) {
      setRgbState(rgbSelectScreen);
    }
  }

  /**
   *******************************************************GREEN Color Picker
   *State
   */
  void colorPickerGreenEnter() {
#ifdef FSM_PRINTS_ENABLED
    Serial.println("State Transition -> State : ColorPicker GREEN");
#endif
    pDsp_->drawRgbConfigWheel(pRgbHandler_->getGval(), TFT_GREEN);
  }

  void colorPickerGreenRun() {
    if (pEncHandler_->UpdateColorPickerSelect(pRgbHandler_->getGval())) {
      pDsp_->drawRgbConfigWheel(pRgbHandler_->getGval(), TFT_GREEN);
      pRgbHandler_->pushCurrentRGBValues();
    }

    if (pEncHandler_->EncButtonPressed()) {
      setRgbState(rgbSelectScreen);
    }
  }

  /**
   *******************************************************BLUE Color Picker
   *State
   */
  void colorPickerBlueEnter() {
#ifdef FSM_PRINTS_ENABLED
    Serial.println("State Transition -> State : ColorPicker BLUE");
#endif
    pDsp_->drawRgbConfigWheel(pRgbHandler_->getBval(), TFT_BLUE);
  }

  void colorPickerBlueRun() {
    if (pEncHandler_->UpdateColorPickerSelect(pRgbHandler_->getBval())) {
      pDsp_->drawRgbConfigWheel(pRgbHandler_->getBval(), TFT_BLUE);
      pRgbHandler_->pushCurrentRGBValues();
    }

    if (pEncHandler_->EncButtonPressed()) {
      setRgbState(rgbSelectScreen);
    }
  }

  /**
   *******************************************************onOff State
   */
  void onOffEnter() {
#ifdef FSM_PRINTS_ENABLED
    Serial.print("State Transition -> State : onOff");
#endif
  }

  void onOffRun() {
    if (pRgbHandler_->rgbIsOn()) {
      pRgbHandler_->turnRgbOff();
    } else {
      pRgbHandler_->turnRgbOn();
    }
    setRgbState(rgbSelectScreen);
  }

  /**
   *******************************************************brignessSetting State
   */
  void brignessSettingEnter() {
#ifdef FSM_PRINTS_ENABLED
    Serial.println("State Transition -> State : brignessSetting");
#endif

    pDsp_->drawRgbConfigWheel(pRgbHandler_->getCurrentBrightness(), TFT_CYAN);
  }

  void brignessSettingRun() {
    if (pEncHandler_->updateRgbBrightnessSelect(
            pRgbHandler_->getCurrentBrightness(),
            pRgbHandler_->getMaxRgbBrightness())) {
      pDsp_->drawRgbConfigWheel(pRgbHandler_->getCurrentBrightness(), TFT_CYAN);
      pRgbHandler_->pushcurrentBrightness();
    }

    if (pEncHandler_->EncButtonPressed()) {
      setRgbState(rgbSelectScreen);
    }
  }

  /**
   *******************************************************accept settings State
   */
  void acceptSettingsEnter() {
#ifdef FSM_PRINTS_ENABLED
    Serial.println("State Transition -> State : acceptSettings");
#endif
    // nothing else to do here yet in the furture. write config to flash
  }

  void acceptSettingsRun() {
    setMainState(mainMenu);
    setRgbState(rgbSelectScreen);
  }
};
