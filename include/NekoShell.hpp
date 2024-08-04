#pragma once

#include "Defines/hardwareDef.hpp"
#include "ESP32Console.h"
#include <Arduino.h>
#include <stdio.h>

using namespace ESP32Console;

class NekoShell {
public:
  void init() {

    console.setPrompt("meow > ");

    console.begin(BAUD_RATE);

    console.registerSystemCommands(); // Register builtin commands

    /***************************************************************************
     *                                                                         *
     *                          REGISTER COMMANDS                              *
     *                                                                         *
     ***************************************************************************/

    console.registerCommand(
        ConsoleCommand("hello", &helloWorld, "just a hello world"));

    /***************************************************************************/

    // When console is in use, we can not use Serial.print but you can use
    // printf to output text
    delay(100); // give shell time to start before wprinting welcome promt
    welcome();
  }

private:
  Console console;

  static int helloWorld(int argc, char **argv) {
    // Ensure that we have an argument to parse
    if (argc != 2) {
      printf("give me an argument pleas\n");
      // Return EXIT_FAILURE if something did not work.
      return EXIT_FAILURE;
    }

    // Take the first argument...
    auto arg = String(argv[1]);

    // and use it to decide what to do with the LED
    printf("you gave me : %s\n", arg);
    printf("but i am just a cat hehe \n");

    // Return EXIT_SUCCESS if everything worked as intended.
    return EXIT_SUCCESS;
  }

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