#include <msp430.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "i2c.h"
#include "lcd.h"

// Initialize the feeder with a message.
void feederInitialize(void){
    __delay_cycles(10000);
    lcdPrint("Automatic Feeder");
    lcdNextLine();
    lcdPrint("  Initialized.");
    __delay_cycles(2000000);
    lcdResetDisplay();
}

// Print feeder's time that was set.
void printTime(char vect[6]){
    char lcdStringToPrint[6];

    lcdStringToPrint[0] = vect[2];
    lcdStringToPrint[1] = vect[3];
    lcdStringToPrint[2] = ':';
    lcdStringToPrint[3] = vect[4];
    lcdStringToPrint[4] = vect[5];
    lcdStringToPrint[5] = '\0';

    lcdPrint("     ");
    lcdPrint(lcdStringToPrint);
}

// Show feeder instructions.
void feederInstructions(void){
    __delay_cycles(10000);
    lcdPrint("  To configure");
    lcdNextLine();
    lcdPrint(" hour, press H.");
    __delay_cycles(1000000);
    lcdResetDisplay();

    __delay_cycles(10000);
    lcdPrint("  To configure");
    lcdNextLine();
    lcdPrint("feeder, press A.");
    __delay_cycles(1000000);
    lcdResetDisplay();

    __delay_cycles(10000);
    lcdPrint(" Press O to see");
    lcdNextLine();
    lcdPrint("  the options.");
    __delay_cycles(1000000);
    lcdResetDisplay();
}

// Show message that the time is being configured.
void printConfigTime(){
    lcdResetDisplay();
    __delay_cycles(10000);
    lcdPrint("  Configuring");
    lcdNextLine();
    lcdPrint("    time...");
}

// Show message that the feeder is being configured.
void printConfigFeeder(){
    lcdResetDisplay();
    __delay_cycles(10000);
    lcdPrint("  Configuring");
    lcdNextLine();
    lcdPrint("   feeder...");
}

// Adjust the opening magnitude or time.
int adjustOpening(char option[6]){
    char openingAdjust[4];
    int len = strlen(option);
    strcpy(openingAdjust, option + 2);
    if (option[1] == 'T' || option[1] == 't'){
        if (option[2] == '1'){
            __delay_cycles(1000000);
        }
        else if (option[2] == '2'){
            __delay_cycles(2000000);
        }
        else if (option[2] == '3'){
            __delay_cycles(3000000);
        }
        else if (option[2] == '4'){
            __delay_cycles(4000000);
        }
        else if (option[2] == '5'){
            __delay_cycles(5000000);
        }
    }
    return atoi(openingAdjust);
}

// Show the feeder options.
void printOptions(){
    lcdResetDisplay();
    __delay_cycles(10000);
    lcdPrint("  To configure");
    lcdNextLine();
    lcdPrint(" hour, press H.");
    __delay_cycles(2000000);
    lcdResetDisplay();
    __delay_cycles(10000);
    lcdPrint("  To configure");
    lcdNextLine();
    lcdPrint("feeder, press A.");
    __delay_cycles(2000000);
    lcdResetDisplay();
    __delay_cycles(10000);
    lcdPrint(" To adjust the");
    lcdNextLine();
    lcdPrint("opening, press I");
    __delay_cycles(2000000);
    lcdResetDisplay();
    __delay_cycles(10000);
    lcdPrint("Then Press M to");
    lcdNextLine();
    lcdPrint("   change the");
    __delay_cycles(300000);
    lcdResetDisplay();
    __delay_cycles(10000);
    lcdPrint("    opening");
    lcdNextLine();
    lcdPrint("   magnitude");
    __delay_cycles(2000000);
    lcdResetDisplay();
    __delay_cycles(10000);
    lcdPrint(" Or Press T to");
    lcdNextLine();
    lcdPrint("   change the");
    __delay_cycles(300000);
    lcdResetDisplay();
    __delay_cycles(10000);
    lcdPrint("    opening");
    lcdNextLine();
    lcdPrint("     time.");
    __delay_cycles(2000000);
    lcdResetDisplay();
    __delay_cycles(10000);
    lcdPrint("   Press R to");
    lcdNextLine();
    lcdPrint(" cancel command");
    __delay_cycles(2000000);
    lcdResetDisplay();
}

// Show message that the opening is being configured.
void printConfigOpening(){
    lcdResetDisplay();
    __delay_cycles(10000);
    lcdPrint("  Configuring");
    lcdNextLine();
    lcdPrint("   opening...");
}

// Show on LCD the message that the opening magnitude is being configured.
void printConfigOpeningMagnitude(){
    lcdResetDisplay();
    __delay_cycles(10000);
    lcdPrint("  Configuring");
    lcdNextLine();
    lcdPrint("  magnitude...");
}

// Show on LCD the message that the opening time is being configured.
void printConfigOpeningTime(){
    lcdResetDisplay();
    __delay_cycles(10000);
    lcdPrint("  Configuring");
    lcdNextLine();
    lcdPrint("    time...");
}

// Show the new opening magnitude.
void printOpeningMagnitude(char vect[6]){
    char lcdStringToPrint[3];

    lcdStringToPrint[0] = vect[2];
    lcdStringToPrint[1] = vect[3];
    lcdStringToPrint[2] = '\0';

    lcdPrint("    ");
    lcdPrint(lcdStringToPrint);
    lcdPrint(" degrees");
}

// Show the new opening time.
void printOpeningTime(char vect[6]){
    char lcdStringToPrint[2];

    lcdStringToPrint[0] = vect[2];
    lcdStringToPrint[1] = '\0';

    lcdPrint("   ");
    lcdPrint(lcdStringToPrint);
    lcdPrint(" seconds");
}

// Message to warn about invalid command.
void printInvalidCommand(){
    __delay_cycles(10000);
    lcdPrint("    Invalid");
    lcdNextLine();
    lcdPrint("    Command");
    __delay_cycles(1000000);
    lcdResetDisplay();
}

// Cancel command at any time.
void printCancellingCommand(){
    lcdResetDisplay();
    __delay_cycles(10000);
    lcdPrint("    Cancelling");
    lcdNextLine();
    lcdPrint("   Command");
    __delay_cycles(1000000);
    lcdResetDisplay();
}

// Show message to ask user to configure the feeder
void printPleaseConfigure(){
    __delay_cycles(10000);
    lcdPrint("Please configure");
    lcdNextLine();
    lcdPrint("   the feeder");
}

void showRealTime(){
    // Get the current time from the RTC registers
    int hours = RTCHOUR;
    int minutes = RTCMIN;
    int seconds = RTCSEC;

    lcdResetDisplay();
    __delay_cycles(10000);
    if (RTCDOW == 0){
        lcdPrintFast("     Sunday");
    }
    else if (RTCDOW == 1){
        lcdPrintFast("     Monday");
    }
    else if (RTCDOW == 2){
        lcdPrintFast("    Tuesday");
    }
    else if (RTCDOW == 3){
        lcdPrintFast("   Wednesday");
    }
    else if (RTCDOW == 4){
        lcdPrintFast("    Thursday");
    }
    else if (RTCDOW == 5){
        lcdPrintFast("     Friday");
    }
    else if (RTCDOW == 6){
        lcdPrintFast("    Saturday");
    }

    // Convert the time values to ASCII characters
    lcdNextLine();
    lcdPrintFast("    ");
    char hour_str[3];
    sprintf(hour_str, "%d", hours);
    if (hours < 10){
        lcdPrintFast("0");
        lcdPrintFast(hour_str);
    }
    else{
        lcdPrintFast(hour_str);
    }
    lcdPrintFast(":");
    char min_str[3];
    sprintf(min_str, "%d", minutes);
    if (minutes < 10){
        lcdPrintFast("0");
        lcdPrintFast(min_str);
    }
    else{
        lcdPrintFast(min_str);
    }
    lcdPrintFast(":");
    char sec_str[3];
    sprintf(sec_str, "%d", seconds);
    if (seconds < 10){
        lcdPrintFast("0");
        lcdPrintFast(sec_str);
    }
    else{
        lcdPrintFast(sec_str);
    }
}
