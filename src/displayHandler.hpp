/**
 * DisplayHandler.hpp
 * brief   : this class handles all the tasks of the DisplayHandler.
 * created : 26.05.2024
 * creator : @tigmit Licence : opensource
 */

// Image files can be converted to arrays using the tool here:
// https://notisrac.github.io/FileToCArray/
// To use this tool:
//   1. Drag and drop file on "Browse..." button
//   2. Tick box "Treat as binary"
//   3. Click "Convert"
//   4. Click "Save as file" and move the header file to sketch folder
//   5. use the array in your display designs

#pragma once
#include "BatteryHandler.hpp"
#include "Defines\displayDef.hpp"
#include "EncoderHandler.hpp"
#include "KeyboardHandler.hpp"
#include "RgbHandler.hpp"
#include "debugSettings.hpp"
#include "helpers.hpp"
#include <PNGdec.h>

#include "immages/allImmages.hpp"

#include "SPI.h"
#include <TFT_eSPI.h> // Hardware-specific library

#define MAX_IMAGE_WIDTH 240 // Adjust for your images

PNG png;                   // PNG decoder instance
TFT_eSPI tft = TFT_eSPI(); // Invoke custom library
int16_t xpos = 0;
int16_t ypos = 0;

void pngDraw(PNGDRAW *pDraw);

//-------------------------------------Display  Wrapper Class------------
class DisplayHandler {
public:
  DisplayHandler(BatteryHandler *pBatHandler, KeyboardHandler *pKbdHandler)
      : pBatHandler_(pBatHandler), pKbdHandler_(pKbdHandler) {}

  void init() {
    // make sure dependecies arent NULL!
    assert(pBatHandler_);
    assert(pKbdHandler_);
    // Initialise the TFT
    tft.begin();
    tft.fillScreen(TFT_WHITE);
    initSprites();
  }

  //-------------------------------------Helpers-----------------------

  void printSOC(int x, int y) {
    SOCtext.fillSprite(TFT_TRANSPARENT);
    SOCtext.setCursor(0, 0);
    SOCtext.print("  ");
    SOCtext.print((int)pBatHandler_->getSOC());
    SOCtext.print("%");
    SOCtext.pushSprite(x, y);
    currentSOC = pBatHandler_->getSOC();
  }

  void printKeyStreak(int16_t x, int16_t y) {
    tft.setCursor(x, y, 2);
    tft.setTextColor(TFT_BLACK);
    tft.fillRect(x + 55, y, 50, 18, TFT_WHITE);
    tft.print(" streak : ");
    tft.print(keyStreak);
  }

  void initSprites() {
    // charging Lightning Sprite
    chargeLightning.setColorDepth(1);
    chargeLightning.createSprite(20, 30);
    chargeLightning.fillTriangle(0, 19, 12, 19, 12, 0, TFT_WHITE);
    chargeLightning.fillTriangle(7, 30, 7, 11, 19, 11, TFT_WHITE);

    // SOC text sprite
    SOCtext.setColorDepth(8);
    SOCtext.createSprite(35, 10);
    SOCtext.setTextColor(TFT_WHITE);
    SOCtext.setTextFont(1);
  }

  // ____________________________________________DISPLAY ELEMENTS_____________

  void updateChargeIcon(int x = 90, int y = 6, u_int32_t BGcolor = TFT_BLACK);
  void updateBatteryIcon(int x = 90, int y = 10, u_int32_t BGcolor = TFT_BLACK);
  void updateCapslockIcon(int x = 90, int y = 10, bool caps = false);
  void updateBLEIcon(int x, int y, bool connected = false);

  void drawMainScreenSelect(int x = 120, int y = 120);
  void drawRgbScreenSelect();

  void drawRgbConfigWheel(int value, u_int32_t settingsColor = TFT_LIGHTGREY,
                          int x = 120, int y = 120);

  void printPNG(const byte *image, int size, int16_t x = 0, int16_t y = 0) {
    xpos = x;
    ypos = y;
    int16_t rc = png.openFLASH((uint8_t *)image, size, pngDraw);
    if (rc == PNG_SUCCESS) {
      tft.startWrite();
      rc = png.decode(NULL, 0);
      tft.endWrite();
    }
  }

  // ---------------------------------- Main Modes---------------------------

  //  Bongo cat update:
  void bongoMode(); // drumm the bongo hehe

  void resetRgbScreenFlags() {
    rgbScreenWallpaperOn = false;
    currentRgbSelect = 0;
    rgbSelect = 0;
  }

  void resetDspdrawflags() {
    capsLockON = false;
    capsLockOFF = false;
    mainWallpaperOn = false;
    BLEwaitOn = false;
    BLEconnOn = false;
    battIconOn = false;
    chargeFlashOn = false;
  }

  void resetMainScreenWheel() {
    MainScreenSelect = 0;
    currentMainSelect = 0;
  }

  void resetBongoMode() {
    handsOfftrigger = 700000;
    handsOffCount = handsOfftrigger;
    keyStreak = 0;
    bongoLftOn = false;
    bongoRgtOn = false;
    bongoRestOn = false;
  }

  void drawMainScreenWallpaper() {
    if (!mainWallpaperOn) {
      // print wallpaper
      printPNG(wallpaper240X240, sizeof(wallpaper240X240), 0, 0);
      mainWallpaperOn = true;
    }
  }

  void drawRgbScreenWallpaper() {
    if (!rgbScreenWallpaperOn) {
      printPNG(LEDSettingsScreen, sizeof(LEDSettingsScreen), 0, 0);
      rgbScreenWallpaperOn = true;
    }
  }

  int &getMainItemSelect() { return MainScreenSelect; }
  int &getRgbItemSelect() { return rgbSelect; }

private:
  // immage draw states
  bool prevBtnPressed = false;
  bool bongoLftOn = false;
  bool bongoRgtOn = false;
  bool bongoRestOn = false;

  bool BLEwaitOn = false;
  bool BLEconnOn = false;

  bool capsLockON = false;
  bool capsLockOFF = false;

  bool mainWallpaperOn = false;
  bool rgbScreenWallpaperOn = false;

  u_int32_t handsOfftrigger = 700000;
  u_int32_t handsOffCount = handsOfftrigger;
  u_int32_t keyStreak = 0;

  bool chargeFlashOn = false;
  bool battIconOn = false;

  float currentSOC = 0;

  int currentRgbSelect = 0;
  int rgbSelect = 0;

  int currentMainSelect = 0;
  int MainScreenSelect = 0;

  const uint32_t mainSelectWheelAngles[5][2] = {
      {144, 216}, {216, 288}, {288, 360}, {360, 72}, {72, 144}};

  const uint32_t LedSelectRings[6][3] = {{95, 62, 19},   {145, 86, 19},
                                         {198, 107, 19}, {147, 189, 21},
                                         {93, 192, 21},  {37, 140, 19}};

  // Sprites
  TFT_eSprite chargeLightning = TFT_eSprite(&tft);
  TFT_eSprite SOCtext = TFT_eSprite(&tft);

  // refferences
  BatteryHandler *pBatHandler_ = nullptr;
  KeyboardHandler *pKbdHandler_ = nullptr;
};

//____________________________________________________________________________________________________________________
//____________________________________________________________________________________________________________________
//____________________________________________________________________________________________________________________
//____________________________________________________________________________________________________________________
//____________________________________________________________________________________________________________________
//____________________________________________________________________________________________________________________

// This next function will be called during decoding of the png file to
// render each image line to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
// Callback function to draw pixels to the display
void pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WIDTH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  tft.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}

//_____________________________________________IMPLEMENTATIONS___________________

void DisplayHandler::updateChargeIcon(int x, int y, u_int32_t BGcolor) {

  if (pBatHandler_->isCharging() && !chargeFlashOn) {

    // i want to erse just the symbol. not the wole screen
    chargeLightning.pushSprite(x + 20, y + 15);
    printSOC(x + 12, y + 25);
    chargeFlashOn = true;
    battIconOn = false;
  } else if (!pBatHandler_->isCharging() && !battIconOn) {

    // i want to erse just the symbol. not the wole screen
    updateBatteryIcon();
    printSOC(x + 12, y + 25);
    battIconOn = true;
    chargeFlashOn = false;
  }

  if (outOfBounds((int)pBatHandler_->getSOC(), currentSOC, 2)) {
    printSOC(x + 12, y + 25);
  }
}

void DisplayHandler::updateBatteryIcon(int x, int y, u_int32_t BGcolor) {
  tft.fillCircle(x + 30, y + 30, 31, BGcolor);
  tft.drawCircle(x + 30, y + 30, 30, TFT_GREEN);
  tft.drawCircle(x + 30, y + 30, 25, TFT_GREEN);
  tft.drawCircle(x + 30, y + 30, 20, TFT_GREEN);
}

void DisplayHandler::updateBLEIcon(int x, int y, bool connected) {

  if (connected && !BLEconnOn) {
    BLEwaitOn = false;
    BLEconnOn = true;
  } else if (!connected && !BLEwaitOn) {
    BLEwaitOn = true;
    BLEconnOn = false;
  } else
    return;

  uint32_t BLE_COLOR = connected ? TFT_WHITE : TFT_RED;

  // draw blue circle with border (2 pixel border)
  tft.fillCircle(x + 30, y + 30, 30, BLE_COLOR);
  tft.fillCircle(x + 30, y + 30, 28, TFT_BLUE);
  // draw BLE symbol
  tft.drawLine(x + 30, y + 50, x + 30, y + 10, BLE_COLOR); // long vertical
  tft.drawLine(x + 15, y + 40, x + 45, y + 20, BLE_COLOR); // diagonal center up
  tft.drawLine(x + 15, y + 20, x + 45, y + 40, BLE_COLOR); // diag. cntr down
  tft.drawLine(x + 30, y + 50, x + 45, y + 40, BLE_COLOR);
  tft.drawLine(x + 30, y + 10, x + 45, y + 20, BLE_COLOR);
}

void DisplayHandler::updateCapslockIcon(int x, int y, bool caps) {

  if (!caps && !capsLockOFF) {
    capsLockON = false;
    capsLockOFF = true;
  } else if (caps && !capsLockON) {
    capsLockON = true;
    capsLockOFF = false;
  } else
    return;

  uint32_t CAPS_COLOR = caps ? TFT_RED : TFT_WHITE;

  // draw grey circle with wihte border (2 pixel border)
  tft.fillCircle(x + 30, y + 30, 30, CAPS_COLOR);
  tft.fillCircle(x + 30, y + 30, 24, TFT_LIGHTGREY);
  // draw caps lock symbol
  tft.setTextColor(TFT_BLACK);
  tft.drawString("aA", x + 17, y + 20, 4);
}

void DisplayHandler::drawMainScreenSelect(int x, int y) {
  if (MainScreenSelect != currentMainSelect) {
    currentMainSelect = MainScreenSelect;

    tft.drawArc(x, y, 120, 117, 0, 360, TFT_BLACK, TFT_BLACK, true);
    tft.drawArc(x, y, 120, 117, mainSelectWheelAngles[currentMainSelect][0],
                mainSelectWheelAngles[currentMainSelect][1], TFT_GREEN,
                TFT_BLACK, true);
  }
}

void DisplayHandler::drawRgbScreenSelect() {
  if (rgbSelect != currentRgbSelect) {
    currentRgbSelect = rgbSelect;

    for (int i = 0; i < 6; i++) {
      tft.drawArc(LedSelectRings[i][0], LedSelectRings[i][1],
                  LedSelectRings[i][2] + 3, LedSelectRings[i][2], 0, 360,
                  TFT_WHITE, TFT_WHITE, true);
    }
    tft.drawArc(LedSelectRings[currentRgbSelect][0],
                LedSelectRings[currentRgbSelect][1],
                LedSelectRings[currentRgbSelect][2] + 3,
                LedSelectRings[currentRgbSelect][2], 0, 360, TFT_DARKGREY,
                TFT_DARKGREY, true);
  }
}

void DisplayHandler::drawRgbConfigWheel(int value, u_int32_t settingsColor,
                                        int x, int y) {

  tft.drawArc(x, y, 120, 111, 0, 360, TFT_BLACK, TFT_BLACK, true);
  tft.drawArc(x, y, 119, 112, 308, 52, settingsColor, settingsColor, true);
  tft.drawArc(x, y, 119, 112, 52, 52 + value, TFT_WHITE, TFT_WHITE, true);
}

/**************************************************STATES****************************/

/**
 * Bongo Cat State
 */
void DisplayHandler::bongoMode() {
  if ((handsOffCount >= handsOfftrigger) && !bongoRestOn) {
    tft.fillScreen(TFT_WHITE);
    printPNG(bongoHandsOff, sizeof(bongoHandsOff), 0, 90);
    printKeyStreak(30, 60);

    bongoRestOn = true;
    bongoLftOn = false;
    bongoRgtOn = false;
    handsOffCount = 0;
    keyStreak = 0;
  } else if ((pKbdHandler_->getKeyPressToggle() != prevBtnPressed) &&
             !bongoLftOn) {
    prevBtnPressed = pKbdHandler_->getKeyPressToggle();

    printPNG(bongoLeft, sizeof(bongoLeft), 0, 90);
    keyStreak++;
    printKeyStreak(30, 60);

    bongoRestOn = false;
    bongoLftOn = true;
    bongoRgtOn = false;
    handsOffCount = 0;
  } else if ((pKbdHandler_->getKeyPressToggle() != prevBtnPressed) &&
             !bongoRgtOn) {
    prevBtnPressed = pKbdHandler_->getKeyPressToggle();

    printPNG(bongoRight, sizeof(bongoRight), 0, 90);
    keyStreak++;
    printKeyStreak(30, 60);

    bongoRestOn = false;
    bongoLftOn = false;
    bongoRgtOn = true;
    handsOffCount = 0;
  } else if (!bongoRestOn) {
    // maybe we can fix this with a timer....?
    handsOffCount++;
  }
}
