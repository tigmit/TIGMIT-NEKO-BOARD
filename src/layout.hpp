/**
 * layout.hpp
 * brief   : defining the layout for the keymatrix
 * created : 25.05.2024
 * creator : @tigmit
 * Licence : opensource
 */
#pragma once
#include "hardwareDef.hpp"
#include <BleKeyboard.h>

short rows[numRows]{row1, row2, row3, row4, row5, row6};

short cols[numCols]{col1,  col2,  col3,  col4,  col5,  col6,  col7,  col8, col9,
                    col10, col11, col12, col13, col14, col15, col16, col17};

constexpr int NC = 0;
constexpr int KEY_FN_1 = 1;
constexpr int KEY_FN_2 = 2;

int layout1[numLayers][numRows][numCols]
{
    {
        {KEY_ESC,             NC,       KEY_F1,  KEY_F2,  KEY_F3,  KEY_F4,  KEY_F5,  KEY_F6,  KEY_F7,        KEY_F8,   KEY_F9,  KEY_F10,         KEY_F11,       KEY_F12,      KEY_PRTSC,              0,               0},
        {'~',                '1',          '2',     '3',     '4',     '5',     '6',     '7',     '8',           '9',      '0',      '-',             '+', KEY_BACKSPACE,            ' ',            ' ',             ' '},
        {KEY_TAB,            'q',          'w',     'e',     'r',     't',     'z',     'u',     'i',           'o',      'p',      '{',             '}',          '\\',     KEY_DELETE,            ' ',             ' '},
        {KEY_CAPS_LOCK,      'a',          's',     'd',     'f',     'g',     'h',     'j',     'k',           'l',      'o',      'a',              NC,    KEY_RETURN,             NC,             NC,              NC},
        {KEY_LEFT_SHIFT,     'y',          'x',     'c',     'v',     'b',     'n',     'm',     ',',           '.',      '-',       NC, KEY_RIGHT_SHIFT,            NC,             NC,   KEY_UP_ARROW,              NC},
        {KEY_LEFT_CTRL, KEY_HOME, KEY_LEFT_ALT,     NC ,      NC,     ' ',      NC,      NC,      NC, KEY_RIGHT_ALT, KEY_FN_1, KEY_FN_2,  KEY_RIGHT_CTRL,            NC, KEY_LEFT_ARROW, KEY_DOWN_ARROW, KEY_RIGHT_ARROW}
    } 
};

short pressed[numLayers][numRows][numCols] = {OFF};



/*  LAYOUT BLANK : the layout is based on a qwerty TKL sized board. all values vontaining NC are not connected with a switch in the physical key-Matrix
    {
        {' ',     NC,    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' '},
        {' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' '},
        {' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' '},
        {' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',     NC,    ' ',     NC,     NC,     NC},
        {' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',    ' ',     NC,    ' ',     NC,     NC,    ' ',     NC},
        {' ',    ' ',    ' ',    NC ,     NC,    ' ',     NC,     NC,     NC,    ' ',    ' ',    ' ',    ' ',     NC,    ' ',    ' ',    ' '}
    }
*/







