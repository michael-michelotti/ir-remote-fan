# IR Remote Fan
A [STM32F407G-DISC1](https://www.st.com/en/evaluation-tools/stm32f4discovery.html) development board receives signals from a [TSOP1838 IR Receiver module](https://hobbyking.com/en_us/keyes-tsop1838-infra-red-37-9khz-receiver-for-arduino.html). That signal is processed to determine how to drive a DC fan with a PWM siganl through a [L293D push-pull driver](https://www.ti.com/product/L293D).

![IR remote fan project demo gif](Img/IR_Remote_Fan_Demo.gif)

## Table of Contents
1. [About the Project](#about-the-project)
2. [Getting Started](#getting-started)
3. [Installation](#installation)
4. [Challenges and Solutions](#challenges-and-solutions)
5. [Future Improvements](#future-improvements)

## About the Project
For this project, I wanted to practice a few things:
1. Receiving and processing a wireless digital signal.
2. Driving a load which drew a significant amount of current.
3. Development using the STM32 HAL.

For #1, the IR receiver module receives signals from an [Elegoo IR remote](https://myenterprised.com/mods/model-remodel/arduino/remote-control/) which are [NEC encoded](https://techdocs.altium.com/display/FPGA/NEC+Infrared+Transmission+Protocol). To receive and decode these signals, I wrote a NEC driver, which you can find in `Drivers/NEC/`.

For #2, IR remote commands are decoded and used to determine how to drive a DC motor through a L293D push-pull driver. You can find the L293D driver code in `Drivers/L293D`. 

For #3, I designed this project using CubeMX. The CubeMX configuration can be found in `ir-remote-fan.ioc`.

## Getting Started
### Required Hardware
* __STM32F407G-DISC1 Microcontroller__ - The main development board used for this project. [View product.](https://www.st.com/en/evaluation-tools/stm32f4discovery.html)
* __TSOP1838 IR Receiver Module__ - Receives NEC IR signal from the remote. [View product.](https://quagmirerepair.com/tsop1838-38khz-ir-receiver-sensor-module-2-5-5-5vdc.html)
* __Elegoo IR Remote__ - The IR remote control. [View product.](https://myenterprised.com/mods/model-remodel/arduino/remote-control/)
* __L293D Push-Pull Driver__ - Drives the DC motor. [View product.](https://www.st.com/en/motor-drivers/l293d.html)
* __3.3V LDO regulator__ - Regulates 5V rail to create 3.3V rail for logic. [View product.](https://www.st.com/en/motor-drivers/l293d.html)
* DC Fan.
* USB Micro-AB to USB A cable.
  * Connects the discovery board to your host PC via ST-Link.
* Breadboard
* Jumper Cables (M-M, M-F).
* Two (2) 100nF, one (1) 100uF, one (1) 4.7uF, bypass capacitors.
* 5V wall wart and breadboard barrel jack adapter.

### Schematic
You can find a schematic at `Img/IR_Remote_Schematic.png`.

### Required Software
* __STM32CubeIDE (v. 15.1)__ - The IDE used for development. [Download](https://www.st.com/en/development-tools/stm32cubeide.html).

## Installation
1. Clone the repository:
```
git clone git@github.com:michael-michelotti/ir-remote-fan.git
```

2. Open STM32CubeIDE, navigate to the repository location and select `Launch`.
3. Select `File > Import > General > Import an Existing STM32CubeMX Configuration File (.ioc)`
4. Under `STM32CubeMX .ioc file`, select `Browse...` and navigate to the `ir-remote-fan.ioc` file.
5. Select `Finish`.
6. Select the `ir-remote-fan` project from the `Project Explorer`.
7. Ensure your STM32F4-DISC1 board is connected to your host PC via USB, then click the `Debug` icon.

## Challenges and Solutions
### Fan Noise
Once I had the IR receiver and DC motor hooked up to the same circuit, I quickly realized that using a sensitive receiver with a noisy motor would pose a significant EMI challenge. Originally I had two separate power sources, my DC power supply for my 5V rail and my discovery board for my 3V rail. I also had long arching jumper wires and no bypass capacitors. Slowly, and with the help of `eevblog.com`, I refined my EMI approach to the point where the motor no longer tripped my sensor. I detailed this refinement process [on my website](https://michaelmichelotti.com/articles/dc-fan-noise-and-the-ir-receiver/).

### Robust Interrupt-Based NEC Decoding Logic
Receiving instructions from my IR remote posed a significant challenge. My IR remote uses the NEC protocol, for which the STM32F407 has no PHY. Therefore, I needed to write my own "big-banging" driver. Of course, my driver code has no way to know when a remote button has been pressed, so it won't know when to run unless I go with an interrupt-based approach. I chose to link a callback in my driver to all rising and falling edges on GPIO pin PD6, which is hooked externally to my TSOP IR receiver. The callback simply stashes each edge away in a `NEC_Device_t` struct along with a timestamp for later decoding. Once all edges of a frame have been received, the driver calls a decode function to decode the frame and store it on the `NEC_Device_t` object. 

The driver is designed to feed the `NEC_Device_t` object into one of several callbacks:
* `NEC_Full_Frame_Received_Callback`
* `NEC_Hold_Frame_Received_Callback`
* `NEC_Decode_Error_Callback`

These callbacks are externally exposed, so all any consumer of this driver must do is create a local instance of `NEC_Device_t`, initialize it, then implement these callbacks. In this case, the NEC driver consumer is the IR remote, so the callbacks are implemented in `Core/Src/remote.c`. 

## Future Improvements
### LCD GUI
A 16x2 LCD could be used to communicate the current state of the fan, confirm receipt of button presses, etc.
