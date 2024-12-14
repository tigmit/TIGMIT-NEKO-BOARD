/**
 * layout.hpp
 * brief   : defining the layout for the keymatrix
 * created : 25.05.2024
 * creator : @tigmit
 * Licence : opensource
 */

#pragma once
#include "Defines/hardwareDef.hpp"

#include <BleKeyboard.h>

short rows[numRows]{row0, row1, row2, row3, row4, row5};

constexpr int NC = 0;
constexpr int KEY_FN_1 = 1;
constexpr int KEY_FN_2 = 2;

int layout1[numLayers][numRows][numCols]{
    {{       KEY_ESC,           NC,           NC, KEY_F1,  KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6,           KEY_F7,   KEY_F8,   KEY_F9,          KEY_F9,    KEY_DELETE,               NC},
     {           '`',          '1',          '2',    '3',     '4',    '5',    '6',    '7',    '8',              '9',      '0',      '-',             '=', KEY_BACKSPACE,          KEY_F11},
     {       KEY_TAB,          'q',          'w',    'e',     'r',    't',    'y',    'u',    'i',              'o',      'p',      '[',             ']',          '\\',          KEY_F12},
     { KEY_CAPS_LOCK,          'a',          's',    'd',     'f',    'g',    'h',    'j',    'k',              'l',      ';',     '\'',  NC,                KEY_RETURN,       KEY_DELETE},
     {KEY_LEFT_SHIFT,           NC,          'z',    'x',     'c',    'v',    'b',    'n',    'm',              ',',      '.',      '/', KEY_RIGHT_SHIFT,  KEY_UP_ARROW,               NC},
     { KEY_LEFT_CTRL, KEY_LEFT_GUI, KEY_LEFT_ALT,     NC,      NC,     NC,    ' ',     NC,     NC,    KEY_RIGHT_ALT, KEY_FN_1, KEY_FN_2,  KEY_LEFT_ARROW, KEY_DOWN_ARROW, KEY_RIGHT_ARROW}
    }
};

constexpr uint8_t rgbNC{80}; // out of array

uint8_t rgbLayout[numRows][numCols]{
    { 0,  rgbNC,  rgbNC,     1,     2,     3,   4,     5,     6,   7,   8,   9,     10,  11,     rgbNC},
    { 12,    13,     14,    15,    16,    17,  18,    19,    20,  21,  22,  23,     24,  25,        26},
    { 27,    28,     29,    30,    31,    32,  33,    34,    35,  36,  37,  38,     39,  40,        41},
    { 42,    43,     44,    45,    46,    47,  48,    49,    50,  51,  52,  53,  rgbNC,  54,        55},
    { 56, rgbNC,     57,    58,    59,    60,  61,    62,    63,  64,  65,  66,     67,  68,     rgbNC},
    { 69,    70,     71, rgbNC, rgbNC, rgbNC,  72, rgbNC, rgbNC,  73,  74,  75,     76,  77,        78}

};

short pressed[numLayers][numRows][numCols] = {OFF};

/*
// LAYOUT BLANK : the layout is based on a qwerty TKL sized board. all values
vontaining NC are not connected with a switch in the physical key-Matrix int
layout1[numLayers][numRows][numCols]{{
        {' ',     NC,     NC,    ' ',    ' ',    ' ',    ' ',    ' ',    ' ', '
',    ' ',    ' ',    ' ',    ' ',     NC},
        {' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ', '
',    ' ',    ' ',    ' ',    ' ',    ' '},
        {' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ', '
',    ' ',    ' ',    ' ',    ' ',    ' '},
        {' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ', '
',    ' ',    ' ',     NC,    ' ',    ' '},
        {' ',     NC,    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ', '
',    ' ',    ' ',    ' ',    ' ',     NC},
        {' ',    ' ',    ' ',     NC,     NC,     NC,    ' ',     NC,     NC, '
',    ' ',    ' ',    ' ',    ' ',    ' '}
    }};
*/
