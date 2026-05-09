# SMV UI Board

This is the test bench for the UI board.

## Setup

This board depends on the [SMV CANbus library](https://github.com/UCLA-Bruin-Supermileage/SMV_STM32_CANbus) and uses a custom button library.

### Pin Mappings

Aside from the CAN pins, the pins should be set to GPIO input.

| Name | Pin |
|----- | ---- |
| Left | PA15 |
| Right | PB7 |
| Regen | PC13 | 
| Reverse | PA0 |
| Hazard | PA1 |
| Switch | PA4 |
| Headlights | PB0 |
| Wipers | PC2 |
| DAQ Button | PC1 |
| Horn | PC3 |
| Button | PC0 |
| CAN1_TX | PA12 |
| CAN1_RX | PA11 |
| Supercap Discharge | PB5|
| Emergency Stop | PA10 |

The following peripheral pins should be set to GPIO output

| Name | Pin |
|----- | ---- |
| Digit_1 | PD2 |
| Digit_2 | PC12 |
| Digit_3 | PC11 |
| Digit_4 | PC10 |
| BCD_A | PC5 |
| BCD_B | PC9 |
| BCD_C | PC8 |
| BCD_D | PC6 |
| DP | PA6 |

## CAN Communication

### CAN TX

The UI board broadcasts all the GPIO input pins through the button library. 

| Message | Target Board(s) | 
| ------- | ------------ |
| Left | FC, RC |
| Right | FC, RC |
| Regen | ? |
| Reverse | ? |
| Hazard | FC, RC |
| Switch | DAQ |
| Headlights | FC |
| Wipers | FC |
| DAQ Button | DAQ |
| Horn | FC |
| Button | DAQ |
| Supercap Discharge | ? |
| Emergency Stop | Safety |

### CAN RX

The board listens for RPM data from the motor controller board in order to display the speed of the car on a speedometer. It uses the DIGIT_x and BCD_x pins to do so.