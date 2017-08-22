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

* [Introduction](#Introduction)
    * [Function](#Function)
    * [System Architecture](#System-Architecture)
* [Hardware and Software Setup](#Hardware-and-Software-Setup)
    * [Required Hardware](#Required-Hardware)
    * [Required Software](#Required-Software)
    * [Hardware Connection](#Hardware-Connection)
* [User Manual](#User-Manual)
    * [DRIVER](#DRIVER)
    * [OBJ](#OBJ)
    * [USER](#USER)
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




[0]: ./DOC/screenshots/diagram.png