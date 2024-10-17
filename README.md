# Automatic Pet Feeder using MSP430

This repository contains the code for an automatic pet feeder, simulated with the MSP430 microcontroller. The system uses an RTC (Real-Time Clock) to schedule feeding times, a servo motor to control the feeder's opening, a Bluetooth module to communicate via UART, and an LCD to display information to the user. The user can configure the feeding schedule, feeder settings, and the duration the feeder remains open via UART commands.

## Features
* **Real-Time Clock (RTC):** Syncs the feeder's operation with the actual time.
* **Servo Motor Control:** Opens and closes the feeder at the scheduled times.
* **LCD Display:** Provides user feedback for configurations, current time, and feeder status.
* **Bluetooth Communication:** Configures the feeder via UART using a Bluetooth module.
* **User Commands:** Allows users to set feeding times, configure the feeder opening, and view current time through simple commands.

## Hardware Components
1. **MSP430 Microcontroller:** The heart of the system.
2. **Servo Motor:** Controls the feeder's gate, allowing food to dispense.
3. **RTC Module:** Maintains the current time to synchronize feeding events.
4. **LCD Screen:** Displays configurations, current time, and status messages.
5. **Bluetooth Module:** Communicates with the MSP430 using UART to receive commands.
6. **LEDs:** Indicate system states (e.g., operation, configuration, etc.).

## UART Commands
You can configure the feeder via Bluetooth using the following commands:

| Command |	Description |	Example |
| ------- | ----------- | ------- |
| `H` |	Set the current time (Day, Hour, Minute) |	`H12345` (Set Day 1, 12:34) |
| `A` |	Set the feeder alarm (Hour, Minute) |	`A1234` (Set feeder to dispense at 12:34) |
| `O` |	View feeder options |	`O` |
| `I` |	Configure feeder opening (Magnitude/Time) |	`IM45` (Set opening to 45 degrees) |
| `R` |	Cancel the current command |	`R` |

## Command Structure
* `H`: Configure the current time for the system. Example: `H12345` sets Day 1, 12:34.
* `A`: Configure the alarm time for the feeder. Example: `A1234` sets the feeder to dispense food at 12:34.
* `I`: Configure the feeder's opening magnitude and time.
  * `IM` for magnitude (degrees). Example: `IM90` sets the opening to 90 degrees.
  * `IT` for opening time (seconds). Example: `IT3` sets the feeder to stay open for 3 seconds.

## Code Structure
The main components of the project include:

* `main.c`: The main program controlling the feeder logic.
* `servomotor.c/h`: Functions to control the servo motor for opening/closing the feeder.
* `realtimeclock.c/h`: Functions to handle RTC configurations and alarms.
* `uart.c/h`: UART communication handling, allowing command input from the Bluetooth module.
* `i2c.c/h`: I2C communication for RTC.
* `lcd.c/h`: Functions to display messages and feedback on the LCD.
* `feeder.c/h`: Functions that manage the feeder's overall functionality and initialization.

## Main Loop
The system continuously checks for new UART commands and RTC alarms:

1. **Time Display:**  If not in configuration mode, the LCD displays the current time.
2. **Command Processing:**  Commands sent via UART are processed to configure the time, alarm, or feeder opening.
3. **RTC Alarm:**  When the RTC alarm triggers, the feeder opens at the specified time and remains open for a set duration before closing.

## Interrupts
* **RTC Interrupt:** Handles the feeder's opening and closing based on the configured feeding time.
* **UART Interrupt:** Receives and processes user commands sent via Bluetooth.

## Installation
1. Clone this repository:
bash
Copiar código
```
git clone https://github.com/yourusername/automatic-pet-feeder.git
```
3. Open the project in your MSP430 IDE.
4. Connect the required hardware components as per the circuit diagram.
5. Upload the code to your MSP430 microcontroller.

## Usage
1. Power on the system and wait for the initial LCD message.
2. Use a Bluetooth terminal on your phone or computer to send UART commands to the MSP430.
3. Configure the feeder time, opening magnitude, or view options using the supported commands.
4. The system will dispense food at the set times, opening the feeder for the configured duration.
