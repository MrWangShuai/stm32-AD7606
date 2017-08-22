/*
	* 简介：stm32探索者开发板LED驱动头文件
	* 版本：V1.1
	* 作者：Mr.WangS
	* 时间: 2017-8-3
	* 详细：定义了一些控制变量，声明了函数。
*/
#ifndef __LED_H
#define __LED_H
#include "sys.h"

/* LED 控制输出变量宏定义，可直接对变量赋值控制LED亮灭。1熄灭，0点亮 */
#define LED0 PFout(9)	// DS0
#define LED1 PFout(10)	// DS1	 

/* LED控制引脚初始化 */
void LED_Init(void);//初始化		 				    
#endif
