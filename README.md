# Data collection project
> 2017-8-21    Mr.WangS

Machine learning is getting hotter day by day,but the problem which get machine learning used in industrial control will meet is that you have to collect enough data to train your algorithm. So I have tried to solve this problem by stm32f407zgt6 and AD7606.The uploaded code is just a few simple drivers and functional modules. In the future I will tried to use freertos and send all the date on cloud to produce a practical industrial data collector.

## Functional description
* 16-bit A/D conversion.
* Sampling range can be setted and is-10v~10v or -5v~5v.
* Sampling frequency can be setted from 1KHz to 20KHz.
* The data obtained for each sample wii be sent to the cloud by the network.
* Watchdog is used to confer that the program is running correctly.
* If you have the LED screen,the trace of date will be printed on the screen.

* [Introduction](#introduction)
    * [Function](#function)
    * [System Architecture](#system-architecture)
* [Hardware and Software Setup](#hardware-and-software-setup)
    * [Required Hardware](#required-hardware)
    * [Required Software](#required-software)
    * [Hardware Connection](#hardware-connection)
* [User Manual](#user-manual)
    * [DRIVER](#driver)
    * [OBJ](#obj)
    * [USER](#user)
## Introduction
### Function
* 16-bit A/D conversion.
* Sampling range can be setted and is-10v~10v or -5v~5v.
* Sampling frequency can be setted from 1KHz to 20KHz.
* The data obtained for each sample wii be sent to the cloud by the network.
* Watchdog is used to confer that the program is running correctly.
* If you have the LED screen,the trace of date will be printed on the screen.
### System Architecture
![system arcchiture][0]
## Hardware and Software Setup
### Required Hardware
* stm32f407zgt6
* ad7606
### Required Software
* Keil5(Some other development platforms for arm)
* Serial debugging assistant
### Hardware connection
* +5V connected to VCC +5V  
* GND connected to GND
* VIO connected to VCC +3.3V
* OS0，OS1，OS2 connected to PC0，PC1，PC2
* RANGE connected to PC3
* CVA，CVB connected to PB6
* RST connected to PB8
* CS connected to PB9
* BUSY connected to PA0
* RD/SCLK connected to PB3（SPI12-SCK）   *SPI3 SCK  PC10*
* DB7（DOUT） 接 开发板 PB4            *SPI3 MISO PC11*
* DB15 connected to GND
* FRST connected to PC9

## Use manual

### Driver
* There are some drivers for this project such as ad7606 and led.

### OBJ
* Those files which producted by system compiling.


[0]: ./DOC/screenshots/diagram.png