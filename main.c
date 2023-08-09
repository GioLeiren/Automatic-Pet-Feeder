#include <msp430.h> 
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "libs/servomotor.h"
#include "libs/realtimeclock.h"
#include "libs/uart.h"
#include "libs/i2c.h"
#include "libs/lcd.h"
#include "libs/feeder.h"

typedef enum { false, true } bool;

char uart_input[6] = {'s'};         // Array that stores UART sequence to set time and RTC alarm.
char uart_input_char;               // Variable that stores current UART input for some operations.
unsigned int uart_input_index = 0;  // Index of the Array, used to store elements in the Array or reset it.
unsigned int opening = 90;          // Default opening for the feeder opening (90 degrees on the servomotor).
int openingTime = 3;                // Default time in which the opening remains open for the cat food to drop.
char openingArray[6] = {'i', 't', '3'};
bool configTime = false;            // Bool
bool configFeeder = false;          // variables
bool configOpening = false;         // used
bool configOpeningMagnitude = false;// to
bool configOpeningTime = false;     // halt the LCD display while the user is configuring something in the feeder.
bool showTime = false;              // Flag to stop showing real time while the user is configuring something or checking the feeder's options.


void setTime(char vector[6]);       // Function to set the feeder time when it is initialized.
void setAlarm(char vector[5]);      // Function to set the RTC alarm, or in other words, to set the time in which the feeder will work.
int checkCommand(char vector[5]);   // Check if command is valid.

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer.

    // This block initializes everything needed.
    i2cConfig();
    lcdInit();
    uartInit();
    servoConfig();
    configureRTC();

    // This block starts the leds.
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;
    P4DIR |= BIT7;
    P4OUT &= ~BIT7;

    __enable_interrupt();           // Enabling interruptions.

    // Leds turned on.
    P1OUT |= BIT0;
    P4OUT |= BIT7;

    feederInitialize();             // Initialize the feeder displaying a message on the LCD.
    feederInstructions();           // Show feeder instructions.
    lcdResetDisplay();              // Reset LCD display.

    // Leds turned off.
    P1OUT &= ~BIT0;
    P4OUT &= ~BIT7;

    printPleaseConfigure();

    while(1){
        RTCADOW = RTCAE | RTCDOW;      // Make RTACDOW and RTCDOW equivalent for alarm interruption to work.

        if (showTime == true){
            showRealTime();            // Show real time when the feeder has been configured.
            __delay_cycles(1000000);
        }


        if (checkCommand(uart_input) == 0){
            printInvalidCommand();     // Show Invalid Command message.
            uart_input_index = 0;      // Put array pointer to first position.
            uart_input[0] = 's';       // Change the first element of uart_input so that the code doesn't loop inside this if block.
        }


        if (uart_input_char == 'R' || uart_input_char == 'r'){
            printCancellingCommand();
            uart_input_index = 0;      // Put array pointer to first position.
            uart_input[0] = 's';       // Change the first element of uart_input so that the code doesn't loop inside this if block.
            uart_input_char = 's';     // Change uart_input_char to safety command to prevent unwanted if loop.
        }


        if (uart_input[0] == 'H' || uart_input[0] == 'h'){  // Configure feeder time.
            showTime = false;
            if (configTime == false){   // Condition to halt the LCD, to keep the message in it while the user is configuring the time.
                printConfigTime();      // Print a message to show that the time is being configured.
                configTime = true;      // Set configTime flag to true.
            }
            if (uart_input_index == 6){ // Check if The full command to set time has been given.
                setTime(uart_input);    // Set feeder time.
                lcdResetDisplay();

                __delay_cycles(10000);

                lcdPrint("  Time set to:");
                lcdNextLine();
                printTime(uart_input);

                __delay_cycles(3000000);

                lcdResetDisplay();
                uart_input_index = 0;   // Put array pointer to first position.
                uart_input[0] = 's';    // Change the first element of uart_input so that the code doesn't loop inside this if block.
                configTime = false;     // Reset configTime flag.
                showTime = true;        // Start showing real time.
            }
        }


        if (uart_input[0] == 'A' || uart_input[0] == 'a'){
            showTime = false;
            if (configFeeder == false){   // Condition to halt the LCD, to keep the message in it while the user is configuring the feeder.
                printConfigFeeder();      // Print a message to show that the feeder is being configured.
                configFeeder = true;          // Set configFeeder flag to true.
            }
            if (uart_input_index == 5){   // Check if The full command to configure feeder has been given.
                setAlarm(uart_input);     // Set feeder time to drop cat food.
                lcdResetDisplay();

                __delay_cycles(10000);

                lcdPrint(" Feeder set to:");
                lcdNextLine();
                printTime(uart_input);

                __delay_cycles(3000000);

                lcdResetDisplay();
                uart_input_index = 0;   // Put array pointer to first position.
                uart_input[0] = 's';    // Change the first element of uart_input so that the code doesn't loop inside this if block.
                configFeeder = false;   // Reset configTime flag.
            }
        }


        else if (uart_input[0] == 'O' || uart_input[0] == 'o'){
            printOptions();             // Show feeder's options.
            uart_input_index = 0;       // Put array pointer to first position.
            uart_input[0] = 's';        // Change the first element of uart_input so that the code doesn't loop inside this if block.
        }


        else if (uart_input[0] == 'I' || uart_input[0] == 'i'){
            showTime = false;
            if (configOpening == false){   // Condition to halt the LCD, to keep the message in it while the user is configuring the opening.
                printConfigOpening();      // Print a message to show that the opening is being configured.
                configOpening = true;      // Set configOpening flag to true.
            }
            if (uart_input[1] == 'M' || uart_input[1] == 'm'){
                if (configOpeningMagnitude == false){   // Condition to halt the LCD, to keep the message in it while the user is configuring the opening magnitude.
                    printConfigOpeningMagnitude();      // Print a message to show that the opening magnitude is being configured.
                    configOpeningMagnitude = true;      // Set configOpeningMagnitude flag to true.
                }
                if (uart_input_index == 4){
                    opening = adjustOpening(uart_input);          // Adjust the magnitude of the opening.

                    lcdResetDisplay();

                    __delay_cycles(10000);

                    lcdPrint("   Opening");
                    lcdNextLine();
                    lcdPrint("   magnitude");

                    __delay_cycles(500000);

                    lcdResetDisplay();

                    __delay_cycles(10000);

                    lcdPrint("    set to");
                    lcdNextLine();
                    printOpeningMagnitude(uart_input);

                    __delay_cycles(3000000);

                    lcdResetDisplay();
                    uart_input_index = 0;               // Put array pointer to first position.
                    uart_input[0] = 's';                // Change the first element of uart_input so that the code doesn't loop inside this if block.
                    configOpening = false;              // Reset configOpening flag.
                    configOpeningMagnitude = false;     // Reset configOpeningMagnitude flag.
                    showTime = true;
                }
            }


            else if (uart_input[1] == 'T' || uart_input[1] == 't'){
                if (configOpeningTime == false){        // Condition to halt the LCD, to keep the message in it while the user is configuring the opening time.
                    printConfigOpeningTime();           // Print a message to show that the opening time is being configured.
                    configOpeningTime = true;           // Set configOpeningTime flag to true.
                }
                if (uart_input_index == 3){
                    int siz = strlen(uart_input);
                    strcpy(openingArray, uart_input);
                    adjustOpening(openingArray);        // Adjust the time that the feeder remains open.

                    lcdResetDisplay();

                    __delay_cycles(10000);

                    lcdPrint("   Opening");
                    lcdNextLine();
                    lcdPrint("     time");

                    __delay_cycles(500000);

                    lcdResetDisplay();

                    __delay_cycles(10000);

                    lcdPrint("   set to");
                    lcdNextLine();
                    printOpeningTime(uart_input);

                    __delay_cycles(3000000);

                    lcdResetDisplay();
                    uart_input_index = 0;               // Put array pointer to first position.
                    uart_input[0] = 's';                // Change the first element of uart_input so that the code doesn't loop inside this if block.
                    configOpening = false;              // Reset configOpening flag.
                    configOpeningTime = false;          // Reset configOpeningTime flag.
                    showTime = true;
                }
            }
        }
    }
}

// Function to set the feeder's time.
void setTime(char vector[6]){
    // Day, hour and minute char strings
    char day[2];
    char hour[3];
    char minute[3];

    // Get vector values corresponding to day, hour and minute and put them into their strings.
    day[0] = vector[1];
    day[1] = '\0';
    hour[0] = vector[2];
    hour[1] = vector[3];
    hour[2] = '\0';
    minute[0] = vector[4];
    minute[1] = vector[5];
    minute[2] = '\0';

    // Convert the strings into integers.
    int d = atoi(day);
    int h = atoi(hour);
    int m = atoi(minute);

    // Set the RTC day, hour and minute.
    RTCSEC = 0;
    RTCDOW = d;
    RTCHOUR = h;
    RTCMIN = m;
}

// Function to set the feeder's time to work.
void setAlarm(char vector[5]){
    // Reset the RTC alarm hour and minute registers.
    RTCAHOUR = 0;
    RTCAMIN = 0;

    // Hour and minute char strings
    char hour[3];
    char minute[3];

    // Get vector values corresponding to hour and minute and put them into their strings.
    hour[0] = vector[1];
    hour[1] = vector[2];
    hour[2] = '\0';
    minute[0] = vector[3];
    minute[1] = vector[4];
    minute[2] = '\0';

    // Convert the strings into integers.
    int h = atoi(hour);
    int m = atoi(minute);

    // Set the RTC alarm hour and minute.
    RTCAHOUR = RTCAE | h;
    RTCAMIN = RTCAE | m;
}

// Check if the command is valid.
int checkCommand(char vector[5]){
    char commands[] = "hHaAoOiIsS";
    char* compare = strchr(commands, vector[0]);

    // Valid command
    if (compare != NULL){
        return 1;
    }
    // Invalid command
    else{
        return 0;
    }
}



// RTC interruption
#pragma vector = RTC_VECTOR
__interrupt void RTC_ISR()
{
    switch (RTCIV) {
    case RTCIV_RTCAIFG:
        servoSetPosition(opening);      // Open the feeder's door.
        adjustOpening(openingArray);    // Set the time in which the feeder's door will remain open.

        servoSetPosition(0);            // Close the feeder's door.
        break;
    default:
        break;
    }
}

// UART interruption
#pragma vector = USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
    switch(UCA1IV){
    case 0:
        break;
    case 2:
        uart_input[uart_input_index++] = UCA1RXBUF;                 // Receive the UART input and put it into the char string.
        uart_input_char = UCA1RXBUF;                                // Current UART input.
        break;
    default:
        break;
    }
}
