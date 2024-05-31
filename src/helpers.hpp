/**
 * helpers.hpp
 * brief   : some basic helper functions
 * created : 25.05.2024
 * creator : @tigmit
 * Licence : opensource
 */

#pragma once

bool outOfBounds(int val, int range) {
  return (val < (val - (range / 2)) || val > (val + ((range / 2))));
}