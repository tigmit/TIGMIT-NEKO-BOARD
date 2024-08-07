#pragma once

#include "Defines/hardwareDef.hpp"
#include "ESP32Console.h"
#include "Model.hpp"
#include "helpers.hpp"
#include <Arduino.h>
#include <stdio.h>

using namespace ESP32Console;

class NekoShell {
public:
  void init() {

    console.setPrompt("meow > ");
    console.begin(BAUD_RATE);

    console.registerSystemCommands(); // Register builtin commands
    registerNekoCommands();           // register custom commands

    delay(100); // give shell time to start before wrinting welcome promt
    welcome();
  }

private:
  Console console;

  void registerNekoCommands() {
    /***************************************************************************
     *                                                                         *
     *                          REGISTER COMMANDS                              *
     *                                                                         *
     ***************************************************************************/

    console.registerCommand(
        ConsoleCommand("hello", &helloWorld, "just a hello world"));

    console.registerCommand(
        ConsoleCommand("setRGB", &setRGB, "set a new RGB LED color"));

    console.registerCommand(
        ConsoleCommand("clearPersistentMem", &clearPersistentMem,
                       "erase current RGB config stored in EEPROM"));

    console.registerCommand(ConsoleCommand("printConfigInEEPROM",
                                           &printConfigInEEPROM,
                                           "show hex values stored in EEPROM"));

    console.registerCommand(
        ConsoleCommand("writeConfigToEEPROM", &writeConfigToEEPROM,
                       "store current RGB config to EEPROM"));

    console.registerCommand(
        ConsoleCommand("setIndividualRGB", &setIndividualRGB,
                       "push a single LED provided with values for ROW, COLL"));

    /***************************************************************************/
  }

  /***************************************************************************
   *                                                                         *
   *                          IMPLEMENT COMMANDS                             *
   *                                                                         *
   ***************************************************************************/

  static int helloWorld(int argc, char **argv) { // this is an example Command.
    // Ensure that we have an argument to parse
    if (argc != 2) {
      printf("give me an argument pleas\n");
      // Return EXIT_FAILURE if something did not work.
      return EXIT_FAILURE;
    }
    // Take the first argument...
    auto arg = String(argv[1]);
    printf("you gave me : %s\n", arg);
    if (arg == "treat") {
      printf("NOM NOM NOM NOM! \n");
    } else {
      printf("but i am just a cat hehe \n");
    }
    // Return EXIT_SUCCESS if everything worked as intended.
    return EXIT_SUCCESS;
  }

  static int setRGB(int argc, char **argv) {
    // Ensure that we have an argument to parse
    if (argc != 4) {
      printf("need values for R, G, B\n");
      return EXIT_FAILURE;
    } else {
      // check max value
      for (int i = 1; i < 4; i++) {
        if (chartoint(argv[i]) > 255) {
          printf("val must be between 0 - 255\n");
          return EXIT_FAILURE;
        }
      }
    }
    // set to RGB handler
    rgbHandler.getRval() = chartoint(argv[1]);
    rgbHandler.getGval() = chartoint(argv[2]);
    rgbHandler.getBval() = chartoint(argv[3]);
    rgbHandler.pushCurrentRGBValues();
    printf("success. R: %d | G: %d | B: %d\n", rgbHandler.getRval(),
           rgbHandler.getGval(), rgbHandler.getBval());
    return EXIT_SUCCESS;
  }

  static int setIndividualRGB(int argc, char **argv) {
    // Ensure that we have an argument to parse
    if (argc != 3) {
      printf("please provide values for |0 <= Row <= 5| 0 <= Coll <= 14\n");
      // Return EXIT_FAILURE if something did not work.
      return EXIT_FAILURE;
    }
    // Take the first argument...
    uint8_t argRow = chartoint(argv[1]);
    uint8_t argColl = chartoint(argv[2]);
    printf("show RGB at ROW : %d | COLL : %d\n", argRow, argColl);

    rgbHandler.pushSingleRGB(argRow, argColl);

    // Return EXIT_SUCCESS if everything worked as intended.
    return EXIT_SUCCESS;
  }

  static int clearPersistentMem(int argc, char **argv) {
    printf("clear EEPROM\n");
    rgbHandler.getCurrentConfig().clearEEPROM();
    printf("done... wow. such empty\n");
    // Return EXIT_SUCCESS if everything worked as intended.
    return EXIT_SUCCESS;
  }

  static int printConfigInEEPROM(int argc, char **argv) {
    rgbHandler.getCurrentConfig().printConfigBytesInEEPROM();
    // Return EXIT_SUCCESS if everything worked as intended.
    return EXIT_SUCCESS;
  }

  static int writeConfigToEEPROM(int argc, char **argv) {
    rgbHandler.getCurrentConfig().storeConfigToEEPROM();
    // Return EXIT_SUCCESS if everything worked as intended.
    return EXIT_SUCCESS;
  }

  /***************************************************************************/

  void welcome() {
    Serial.println("");
    Serial.println(" _   _  ______  _  __  ____      ");
    Serial.println("| \\ | ||  ____|| |/ / / __ \\   ");
    Serial.println("|  \\| || |__   | ' / | |  | |   ");
    Serial.println("| . ` ||  __|  |  <  | |  | |    ");
    Serial.println("| |\\  || |____ | . \\ | |__| |  ");
    Serial.println("|_| \\_||______||_|\\_\\ \\____/ ");
    Serial.println("                                                  ");
    Serial.println("  ####                                            ");
    Serial.println(" #    #             ####                          ");
    Serial.println("##    #             #    ## #########     #####   ");
    Serial.println("#     #             ##  ###           ####    #   ");
    Serial.println("##    #            ##                         #   ");
    Serial.println(" #    ##          #                           #   ");
    Serial.println(" ##    #         #                             #  ");
    Serial.println("  ##   ##        # ###                          # ");
    Serial.println("   ##   ## #####       ####                      #");
    Serial.println("    ##    ##               ###                   #");
    Serial.println("      #     ##                                  # ");
    Serial.println("     #####                                     ## ");
    Serial.println("    #    ##                              ## ###   ");
    Serial.println("   ##    ## ##                           ##       ");
    Serial.println("   ##     ####                           ##       ");
    Serial.println("   ##                                    ##       ");
    Serial.println("    #                                    #        ");
    Serial.println("     #                                  ##        ");
    Serial.println("     ##      #########       ######     #         ");
    Serial.println("      #        #     #       #   #     ##         ");
    Serial.println("      ##       #     #       #  #     ##          ");
    Serial.println("       ##     ##     #      ##  ##   ##           ");
    Serial.println("         #####       ##     #     ##              ");
    Serial.println("                       #####                      ");
    Serial.println("            _____  _    _  ______  _       _      ");
    Serial.println("           / ____|| |  | ||  ____|| |     | |     ");
    Serial.println("          | (___  | |__| || |__   | |     | |     ");
    Serial.println("           \\___ \\ |  __  ||  __|  | |     | |   ");
    Serial.println("           ____) || |  | || |____ | |____ | |____ ");
    Serial.println("          |_____/ |_|  |_||______||______||______|");
    printf("\n\nWelcome to TIGMIT NEKO SHELL\n try typing 'help' to list all "
           "commands :3'\n\n");
  }
};