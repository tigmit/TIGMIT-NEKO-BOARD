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

int chartoint(char s[]) {
  int i, n;
  n = 0;
  for (i = 0; isdigit(s[i]); ++i) {
    n = 10 * n + (s[i] - '0');
  }
  return n;
}