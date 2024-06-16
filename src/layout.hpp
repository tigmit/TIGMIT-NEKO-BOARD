/**
 * layout.hpp
 * brief   : defining the layout for the keymatrix
 * created : 25.05.2024
 * creator : @tigmit
 * Licence : opensource
 */
#pragma once
#include "Defines\hardwareDef.hpp"
#include <BleKeyboard.h>

short rows[numRows]{row0, row1, row2, row3, row4, row5};

constexpr int NC = 0;
constexpr int KEY_FN_1 = 1;
constexpr int KEY_FN_2 = 2;

int layout1[numLayers][numRows][numCols]
{
    {
        {KEY_ESC,             NC,       KEY_F1,  KEY_F2,  KEY_F3,  KEY_F4,  KEY_F5,  KEY_F6,  KEY_F7,        KEY_F8,   KEY_F9,  KEY_F10,         KEY_F11,       KEY_F12,      KEY_PRTSC,             NC},
        {'`',                '1',          '2',     '3',     '4',     '5',     '6',     '7',     '8',           '9',      '0',      '-',             '=', KEY_BACKSPACE,            ' ',             NC},
        {KEY_TAB,            'q',          'w',     'e',     'r',     't',     'y',     'u',     'i',           'o',      'p',      '[',             ']',          '\\',     KEY_DELETE,             NC},
        {KEY_CAPS_LOCK,      'a',          's',     'd',     'f',     'g',     'h',     'j',     'k',           'l',      ';',     '\'',              NC,    KEY_RETURN,             NC,KEY_RIGHT_ARROW},
        {KEY_LEFT_SHIFT,     'z',          'x',     'c',     'v',     'b',     'n',     'm',     ',',           '.',      '/',       NC, KEY_RIGHT_SHIFT,            NC,             NC,   KEY_UP_ARROW},
        {KEY_LEFT_CTRL, KEY_HOME, KEY_LEFT_ALT,     NC ,      NC,     ' ',      NC,      NC,      NC, KEY_RIGHT_ALT, KEY_FN_1, KEY_FN_2,  KEY_RIGHT_CTRL,            NC, KEY_LEFT_ARROW, KEY_DOWN_ARROW}
    } 
};

short pressed[numLayers][numRows][numCols] = {OFF};



/*  LAYOUT BLANK : the layout is based on a qwerty TKL sized board. all values vontaining NC are not connected with a switch in the physical key-Matrix
    {
        {' ',     NC,    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',     NC},
        {' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',     NC},
        {' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',     NC},
        {' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',     NC,    ' ',     NC,    ' '},
        {' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',     NC,    ' ',     NC,     NC,    ' '},
        {' ',    ' ',    ' ',    NC ,     NC,    ' ',     NC,     NC,     NC,    ' ',    ' ',    ' ',    ' ',     NC,    ' ',    ' '}
    }
*/








