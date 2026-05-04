# SMV_UI_TestBench

This is the test bench for the UI board and is currently fully functional

## Transmitter

The `transmitter` folder is the code for the UI board. It uses a button class to send the appropriate message over. Each button is initialized with mode `PULLUP` and has a counter keeping track of how many times it has been pressed. This number is then sent over can every time it is updated. 

### Setup

To set up the UI Board, download all the files in the `transmitter` folder and add the `.h` to `Inc/` and the `.c` files to `Src/`. To set up the IOC configuration, you must follow the instructions in the CAN Setup megadoc (on Notion). Essentially just set up the clock and enable `CAN1`. The appropriate GPIO pins will already be initialized in `main.c`. You will also need to include the [SMV CANBus library](https://github.com/UCLA-Bruin-Supermileage/SMV_STM32_CANbus) in your project folder.

### Code Explanation

There is an array of buttons where each button has its own struct, see `button.h`. Whenever a button is clicked, the GPIO pin state is flipped and a counter is incremented. This counter is sent over CAN with the correct message corresponding to what is being sent. This message is important for the recieving boards to know what data is being sent.

## Reciever

The `reciever` folder is the code that handles all the receiving of the CAN messages. For the sake of the test bench, all the messages are assumed to be sent to the same board, however in actual use, the messages will all propogate to their respective boards. Essentially the messages are read in depending on the message type in the CAN frame and then stored in an array. The supercap and emergency stop data is being sent under the `DAQ_Button` and `Motor` message ids since these two haven't been added to the CAN library.

The actual reading of the data is a little unintuitive. Since there is an existing enum for the UI Messages defined in the CAN library, I use it to index into the recieved array. However, the way the buttons are wired is not in the same order as the messages in the enum so you will have to check what the current index is. For example, the first button is `Reverse` which has a index of `2`. This means in the `btn_dbg` array, the third value will be updated.

## Setup
Similar to the `transmitter` set up, the CANBus library files should be added and the same configuration for the MCU must be selected.

## Button → CAN Message Mapping

The following table describes how the buttons are mapped out, from left to right. 

| Message Name | Pin  | Target Board(s) | Message ID    |
|--------------|------|-----------------|---------------|
| Reverse      | PA0  | Bear_1         | Reverse       |
| Hazard       | PA1  | FC, RC         | Hazard        |
| Headlights   | PB0  | FC             | Headlights    |
| Wipers       | PC2  | FC             | Wipers        |
| Horn         | PC3  | FC             | Horn          |
| Blink_Left   | PA15 | FC, RC         | Blink_Left    |
| Blink_Right  | PB7  | FC, RC         | Blink_Right   |
| Supercap     | PB5  | Bear_1         | DAQ_Button    |
| E-stop       | PA10 | Bear_1         | Motor         |
