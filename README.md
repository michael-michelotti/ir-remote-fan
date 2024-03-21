# IR Remote Fan
A [STM32F407G-DISC1](https://www.st.com/en/evaluation-tools/stm32f4discovery.html) development board receives signals from a [TSOP1838 IR Receiver module](https://hobbyking.com/en_us/keyes-tsop1838-infra-red-37-9khz-receiver-for-arduino.html). That signal is processed to determine how to drive a DC fan with a PWM siganl through a [L293D push-pull driver](https://www.ti.com/product/L293D).

## Table of Contents
1. [About the Project](#about-the-project)
2. [Getting Started](#getting-started)
3. [Setup](#setup)
4. [Challenges and Solutions](#challenges-and-solutions)
5. [Future Improvements](#future-improvements)

## About the Project
For this project, I wanted to practice a few things:
1. Receiving and processing a wireless digital signal.
2. Driving a load which drew a significant amount of current.
3. Development using the STM32 HAL.

For #1, the IR receiver module receives signals from an [Elegoo IR remote](https://myenterprised.com/mods/model-remodel/arduino/remote-control/) which are [NEC encoded](https://techdocs.altium.com/display/FPGA/NEC+Infrared+Transmission+Protocol). To receive and decode these signals, I wrote a NEC driver, which you can find in `Drivers/NEC/`. The driver is designed to run a callback on all rising and falling edges on GPIO pin PD6. Those edges are stashed away in a `NEC_Device_t` struct for later decoding. Once all edges of a frame have been received, the driver calls a decode function to decode the frame and store it on the `NEC_Device_t` object.

For #2, once signals from the IR remote have been decoded, the command is decoded and used to determine how to drive a DC motor through a L293D push-pull driver. You can find the L293D driver code in `Drivers/L293D`. 

For #3, I designed this project using CubeMX. The CubeMX configuration can be found in the `ir-remote-fan.ioc` file. The project should be directly importable in STM32CubeIDE by using `File->New->STM32 Project from an Existing STM32CubeMX Configuration File (.ioc)`.

## Getting Started
I don't necessarily expect that anybody will be building this project in their local environment, so I won't get too into detail about the setup. I will simply list out the hardware and software I used.

### Hardware I Used
* [STM32F407G-DISC1 Microcontroller](https://www.st.com/en/evaluation-tools/stm32f4discovery.html).
<div align="left">
  <img src="https://github.com/michael-michelotti/stm-lcd-clock/blob/main/Img/stm32f4-disc1-board.jpg" alt="Demo usage of STM32 LCD clock" width="300"/>
</div>

* [TSOP1838 IR Receiver Module](https://www.analog.com/media/en/technical-documentation/data-sheets/DS3231.pdf).
<div align="left">
  <img src="https://github.com/michael-michelotti/stm-lcd-clock/blob/main/Img/ds3231-rtc-module.jpg" alt="Demo usage of STM32 LCD clock" width="300"/>
</div>

* [Elegoo IR Remote](https://www.analog.com/media/en/technical-documentation/data-sheets/DS3231.pdf).
<div align="left">
  <img src="https://github.com/michael-michelotti/stm-lcd-clock/blob/main/Img/ds3231-rtc-module.jpg" alt="Demo usage of STM32 LCD clock" width="300"/>
</div>

* [L293D Push-Pull Driver](https://www.sunfounder.com/products/lcd1602-module).
<div align="left">
  <img src="https://github.com/michael-michelotti/stm-lcd-clock/blob/main/Img/lcd-1602a-display.webp" alt="Demo usage of STM32 LCD clock" width="300"/>
</div>

* [DC Fan](https://www.sunfounder.com/products/lcd1602-module).
<div align="left">
  <img src="https://github.com/michael-michelotti/stm-lcd-clock/blob/main/Img/lcd-1602a-display.webp" alt="Demo usage of STM32 LCD clock" width="300"/>
</div>

* USB Micro-AB to USB A cable.
  * Connects the discovery board to your host PC via ST-Link.
* Breadboard
* Jumper Cables (M-M, M-F).
* Two (2) 100nF bypass capacitors.
* One (1) 100uF bypass capacitor.
* 5V wall wart and breadboard barrel jack adapter.

### Software I Used
* [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)

## Setup
***TODO: Schematic Image Here***
#### Power, Code Flashing
The STM32F4 discovery board is hooked up to my host PC via USB. Over that interface, I'm able to flash the board from STM32CubeIDE using ST-Link. 

#### Power Distribution, Ground
The 3V power rail is powered by the 3V rail on the STM32F4 discovery board.

The 5V power rail is powered by a 5V wall wart with a barrel jack connector, hooked up to my breadboard by an adapter.

All components are connected to a common ground on the breadboard.

#### TSOP1838 IR Receiver Module
The V+ pin is hooked up to the 3V power rail. The output signal pin is hooked directly to PD6 on the discovery board. A 100 nF tantalum bypass capacitor is connected across the power and ground pins. 

#### L293D Push-Pull Driver
* Vs (source power) is hooked up to the 5V power rail.
* Vss (logic power) is hooked up to the 3V power rail.
* The input signal pins are hooked up to PA15 and PA3 of the discovery board.
* The output signal pins are twisted and hooked up to the DC fan.
* The enable pin is hooked up to PA1 on the discovery board.
* 100uF electrolytic and 100nF tantalum bypass capacitors are connected across source power and ground.

## Challenges and Solutions
### Robust Interrupt-Based NEC Decoding Logic
### Fan Noise
### Power Rail Grounding


## Future Improvements
Reserved.
