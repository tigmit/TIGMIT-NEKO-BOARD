/**
 * ../lib/debugSettings.hpp
 * brif    : different debug levels for development and debugging
 * created : 26.05.2024
 * creator : @tigmit
 * Licence : opensource
 */

#pragma once

#undef DEBUG_LVL_1
#undef DEBUG_LVL_2
#undef DEBUG_LVL_3
#undef FSM_PRINTS_ENABLED
#undef EEPROM_LOG
#undef NEKO_SHELL

#undef DEVMODE

#ifdef DEVMODE
#define NEKO_SHELL
#define FSM_PRINTS_ENABLED
#define DISABLE_BLE_OUTPUT
#define EEPROM_LOG
#define DEBUG_LVL_1
#endif