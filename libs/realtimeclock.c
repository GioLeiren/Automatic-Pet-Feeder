#include <msp430.h>
#include <stdint.h>
#include "realtimeclock.h"

void configureRTC(void){
    // Configure RTC
    RTCCTL01 = RTCTEVIE | RTCAIE | RTCMODE | RTCSSEL__RT1PS;   // Enable alarm interruption, calendar mode and select clock source as output from RT1PS.

    // Default RTC and RTC alarm's day, hour, minute and seconds (RTC clock).
    RTCSEC = 0;
    RTCMIN = 0;
    RTCHOUR = 0;
    RTCDOW = 0;
    RTCAMIN = RTCAE | 1;
    RTCAHOUR = RTCAE | 0;
    RTCADOW = RTCAE | 0;

    RTCPS0CTL = RT0PSDIV_6;                         // Divide RTC0PS clock by 128.
    RTCPS1CTL = RT1SSEL_2 | RT1PSDIV_7;             // Use SMCLK as RT1PS clock source and divide it by 256.
                                                    // Now RTCSEC will increment every second.
}

