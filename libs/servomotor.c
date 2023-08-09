#include <msp430.h>

#define SERVO_PIN BIT2 // Choose the PWM output pin
#define PWM_PERIOD 20000 // Set the PWM frequency to 50Hz
#define MIN_PULSE_WIDTH 600 // Set the minimum pulse width for 0 degree position
#define MAX_PULSE_WIDTH 2400 // Set the maximum pulse width for 180 degree position

void servoConfig(void) {
    // Initialize timer
    TA0CCR0 = PWM_PERIOD - 1; // Set PWM period
    TA0CCTL1 = OUTMOD_7; // Set output mode to reset/set
    TA0CCR1 = MIN_PULSE_WIDTH; // Set initial duty cycle
    TA0CTL = TASSEL_2 + MC_1; // Select SMCLK as clock source and set to up mode

    // Configure PWM pin as output
    P1DIR |= SERVO_PIN;
    P1SEL |= SERVO_PIN;
}

void servoSetPosition(unsigned int position) {
    // Calculate the duty cycle for the desired position
    unsigned int duty_cycle = MIN_PULSE_WIDTH + (MAX_PULSE_WIDTH - MIN_PULSE_WIDTH) * position / 180;

    // Update the duty cycle
    TA0CCR1 = duty_cycle;
}
