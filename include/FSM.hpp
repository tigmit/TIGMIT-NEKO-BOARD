/**
 * FSM.hpp
 * brief   : this is the statemachine that powers all the display states
 * created : 27.07.2024
 * creator : @tigmit Licence : opensource
 */

#pragma once

// this will be all we need for the new statemachine
#include "FsmStates.hpp"
#include <Arduino.h>

class FSM {
public:
  // start statemachine with an initial state
  FSM(State *initialState = pStartUp) : currentState(initialState) {
    currentState->enter();
  }
  void update() {
    delay(10); // save some processing power
    currentState->run();
    State *next = currentState->nextState();
    if (next != currentState) {
      currentState->exit();
      currentState = next;
      currentState->enter();
    }
  }

private:
  State *currentState;
};
