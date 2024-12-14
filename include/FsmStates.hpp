/**
 * FsmStates.hpp
 * brief   : this file contains all State defines
 * created : 27.07.2024
 * creator : @tigmit Licence : opensource
 */
#pragma once
#include "Model.hpp"
#include "debugSettings.hpp"
#include "helpers.hpp"
#include "immages/allImmages.hpp"
#include <Arduino.h>

//***************************************************************************
//*                                                                         *
//*                           BASE STATE Interface                          *
//*                                                                         *
//**************************************************************************/
class State {
public:
  // virtual
  virtual ~State() = default;

  // interface methods
  virtual void enter() = 0; // Called when entering the state
  virtual void run() = 0;   // Called repeatedly while in the state
  virtual void exit() = 0;  // Called when exiting the state

  // base methods

  // we must reset this -> nextState_
  // otherwise the state will run only once upon entering it a second time
  State *nextState() {
    if (nextState_ != this) {
      State *nextStateBuff = nextState_;
      nextState_ = this;
      return nextStateBuff;
    }
    return this;
  }

protected:
  // set the next state
  void setNextState(State *setState) { nextState_ = setState; }

  // state buffer
  State *nextState_ = this;
};

/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//                              STATE HANDLES                              //
//                               (pre define)                              //
/////////////////////////////////////////////////////////////////////////////

State *pStartUp;
State *pIdle;     // -0
State *pMainMenu; // -1
State *pBongoCat; // -2
State *pRgbMenu;  // -3
// State *RgbSelectScreen;

State *pColorPickerRED;   // -0
State *pColorPickerGREEN; // -1
State *pcolorPickerBLUE;  // -2
State *pRgbOnOff;         // -3
State *pBrignessSetting;  // -4
State *pAcceptSettings;   // -5

static constexpr u_int8_t numMainStates{5};
static constexpr u_int8_t numRgbStates{6};

State **mainFsmSelectPointers[numMainStates] = {&pIdle, &pIdle, &pBongoCat,
                                                &pRgbMenu, &pIdle};

State **rgbFsmSelectPointers[numRgbStates] = {
    &pColorPickerRED, &pColorPickerGREEN, &pcolorPickerBLUE,
    &pRgbOnOff,       &pBrignessSetting,  &pAcceptSettings};

// State *pStateExample; // template for adding new states

//***************************************************************************
//*                                                                         *
//*                            STATE: StartUp                               *
//*                                                                         *
//**************************************************************************/

class StartUp : public State {
public:
  StartUp(State *&stateHandle) { stateHandle = this; }
  //===========> enter
  void enter() override {
#ifdef FSM_PRINTS_ENABLED
    Serial.println(" -> StartUp");
#endif
  }
  //===========> run
  void run() override {
    dspHandler.printPNG(startscreen, sizeof(startscreen), 0, 0);
    rgbHandler.startupSequence();
    setNextState(pIdle); // kick off transition
  }
  //===========> exit
  void exit() override {
#ifdef FSM_PRINTS_ENABLED
    Serial.print(" StartUp ->");
#endif
  }
} startUp(pStartUp);

//***************************************************************************
//*                                                                         *
//*                               STATE: Idle                               *
//*                                                                         *
//**************************************************************************/
class Idle : public State {
public:
  Idle(State *&stateHandle) { stateHandle = this; }
  //===========> enter
  void enter() override {
#ifdef FSM_PRINTS_ENABLED
    Serial.println(" -> Idle");
#endif
    dspHandler.resetIdleScreenFlags();
  }
  //===========> run
  void run() override {
    dspHandler.drawIdleScreenWallpaper();

    batHandler.updateBateryHandler();
    dspHandler.updateChargeIcon(90, 6);

    dspHandler.updateBLEIcon(10, 64, kbd.isConnected());
    dspHandler.updateCapslockIcon(170, 64, kbdHandler.getCapslockStatus());

    encHandler.updateVolume();

    if (encHandler.EncButtonPressed()) {
      // set next state mainMenu
      setNextState(pMainMenu); // kick off transition
    }
  }
  //===========> exit
  void exit() override {
#ifdef FSM_PRINTS_ENABLED
    Serial.print(" Idle ->");
#endif
  }
} idle(pIdle);

//***************************************************************************
//*                                                                         *
//*                            STATE: MainMenu                              *
//*                                                                         *
//**************************************************************************/

class MainMenu : public State {
public:
  MainMenu(State *&stateHandle) { stateHandle = this; }
  //===========> enter
  void enter() override {
#ifdef FSM_PRINTS_ENABLED
    Serial.println(" -> MainMenu");
#endif
    dspHandler.resetDspdrawflags();
    dspHandler.resetMainScreenWheel();
  }
  //===========> run
  void run() override {
    dspHandler.drawMainScreenWallpaper();

    // update the SOC and update the charge icon.
    // dont do this so often please
    // TODO: utalize esp32 Timer?
    batHandler.updateBateryHandler();

    // update rotary encoder select
    encHandler.UpdateMainStateSelect(dspHandler.getMainItemSelect(),
                                     numMainStates);
    dspHandler.drawMainScreenSelect(); // selection wheel

    // update icons
    dspHandler.updateChargeIcon(90, 6);
    dspHandler.updateBLEIcon(10, 64, kbd.isConnected());
    dspHandler.updateCapslockIcon(170, 64, kbdHandler.getCapslockStatus());

    if (encHandler.EncButtonPressed()) {
      setNextState(*mainFsmSelectPointers[dspHandler.getMainItemSelect()]);
    }
  }
  //===========> exit
  void exit() override {
#ifdef FSM_PRINTS_ENABLED
    Serial.print(" MainMenu ->");
#endif
  }
} mainMenu(pMainMenu); // instanciate and pass stateHandle

//***************************************************************************
//*                                                                         *
//*                            STATE: BongoCat                              *
//*                                                                         *
//**************************************************************************/

class BongoCat : public State {
public:
  BongoCat(State *&stateHandle) { stateHandle = this; }
  //===========> enter
  void enter() override {
#ifdef FSM_PRINTS_ENABLED
    Serial.println(" -> BongoCat");
#endif
    dspHandler.resetBongoMode();
  }
  //===========> run
  void run() override {
    dspHandler.bongoMode();
    if (encHandler.EncButtonPressed()) {
      setNextState(pIdle); // kick off transition
    }
  }
  //===========> exit
  void exit() override {
#ifdef FSM_PRINTS_ENABLED
    Serial.print(" BongoCat ->");
#endif
  }
} bongoCat(pBongoCat); // instanciate and pass stateHandle

//***************************************************************************
//*                                                                         *
//*                            STATE: RgbMenu                               *
//*                                                                         *
//**************************************************************************/

class RgbMenu : public State {
public:
  RgbMenu(State *&stateHandle) { stateHandle = this; }
  //===========> enter
  void enter() override {
#ifdef FSM_PRINTS_ENABLED
    Serial.println(" -> RgbMenu");
#endif
    dspHandler.resetRgbScreenFlags();
  }
  //===========> run
  void run() override {

    dspHandler.drawRgbScreenWallpaper();

    // current mode should be reset to 0?
    encHandler.UpdateRgbStateSelect(dspHandler.getRgbItemSelect(),
                                    numRgbStates);
    dspHandler.drawRgbScreenSelect();

    if (encHandler.EncButtonPressed()) {
      setNextState(*rgbFsmSelectPointers[dspHandler.getRgbItemSelect()]);
    }
  }
  //===========> exit
  void exit() override {
#ifdef FSM_PRINTS_ENABLED
    Serial.print(" RgbMenu ->");
#endif
  }
} rgbMenu(pRgbMenu); // instanciate and pass stateHandle

//***************************************************************************
//*                                                                         *
//*                        STATE: ColorPickerRED                            *
//*                                                                         *
//**************************************************************************/

class ColorPickerRED : public State {
public:
  ColorPickerRED(State *&stateHandle) { stateHandle = this; }
  //===========> enter
  void enter() override {
#ifdef FSM_PRINTS_ENABLED
    Serial.println(" -> ColorPickerRED");
#endif
    dspHandler.drawRgbConfigWheel(rgbHandler.getRval(), TFT_RED);
  }
  //===========> run
  void run() override {
    if (encHandler.UpdateColorPickerSelect(rgbHandler.getRval())) {
      dspHandler.drawRgbConfigWheel(rgbHandler.getRval(), TFT_RED);
      rgbHandler.pushCurrentRGBValues();
    }

    if (encHandler.EncButtonPressed()) {
      setNextState(pRgbMenu); // kick off transition
    }
  }
  //===========> exit
  void exit() override {
#ifdef FSM_PRINTS_ENABLED
    Serial.print(" ColorPickerRED ->");
#endif
  }
} colorPickerRED(pColorPickerRED); // instanciate and pass stateHandle

//***************************************************************************
//*                                                                         *
//*                        STATE: ColorPickerGREEN                          *
//*                                                                         *
//**************************************************************************/

class ColorPickerGREEN : public State {
public:
  ColorPickerGREEN(State *&stateHandle) { stateHandle = this; }
  //===========> enter
  void enter() override {
#ifdef FSM_PRINTS_ENABLED
    Serial.println(" -> ColorPickerGREEN");
#endif
    dspHandler.drawRgbConfigWheel(rgbHandler.getGval(), TFT_GREEN);
  }
  //===========> run
  void run() override {
    if (encHandler.UpdateColorPickerSelect(rgbHandler.getGval())) {
      dspHandler.drawRgbConfigWheel(rgbHandler.getGval(), TFT_GREEN);
      rgbHandler.pushCurrentRGBValues();
    }

    if (encHandler.EncButtonPressed()) {
      setNextState(pRgbMenu); // kick off transition
    }
  }
  //===========> exit
  void exit() override {
#ifdef FSM_PRINTS_ENABLED
    Serial.print(" ColorPickerGREEN ->");
#endif
  }
} colorPickerGREEN(pColorPickerGREEN); // instanciate and pass stateHandle

//***************************************************************************
//*                                                                         *
//*                        STATE: ColorPickerBLUE                           *
//*                                                                         *
//**************************************************************************/

class ColorPickerBLUE : public State {
public:
  ColorPickerBLUE(State *&stateHandle) { stateHandle = this; }
  //===========> enter
  void enter() override {
#ifdef FSM_PRINTS_ENABLED
    Serial.println(" -> ColorPickerBLUE");
#endif
    dspHandler.drawRgbConfigWheel(rgbHandler.getBval(), TFT_BLUE);
  }
  //===========> run
  void run() override {
    if (encHandler.UpdateColorPickerSelect(rgbHandler.getBval())) {
      dspHandler.drawRgbConfigWheel(rgbHandler.getBval(), TFT_BLUE);
      rgbHandler.pushCurrentRGBValues();
    }

    if (encHandler.EncButtonPressed()) {
      setNextState(pRgbMenu); // kick off transition
    }
  }
  //===========> exit
  void exit() override {
#ifdef FSM_PRINTS_ENABLED
    Serial.print(" ColorPickerBLUE ->");
#endif
  }
} colorPickerBLUE(pcolorPickerBLUE); // instanciate and pass stateHandle

//***************************************************************************
//*                                                                         *
//*                          STATE: RgbOnOff                                *
//*                                                                         *
//**************************************************************************/

class RgbOnOff : public State {
public:
  RgbOnOff(State *&stateHandle) { stateHandle = this; }
  //===========> enter
  void enter() override {
#ifdef FSM_PRINTS_ENABLED
    Serial.println(" -> RgbOnOff");
#endif
  }
  //===========> run
  void run() override {
    if (rgbHandler.rgbIsOn()) {
      rgbHandler.turnRgbOff();
    } else {
      rgbHandler.turnRgbOn();
    }
    setNextState(pRgbMenu); // kick off transition
  }
  //===========> exit
  void exit() override {
#ifdef FSM_PRINTS_ENABLED
    Serial.print(" RgbOnOff ->");
#endif
  }
} rgbOnOff(pRgbOnOff); // instanciate and pass stateHandle

//***************************************************************************
//*                                                                         *
//*                        STATE: BrignessSetting                           *
//*                                                                         *
//**************************************************************************/

class BrignessSetting : public State {
public:
  BrignessSetting(State *&stateHandle) { stateHandle = this; }
  //===========> enter
  void enter() override {
#ifdef FSM_PRINTS_ENABLED
    Serial.println(" -> BrignessSetting");
#endif
    dspHandler.drawRgbConfigWheel(rgbHandler.getCurrentBrightness(), TFT_CYAN);
  }
  //===========> run
  void run() override {
    if (encHandler.updateRgbBrightnessSelect(
            rgbHandler.getCurrentBrightness(),
            rgbHandler.getMaxRgbBrightness())) {
      dspHandler.drawRgbConfigWheel(rgbHandler.getCurrentBrightness(),
                                    TFT_CYAN);
      rgbHandler.pushcurrentBrightness();
    }

    if (encHandler.EncButtonPressed()) {
      setNextState(pRgbMenu); // kick off transition
    }
  }
  //===========> exit
  void exit() override {
#ifdef FSM_PRINTS_ENABLED
    Serial.print(" BrignessSetting ->");
#endif
  }
} brignessSetting(pBrignessSetting); // instanciate and pass stateHandle

//***************************************************************************
//*                                                                         *
//*                        STATE: AcceptSettings                            *
//*                                                                         *
//**************************************************************************/

class AcceptSettings : public State {
public:
  AcceptSettings(State *&stateHandle) { stateHandle = this; }
  //===========> enter
  void enter() override {
#ifdef FSM_PRINTS_ENABLED
    Serial.println(" -> AcceptSettings");
#endif
    dspHandler.resetRgbAcceptSelect();
  }
  //===========> run
  void run() override {
    encHandler.UpdateRgbAcceptSelect(dspHandler.getRgbAcceptSelect());
    dspHandler.drawRgbAcceptPrompt();
    if (encHandler.EncButtonPressed()) {
      if (dspHandler.getRgbAcceptSelect() == 0) {
        // accepted
        rgbHandler.getCurrentConfig().storeConfigToEEPROM();
        setNextState(pIdle); // kick off transition
      } else {
#ifdef FSM_PRINTS_ENABLED
        Serial.println("  Discard. On reset the old config will be loaded");
#endif
        // discard config
        setNextState(pIdle); // kick off transition
      }
    }
  }
  //===========> exit
  void exit() override {
#ifdef FSM_PRINTS_ENABLED
    Serial.print(" AcceptSettings ->");
#endif
  }
} acceptSettings(pAcceptSettings); // instanciate and pass stateHandle

//***************************************************************************
//*                                                                         *
//*                            STATE: TEMPLATE                              *
//*                                                                         *
//**************************************************************************/
/*
class StateExample : public State {
public:
  StateExample(State *&stateHandle) { stateHandle = this; }
  //===========> enter
  void enter() override {
#ifdef FSM_PRINTS_ENABLED
  Serial.println(" -> StateExample");
#endif
  }
  //===========> run
  void run() override {
    Serial.println("Running StateExample");
    delay(1000); // do stuff
    if (true)    // set next state on some condition
      setNextState(pNextExampleState); // kick off transition
  }
  //===========> exit
  void exit() override {
#ifdef FSM_PRINTS_ENABLED
  Serial.print(" StateExample ->");
#endif
  }
} stateExample(pStateExample); //instanciate and pass stateHandle
*/