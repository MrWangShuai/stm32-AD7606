#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "LED.h"
#include "ad7606.h"

static void ad7606_init(void)
{
	ad7606_gpio_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	spi1_init();
	ad7606_pwm_init(100-1,84-1);  //PWM输出初始化
	ad7606_busy_cap(0xfffffff-1,84-1);
	TIM_SetCompare1(TIM4,20);
	ad7606_os_set(0);
	ad7606_range_set(10);
}

int main(void)
{
	delay_init(168);
	uart_init(921600);
	LED_Init();	//LED引脚初始化
	ad7606_init();
	ad7606_rst();
  while(1){
				LED0=0;
		delay_ms(1000);
		LED0=~LED0;
	}
}



