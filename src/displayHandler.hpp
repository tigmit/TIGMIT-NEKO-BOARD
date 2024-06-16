/**
 * displayHandler.hpp
 * brief   : this class handles all the tasks of the displayHandler.
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
#include "Defines\displayDef.hpp"
#include "batteryHandler.hpp"
#include "debugSettings.hpp"
#include "helpers.hpp"
#include "keyBoardHandler.hpp"
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

class displayHandler {
public:
  displayHandler(batteryHandler *pBatHandler, keyboardHandler *pKbdHandler)
      : pBatHandler_(pBatHandler), pKbdHandler_(pKbdHandler) {
    ;
  }

  void init() {
    // Initialise the TFT
    tft.begin();
    tft.fillScreen(TFT_BLACK);
    initSprites();
  }

  void updateChargingScreen() {
    if (pBatHandler_->isCharging() && !chargeSet) {
      chargingScreen();
      chargeSet = true;
      connSet = false;
    } else if (!pBatHandler_->isCharging() && !connSet) {
      connectedScreen();
      connSet = true;
      chargeSet = false;
    }
  }

  void startScreen() {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 240 / 2, 4);
    tft.setTextColor(TFT_WHITE);
    tft.println(" starting keyboard...\n");
  }

  void waitForConnectionScreen() {
    printPNG(bongoLeft, sizeof(bongoLeft));
    tft.setCursor(10, 240 / 4, 2);
    tft.setTextColor(TFT_BLACK);
    tft.println(" wait for connection...\n");
  }

  void connectedScreen() {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 180, 4);
    tft.setTextColor(TFT_WHITE);
    tft.println("   Connected! :3\n");
  }

  void chargingScreen() {
    // TODO: Show more information about the battery
    // maybe a SOC graph?
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(35, 30, 2);
    tft.setTextColor(TFT_WHITE);
    tft.println("  charging (/*_*)/\n");
    // print voltage
    tft.print("  Voltage : ");
    tft.println(pBatHandler_->getVoltage());

    // print SOC
    tft.print("  SOC     : ");
    tft.println(pBatHandler_->getSOC());
  }

  void printSOC(int x, int y) {
    SOCtext.fillSprite(TFT_BLACK);
    SOCtext.setCursor(0, 0);
    SOCtext.print(" ");
    SOCtext.print((int)pBatHandler_->getSOC());
    SOCtext.print("%");
    SOCtext.pushSprite(x, y);
    currentSOC = pBatHandler_->getSOC();
  }

  void printKeyStreak(int16_t x, int16_t y) {

    tft.setCursor(x, y, 2);
    tft.setTextColor(TFT_BLACK);
    // tft.fillRect(x + 55, y, 50, 18, TFT_WHITE);
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
  void updateChargeIcon(int x = 90, int y = 10, u_int32_t BGcolor = TFT_BLACK);

  void drawBatteryIcon(int x = 90, int y = 10);
  // ____________________________________________DISPLAY MODES________________
  void mainScreen();
  void bongoMODE(); // drumm the bongo hehe

private:
  void printPNG(const byte *image, int size, int16_t x = 0, int16_t y = 0) {
    xpos = x;
    ypos = y;
    int16_t rc = png.openFLASH((uint8_t *)image, size, pngDraw);
    if (rc == PNG_SUCCESS) {
      tft.startWrite();
      rc = png.decode(NULL, 0);
      tft.endWrite();
      // png.close(); // not needed for memory->memory decode
    }
  }

  bool prevBtnPressed = false;
  bool bongoLftOn = false;
  bool bongoRgtOn = false;
  bool bongoRestOn = false;

  bool wallpaperOn = false;

  u_int32_t handsOfftrigger = 1500000;
  u_int32_t handsOffCount = handsOfftrigger;
  u_int32_t keyStreak = 0;

  bool chargeSet = false;
  bool connSet = false;

  float currentSOC = 0;

  // Sprites
  TFT_eSprite chargeLightning = TFT_eSprite(&tft);
  TFT_eSprite SOCtext = TFT_eSprite(&tft);

  batteryHandler *pBatHandler_ = nullptr;
  keyboardHandler *pKbdHandler_ = nullptr;
};

// This next function will be called during decoding of the png file to
// render each image line to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
// Callback function to draw pixels to the display
void pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WIDTH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  tft.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}

//_____________________________________________IMPLEMENTATIONS________

void displayHandler::mainScreen() {
  if (!wallpaperOn) {
    printPNG(wallpaper240X240, sizeof(wallpaper240X240), 0, 0);
    wallpaperOn = true;
  }
  updateChargeIcon();
}

void displayHandler::bongoMODE() {
  if ((pKbdHandler_->getKeyPressToggle() != prevBtnPressed) && !bongoLftOn) {
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
  } else if ((handsOffCount >= handsOfftrigger) && !bongoRestOn) {
    tft.fillScreen(TFT_WHITE); // once i have a state machine. this should be
                               // in enter method

    printPNG(bongoHandsOff, sizeof(bongoHandsOff), 0, 90);
    printKeyStreak(30, 60);

    bongoRestOn = true;
    bongoLftOn = false;
    bongoRgtOn = false;
    handsOffCount = 0;
    keyStreak = 0;
  } else if (!bongoRestOn) {
    // maybe we can fix this with a timer....?
    handsOffCount++;
  }
}

void displayHandler::updateChargeIcon(int x, int y, u_int32_t BGcolor) {

  if (pBatHandler_->isCharging() && !chargeSet) {

    pBatHandler_->updateBateryHandler(); // dont do this so often please

    // i want to erse just the symbol. not the wole screen
    chargeLightning.pushSprite(x + 20, y + 15);
    printSOC(x + 12, y + 25);
    chargeSet = true;
    connSet = false;
  } else if (!pBatHandler_->isCharging() && !connSet) {

    pBatHandler_->updateBateryHandler(); // dont do this so often please

    // i want to erse just the symbol. not the wole screen
    tft.fillCircle(x + 30, y + 30, 31, BGcolor);
    drawBatteryIcon();
    printSOC(x + 12, y + 25);
    connSet = true;
    chargeSet = false;
  }

  pBatHandler_->updateBateryHandler(); // dont do this so often please
  if (outOfBounds((int)pBatHandler_->getSOC(), currentSOC, 4)) {
    printSOC(x + 12, y + 25);
  }
}

void displayHandler::drawBatteryIcon(int x, int y) {
  tft.drawCircle(x + 30, y + 30, 30, TFT_GREEN);
  tft.drawCircle(x + 30, y + 30, 25, TFT_GREEN);
  tft.drawCircle(x + 30, y + 30, 20, TFT_GREEN);
}