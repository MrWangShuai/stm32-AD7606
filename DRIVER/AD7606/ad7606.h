/*
 * 简介：A/D转换芯片 AD7606驱动
 * 版本: V1.0
 * 作者：Mr.WangS
 * 时间：2017-8-3
 * 说明:使用PWM波的上升沿对CVA,CVB进行转换触发，转换完成后通过BUSY触发引脚中断，读取数据。\
				数据读取采用SPI的串行方式读取。在后续版本中将采用定时器的方式来进行数据采集。
*/
#ifndef AD7076_H_
#define AD7076_H_
#include "sys.h"
#include "delay.h"
#include "usart.h"

/* AD模块使用的引脚初始化函数 */
void ad7606_gpio_init(void);

/* AD模块  CVA,CVB控制PWM波输出初始化 */
void ad7606_pwm_init(u32 arr,u32 psc);

/* AD模块重启函数 */
void ad7606_rst(void);

/* 开发板SPI模块设置 */
void spi1_init(void);

/* AD模块过采样率设置 */
void ad7606_os_set(u8 mul_os);

/* AD模块采样范围设置 */
void ad7606_range_set(u8 num_range);
	
/* 输入捕获引脚PA8设置 */
void ad7606_busy_cap(u32 arr,u16 psc);

/* stm32 SPI初始化设置 */
void ad7606_spi_init(void);
#endif 
