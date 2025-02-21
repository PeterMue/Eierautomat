Eierautomat
===========

The Eierautomat project is an automated egg vending machine designed to provide a convenient and efficient way to dispense products (egg carton). This project leverages various hardware components and sensors to ensure accurate and reliable operation. The system is built using an Arduino microcontroller and integrates multiple features to enhance user experience and maintain operational integrity.

## Features

- **Automated Egg Dispensing**: The machine can dispense items automatically after payment.
- **Coin Acceptor Integration**: Accepts coins for payment and processes them accurately.
- **Balance Reset**: Automatically resets the account balance after a specified time, allowing the user to buy multiple items and use the change money for the next item purchase since the coin acceptor will not give change back.
- **Manual Dispense Switch**: Allows manual dispensing of items through a switch (admin).
- **Total and Resettable Counters**: Keeps track of the total number of items dispensed and provides a resettable counter for easy management.
- **Error Handling**: Includes error LEDs and mechanisms to handle and indicate errors.
- **Rotary Encoder**: Utilizes a rotary encoder for menu navigation and selection.
- **LCD Display**: Displays information and menus on an LCD screen for user interaction.
- **Progress Indicator**: Shows progress on the LCD screen during operations.
- **Factory Reset**: Provides an option to reset the machine to factory settings.


## Menu 

- **Long Press**: Open Menu / Go Back
- **Normal Press**: Enter edit mode or activate the selected item
- **Control Wheel**: Navigate up/down or increase/decrese a value when in edit mode

### Structure

- **Info**
  - **Sold**: Displays the number of sold items.
  - **Reset sold**: Resets the sold items counter.
  - **Total**: Displays the total number of items (since Factory Reset).
  - **GIT Version**: The build version

- **Settings**
  - **Price**
    - **Set Price**: Allows setting the price.
    - **Range**: 0 to 2500 ct.
  - **Balance Reset**
    - **Set Delay**: Allows setting the balance reset delay.
    - **Range**: 0 to 600 s.
  - **Speed**
    - **Set Speed**: Allows setting the dispenser speed.
    - **Range**: 0.0 to 1.0.
  - **Retries**
    - **Set Attempts**: Allows setting the maximum dispense attempts.
    - **Range**: 1 to 10.
  - **Wait**
    - **Set Time**: Allows setting the removal wait time.
    - **Range**: 1000 to 10000 ms.
  - **Block**
    - **Set Current**: Allows setting the motor block current.
    - **Range**: 0.0 to 2.0 A.
  - **Inrush**
    - **Set Time**: Allows setting the motor block inrush wait time.
    - **Range**: 0 to 250 ms.
  - **Coin Config**
    - **Interval**
      - **Set Delay**: Allows setting the coin pulse max delay.
      - **Range**: 10 to 500 ms.
    - **1 Pulse**
      - **Set Value**: Allows setting the value for 1 pulse.
      - **Range**: 0 to 200 ct.
    - **2 Pulse**
      - **Set Value**: Allows setting the value for 2 pulses.
      - **Range**: 0 to 200 ct.
    - **3 Pulse**
      - **Set Value**: Allows setting the value for 3 pulses.
      - **Range**: 0 to 200 ct.
    - **4 Pulse**
      - **Set Value**: Allows setting the value for 4 pulses.
      - **Range**: 0 to 200 ct.
    - **5 Pulse**
      - **Set Value**: Allows setting the value for 5 pulses.
      - **Range**: 0 to 200 ct.
    - **6 Pulse**
      - **Set Value**: Allows setting the value for 6 pulses.
      - **Range**: 0 to 200 ct.
  - **Factory Reset**
    - **Reset**: Resets the machine to factory settings.

- **Exit**: Exits the menu.

## Hardware Components

- **Arduino**: Arduino Mega2560
- **Motordriver**: Arduino Motorshield Rev3
- **Switch Breakout Board**: Manages various switches and sensors for the machine.
- **Display Breakout Board**: Connects the LCD display and other display-related components.
- **LCD**: 16x2 LCD Display via I2C
- **Rotary Encoder**: Facilitates user input and menu navigation.
- **Coin Acceptor**: Handles coin insertion and validation.
- **Relays**: Controls various electrical components within the machine.

## Wiring and Connections

### Switch Breakout Board

```
         DISPENSER_A

         +------- REMOVAL_LED (VCC/GND)
         |    +------- EMPTY_SWITCH
         |    |    +------- MIN_LIMIT_SWITCH
         |    |    |    +------- MAX_LIMIT_SWITCH
         |    |    |    |    +------- MAX_LIMIT_SWITCH2
         |    |    |    |    |
+--------------------------------+    A1 -> PIN 23
| ::   [_|_][_|_][_|_][_|_][_|_] |    A2 -> PIN 25
| ::    A-1  A-2  A-3  A-4  A-5  |    A3 -> PIN 27
| ::                             |    A4 -> PIN 29
| ::                             |    A5 -> PIN 31
| ::    Switch Breakout Board    |
| ::                             |    B1 -> PIN 41
| ::                             |    B2 -> PIN 39
| ::    B-1  B-2  B-3  B-4  B-5  |    B3 -> PIN 37
| ::   [_|_][_|_][_|_][_|_][_|_] |    B4 -> PIN 35
+--------------------------------+    B5 -> PIN 33
         |    |    |    |    |
         |    |    |    |    +------- MAX_LIMIT_SWITCH2
         |    |    |    +------- MAX_LIMIT_SWITCH
         |    |    +------- MIN_LIMIT_SWITCH
         |    +------- EMPTY_SWITCH
         +------- REMOVAL_LED (VCC/GND)

         DISPENSER_B
```

### Display Breakout Board

```
           +------- C-1: Screw Terminal to RJ-45
           | +----- H-1: Pin Header 
           | |                      
+-----+    | |               +---+  
| ::  |    | |               | _ |  
| ::  +----------------------+|_||
| ::  [ 1 .. 8 ]              |_|| --- C-4: Screw Terminal Common GND
| ::     ......(H-1)          |_|| 
| ::                           _ |
| ::            ...(H-2)      |_||
| ::  [1 .. 8] [1 .. 6]       |_|| --- C-5: Screw Terminal External Power (+12V)
| ::  +----------------------+|_||
| ::  |   |      | |         |   |
+-----+   |      | |         +---+    
          |      | |                      
          |      | +----- C-3: Screw Terminal
          |      +----- H-2: Pin Header 
          +------- C-2: Screw Terminal
```

 * Screw Terminals
    * C-1: Display Unit Connector 
        1. VCC -[internally wired]-> VCC
        2. GND -[internally wired]-> Common GND
        3. SDA -[internally wired]-> H-1:1
        4. SCL -[internally wired]-> H-1:2
        5. ENC-CLK -[internally wired]-> H-1:3
        6. ENC-DT -[internally wired]-> H-1:4
        7. (empty) -[internally wired]-> H-1:5
        8. ENC-SW -[internally wired]-> H-1:6
    * C-2: Common GND
        1. GND
        2. GND
        3. GND
        4. GND
        5. GND
        6. GND
    * C-3: Coin Acceptor and Relais
        1. VCC
        2. COIN_ACCEPTOR_PULSE -[internally wired]-> H-2:1
        3. ERROR_LED -[internally wired]-> ERROR_LED_PIN (43)
        4. COIN_ACCEPTOR_ENABLE -[internally wired]-> COIN_ACCEPTOR_ENABLE_PIN (45)
        5. DISPENSER_A_MANUAL_SWITCH -[internally wired]-> DISPENSER_A_MANUAL_SWITCH_PIN (47)
        6. DISPENSER_B_MANUAL_SWITCH -[internally wired]-> DISPENSER_B_MANUAL_SWITCH_PIN (49)
    * C-4: Common GND
    * C-5: External Power (+12V)
* Pin Header
    * H-1: Display Unit Connector Pin Header
        1. SDA -[external jumper wire]-> SDA (20)
        2. SCL -[external jumper wire]-> SCL (21)
        3. ENC-CLK -[external jumper wire]-> ROTARY_ENC_PIN_CLK (18)
        4. ENC-DT -[external jumper wire]-> ROTARY_ENC_PIN_DT (19)
        5. (empty)
        6. ENC-SW -[external jumper wire]-> ROTARY_ENC_PIN_SW (17)
    * H-2: Aux
        1. COIN_ACCEPTOR_PULSE -[external jumper wire]-> COIN_ACCEPTOR_PULSE_PIN (2)
        2. COIN_ACCEPTOR_ENABLE (unused)
        3. ERROR_LED (unused)


### Display Unit Connection (RJ-45)

*Wire colors from EIA/TIA-568A/B patch cable.*


| Pin | Wire Color   | Signal   |
|-----|--------------|----------|
| 1   | Brown/White  | VCC      |
| 2   | Brown        | GND      |
| 3   | Blue/White   | SCL      |
| 4   | Blue         | SDA      |
| 5   | Orange/White | ENC-CLK  |
| 6   | Orange       | ENC-DT   |
| 7   | Green/White  | empty    |
| 8   | Green        | ENC-SW   |



# TODO

[x] Reset account balance after n time.
[x] Switch manual dispense
[x] Reset
[x] Total counter + resettable counter
[ ] Shielded cable (CoinAcceptor)
[x] VCC cable (relais card)
[x] Detach interrupt when coin acceptor is disabled