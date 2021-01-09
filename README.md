# AccidentPreventor

## Description
This repository contains the firmware for the accident preventor which is remotely controlled. 
The idea of the project is based on extension of the project made on ATmega88PA microcontroller:
```
https://github.com/justmalysa/ObstacleAvoider
```
New version of the robot will be based on communication between ATmega88PA and FRDM-KL05 platform via UART.
The new functionality will be the ability to detect an obstacle in front of the robot
and react to this event by stopping the vehicle. 
The distance measurement from the HCSR04 sensor will be shown on the display 
connected to KL05 board via the SPI serial interface.
DMA peripheral will be utilized to offload the CPU from managing the SPI transfers.
During CPU inactivity, microcontroller will be put to sleep to conserve energy.

## Authors
Justyna Małysa, Szymon Mazurek
