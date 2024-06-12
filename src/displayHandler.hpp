/**
 * displayHandler.hpp
 * brief   : this class handles all the tasks of the displayHandler.
 * created : 26.05.2024
 * creator : @tigmit Licence : opensource
 */
#pragma once
#include "Defines\displayDef.hpp"
#include "batteryHandler.hpp"
#include "battery_icon.hpp"
#include "bongoLeft.h" // Image is stored here in an 8-bit array
#include "bongoRight.h"
#include "charging_icon.hpp"
#include "debugSettings.hpp"
#include "helpers.hpp"
#include "keyBoardHandler.hpp"
#include <PNGdec.h>

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

  void updateChargeIcon(int x = 150, int y = 60) {

    if (pBatHandler_->isCharging() && !chargeSet) {

      pBatHandler_->updateBateryHandler(); // dont do this so often please

      // i want to erse just the symbol. not the wole screen
      tft.fillRect(x, y, 60, 80, TFT_BLACK);
      printPNG(charging_icon, sizeof(charging_icon), 150, 60);
      printSOC(x, y);
      chargeSet = true;
      connSet = false;
    } else if (!pBatHandler_->isCharging() && !connSet) {

      pBatHandler_->updateBateryHandler(); // dont do this so often please

      // i want to erse just the symbol. not the wole screen
      tft.fillRect(x, y, 60, 80, TFT_BLACK);
      printPNG(battery_icon, sizeof(battery_icon), 150, 60);
      printSOC(x, y);
      connSet = true;
      chargeSet = false;
    }

    pBatHandler_->updateBateryHandler(); // dont do this so often please
    // if (outOfBounds((int)pBatHandler_->getSOC(), currentSOC, 4)) {
    //  printSOC(x, y);
    //}
  }

  void printSOC(int x = 150, int y = 60) {
    tft.fillRect(x, y + 60, 60, 20, TFT_BLACK);
    tft.setCursor(x + 5, y + 60, 2);
    tft.setTextColor(TFT_WHITE);

    tft.print(pBatHandler_->getSOC());
    tft.println("%");
    currentSOC = pBatHandler_->getSOC();
  }

  void bongoPlay() {
    printPNG(bongoLeft, sizeof(bongoLeft));
    delay(100);
    printPNG(bongoRight, sizeof(bongoRight));
    delay(100);
  }

  void bongoMODE() {
    if ((pKbdHandler_->getKeyPressToggle() != prevBtnPressed) && !bongoLftOn) {
      prevBtnPressed = pKbdHandler_->getKeyPressToggle();
      printPNG(bongoLeft, sizeof(bongoLeft));
      bongoLftOn = true;
      bongoRgtOn = false;
    } else if ((pKbdHandler_->getKeyPressToggle() != prevBtnPressed) &&
               !bongoRgtOn) {
      prevBtnPressed = pKbdHandler_->getKeyPressToggle();
      printPNG(bongoRight, sizeof(bongoRight));
      bongoLftOn = false;
      bongoRgtOn = true;
    }
  }

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

  bool chargeSet = false;
  bool connSet = false;

  float currentSOC = 0;

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

// Image files can be converted to arrays using the tool here:
// https://notisrac.github.io/FileToCArray/
// To use this tool:
//   1. Drag and drop file on "Browse..." button
//   2. Tick box "Treat as binary"
//   3. Click "Convert"
//   4. Click "Save as file" and move the header file to sketch folder
//   5. Open the sketch in IDE
//   6. Include the header file containing the array (panda.h in this example)
