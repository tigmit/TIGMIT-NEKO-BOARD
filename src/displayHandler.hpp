/**
 * displayHandler.hpp
 * brief   : this class handles all the tasks of the displayHandler.
 * created : 26.05.2024
 * creator : @tigmit Licence : opensource
 */
#pragma once
#include "Defines\displayDef.hpp"
#include "bongoLeft.h" // Image is stored here in an 8-bit array
#include "bongoRight.h"
#include "debugSettings.hpp"
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
  displayHandler() = default;

  void init() {
    // Initialise the TFT
    tft.begin();
    tft.fillScreen(TFT_BLACK);
  }

  void startScreen() {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 240 / 2, 4);
    tft.setTextColor(TFT_WHITE);
    tft.println(" starting keyboard...\n");
  }

  void waitForConnectionScreen() {
    tft.fillScreen(TFT_BLUE);
    tft.setCursor(10, 240 / 2, 4);
    tft.setTextColor(TFT_WHITE);
    tft.println(" wait for connection...\n");
  }

  void bongoPlay() {
    int16_t rc =
        png.openFLASH((uint8_t *)bongoLeft, sizeof(bongoLeft), pngDraw);
    if (rc == PNG_SUCCESS) {
      tft.startWrite();
      rc = png.decode(NULL, 0);
      // uint32_t dt = millis();
      // Serial.print(millis() - dt);
      // Serial.println("ms");
      tft.endWrite();
      // png.close(); // not needed for memory->memory decode
    }
    delay(50);
    rc = png.openFLASH((uint8_t *)bongoRight, sizeof(bongoRight), pngDraw);
    if (rc == PNG_SUCCESS) {
      tft.startWrite();
      rc = png.decode(NULL, 0);
      // uint32_t dt = millis();
      // Serial.print(millis() - dt);
      // Serial.println("ms");
      tft.endWrite();
      // png.close(); // not needed for memory->memory decode
    }
    delay(50);
  }

private:
  /* data */
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
