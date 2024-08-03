/**
 * ../lib/helpers.hpp
 * brief   : some basic helper functions
 * created : 25.05.2024
 * creator : @tigmit
 * Licence : opensource
 */

#pragma once

bool outOfBounds(int val, int oldVal, int range) {
  return (val < (oldVal - range) || val > (oldVal + range));
}