#include "ad7606.h"

/* ¸´Î»±äÁ¿ºê¶¨Òå */
#define AD_RST PBout(8)

/* ¹ý²ÉÑùÂÊ¿ØÖÆÒý½ÅOS0ºê¶¨Òå */
#define AD_OS0 PCout(0)

/* ¹ý²ÉÑùÂÊ¿ØÖÆÒý½ÅOS0£¬OS1£¬OS2ºê¶¨Òå*/
#define AD_OS1 PCout(1)
#define AD_OS2 PCout(2) 

/* ÉèÖÃAD²ÉÑù·¶Î§¡£0Îª-5V~5V¡£1Îª-10V~10V¡£*/
#define AD_RANGE PCout(3)
/* SPIÍ¨ÐÅ Æ¬Ñ¡ÐÅºÅ*/
#define AD_CS PBout(9)
//#define AD_IRQ_FLAG PCout(6)
/*
 *¼ò½é£º³õÊ¼»¯ADÄ£¿é¸÷¿ØÖÆÒý½Å
 *²ÎÊý£ºÎÞ
*/
void ad7606_gpio_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOB, ENABLE);//Ê¹ÄÜGPIOCÊ±ÖÓ

  //PC0,PC1,PC2Òý½Å³õÊ¼»¯£¬ÓÃÓÚ¿ØÖÆ¹ý²ÉÑù±¶Êý¡£OS0£¬OS1£¬OS2
	//³õÊ¼»¯ÉèÖÃÎªÏÂÀ­£¬Ä¬ÈÏ²ÉÑù±¶ÊýÎª0.
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2|GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//ÆÕÍ¨Êä³öÄ£Ê½
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//ÍÆÍìÊä³ö
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//ÏÂÀ­
  GPIO_Init(GPIOC, &GPIO_InitStructure);//³õÊ¼»¯GPIO
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2|GPIO_Pin_6);//Èý¸öÒý½ÅÔÙ´ÎÉèÖÃµÍÎ»Êä³ö
	
	//PC3Òý½Å³õÊ¼»¯ÉèÖÃ£¬ÓÃÓÚ¿ØÖÆ²ÉÑù·¶Î§£¬Ä¬ÈÏÎªÉÏÀ­£¬10V¡£
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//¶ÔÓ¦RANGEÒý½Å
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//ÆÕÍ¨Êä³öÄ£Ê½
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//ÍÆÍìÊä³ö
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//ÉÏÀ­
  GPIO_Init(GPIOC, &GPIO_InitStructure);//³õÊ¼»¯GPIO
	
	GPIO_SetBits(GPIOC,GPIO_Pin_3);
	
	//PB8Òý½Å³õÊ¼»¯¡£¿ØÖÆADÄ£¿é¸´Î»ÖØÆô¡£Ä¬ÈÏÀ­µÍ¡£PB9ÎªSPI1Æ¬Ñ¡ÐÅºÅ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;//¶ÔÓ¦RSTÒý½Å
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//ÆÕÍ¨Êä³öÄ£Ê½
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//ÍÆÍìÊä³ö
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//ÏÂÀ­
  GPIO_Init(GPIOB, &GPIO_InitStructure);//³õÊ¼»¯GPIO
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
}

/*
 *¼ò½é£º³õÊ¼»¯PWM²¨Êä³öÒý½Å£¬¿ØÖÆA/DÐ¾Æ¬²ÉÑù   Êä³öÒý½ÅÎªTIM4µÄCH1ºÍCH2£¬¶ÔÓ¦PB6ºÍPB7Òý½Å
 *ÊäÈë²ÎÊý:arr×Ô¶¯ÖØ×°Öµ
 *ÊäÈë²ÎÊý:psc¡£¶¨Ê±Æ÷Ê±ÖÓÎª84MHz£¬·ÖÆµÏµÊýÒ»°ãÉè¶¨Îª84£¬¼ÆÊýÆµÂÊÎª1MHz,Éè¶¨ÖØ×°ÔÚÖµÎªarr,ÔòPWMµÄÆµÂÊÎª1MHz/arr.arr=50µÄÊ±ºòÎª20KHz¡£
*/
void ad7606_pwm_init(u32 arr,u32 psc)
{		 					 
	//´Ë²¿·ÖÐèÊÖ¶¯ÐÞ¸ÄIO¿ÚÉèÖÃ
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//TIM14Ê±ÖÓÊ¹ÄÜ    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//Ê¹ÄÜPORTFÊ±ÖÓ	
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4); //GPIOB6¸´ÓÃÎª¶¨Ê±Æ÷14
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIOB6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //¸´ÓÃ¹¦ÄÜ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//ËÙ¶È100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //ÉÏÀ­
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //³õÊ¼»¯PB6
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //ÏòÉÏ¼ÆÊýÄ£Ê½
	TIM_TimeBaseStructure.TIM_Period=arr;   //×Ô¶¯ÖØ×°ÔØÖµ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//³õÊ¼»¯¶¨Ê±Æ÷14
	
	//³õÊ¼»¯TIM14 Channel1 PWMÄ£Ê½	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //Ñ¡Ôñ¶¨Ê±Æ÷Ä£Ê½:TIMÂö³å¿í¶Èµ÷ÖÆÄ£Ê½2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //±È½ÏÊä³öÊ¹ÄÜ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //Êä³ö¼«ÐÔ:TIMÊä³ö±È½Ï¼«ÐÔµÍ
	
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //¸ù¾ÝTÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉèTIM1 4OC1
	
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //Ê¹ÄÜTIM14ÔÚCCR1ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷
 

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4); //GPIOF9¸´ÓÃÎª¶¨Ê±Æ÷14
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;           //GPIOB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //¸´ÓÃ¹¦ÄÜ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//ËÙ¶È100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //ÉÏÀ­
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //³õÊ¼»¯PF9
	
	//³õÊ¼»¯TIM14 Channel1 PWMÄ£Ê½	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //Ñ¡Ôñ¶¨Ê±Æ÷Ä£Ê½:TIMÂö³å¿í¶Èµ÷ÖÆÄ£Ê½2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //±È½ÏÊä³öÊ¹ÄÜ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //Êä³ö¼«ÐÔ:TIMÊä³ö±È½Ï¼«ÐÔµÍ
	
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //¸ù¾ÝTÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉèTIM1 4OC1
	
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //Ê¹ÄÜTIM14ÔÚCCR1ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷
  
	TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPEÊ¹ÄÜ 
	
	TIM_Cmd(TIM4, ENABLE);  //Ê¹ÄÜTIM14		
}  

/*
 *¼ò½é£ºÐ¾Æ¬ÖØÆôÒý½ÅRSTÉèÖÃ¡£RSTÀ­¸ßÖÁÉÙ50ns£¬È»ºóÀ­µÍ
 *²ÎÊý£ºÎÞ
*/
void ad7606_rst(void)
{
	AD_RST=1;
	delay_us(1);
	AD_RST=0;
}

/*
 *¼ò½é£ºADÄ£¿é²ÉÑùÂÊÉèÖÃ
 *²ÎÊý:mul_os.ÄãÏëÒªÉèÖÃµÄ¹ý²ÉÑù±¶Êý£¬0.2,4,8,16,32,64¡£Èç¹ûÊäÈëµÄ²»ÊÇÕâÐ©ÊýÖÐµÄ£¬¾ÍÄ¬ÈÏÎª0¡£
*/
void ad7606_os_set(u8 mul_os)
{
		u16 num_os=0;
		switch(mul_os)
		{
			case 0:	num_os = 0x000;
							break;
			case 2:	num_os = 0x001;
							break;
			case 4:	num_os = 0x010;
							break;
			case 8:	num_os = 0x011;
							break;
			case 16:num_os = 0x100;
							break;
			case 32:num_os = 0x101;
							break;
			case 64:num_os = 0x110;
							break;
			default:num_os = 0x000;
							break;
		}
		AD_OS0 = num_os&0x001;
		AD_OS1 = (num_os>>4)&0x001;
		AD_OS2 = (num_os>>8)&0x001;	
}

/*
 *¼ò½é£ºÉèÖÃADÄ£¿éµÄ²ÉÑù·¶Î§.
 *²ÎÊý£ºnum_range,·¶Î§Éè¶¨Öµ¡£5£¬ÉèÖÃ²ÎÊý·¶Î§Îª-5V~5V.10,²ÎÊý·¶Î§Îª-10V~10V¡£Ä¬ÈÏÎª10¡£
*/
void ad7606_range_set(u8 num_range)
{
	if (num_range == 5){
		AD_RANGE = 0;
	} else AD_RANGE = 1;
}
/*
 *¼ò½é£º¿ª·¢°åSPI1³õÊ¼»¯£¬ÓÃÓÚºÍADÄ£¿éÍ¨ÐÅ¡£SPIÉèÖÃ£¬CPHA=0£¬ÔÚSCKµÚÒ»¸öÌõ±ßÑØ²ÉÑùÊý¾Ý¡£CPOL=1£¬¿ÕÏÐ×´Ì¬£¬¸ßµãÆ½¡£
 *²ÎÊý£ºÎÞ
*/
void spi1_init(void)
{	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//Ê¹ÄÜGPIOBÊ±ÖÓ
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);//Ê¹ÄÜSPI1Ê±ÖÓ
 
  //GPIOFB3,4,5³õÊ¼»¯ÉèÖÃ
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;//PB3~5¸´ÓÃ¹¦ÄÜÊä³ö	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//¸´ÓÃ¹¦ÄÜ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//ÍÆÍìÊä³ö
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//ÉÏÀ­
  GPIO_Init(GPIOC, &GPIO_InitStructure);//³õÊ¼»¯
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_SPI3); //PB3¸´ÓÃÎª SPI1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_SPI3); //PB4¸´ÓÃÎª SPI1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_SPI3); //PB5¸´ÓÃÎª SPI1
 
	//ÕâÀïÖ»Õë¶ÔSPI¿Ú³õÊ¼»¯
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3,ENABLE);//¸´Î»SPI1
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3,DISABLE);//Í£Ö¹¸´Î»SPI1

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //ÉèÖÃSPIµ¥Ïò»òÕßË«ÏòµÄÊý¾ÝÄ£Ê½:SPIÉèÖÃÎªË«ÏßË«ÏòÈ«Ë«¹¤
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//ÉèÖÃSPI¹¤×÷Ä£Ê½:ÉèÖÃÎªÖ÷SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;		//ÉèÖÃSPIµÄÊý¾Ý´óÐ¡:SPI·¢ËÍ½ÓÊÕ8Î»Ö¡½á¹¹
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//´®ÐÐÍ¬²½Ê±ÖÓµÄ¿ÕÏÐ×´Ì¬Îª¸ßµçÆ½
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//´®ÐÐÍ¬²½Ê±ÖÓµÄµÚ¶þ¸öÌø±äÑØ£¨ÉÏÉý»òÏÂ½µ£©Êý¾Ý±»²ÉÑù
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSSÐÅºÅÓÉÓ²¼þ£¨NSS¹Ü½Å£©»¹ÊÇÈí¼þ£¨Ê¹ÓÃSSIÎ»£©¹ÜÀí:ÄÚ²¿NSSÐÅºÅÓÐSSIÎ»¿ØÖÆ
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;		//¶¨Òå²¨ÌØÂÊÔ¤·ÖÆµµÄÖµ:²¨ÌØÂÊÔ¤·ÖÆµÖµÎª256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//Ö¸¶¨Êý¾Ý´«Êä´ÓMSBÎ»»¹ÊÇLSBÎ»¿ªÊ¼:Êý¾Ý´«Êä´ÓMSBÎ»¿ªÊ¼
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCÖµ¼ÆËãµÄ¶àÏîÊ½
	SPI_Init(SPI3, &SPI_InitStructure);  //¸ù¾ÝSPI_InitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉèSPIx¼Ä´æÆ÷
 
  SPI_Cmd(SPI3,DISABLE);
	SPI_Cmd(SPI3, ENABLE); //Ê¹ÄÜSPIÍâÉè
} 

/*
 *SPI1ËÙ¶ÈÉèÖÃº¯Êý
 *SPIËÙ¶È=fAPB2/·ÖÆµÏµÊý
 *@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
 *fAPB2Ê±ÖÓÒ»°ãÎª84Mhz£
*/
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//ÅÐ¶ÏÓÐÐ§ÐÔ
	SPI1->CR1&=0XFFC7;//Î»3-5ÇåÁã£¬ÓÃÀ´ÉèÖÃ²¨ÌØÂÊ
	SPI1->CR1|=SPI_BaudRatePrescaler;	//ÉèÖÃSPI1ËÙ¶È 
	SPI_Cmd(SPI1,ENABLE); //Ê¹ÄÜSPI1
} 

/*
 *¼ò½é£º¶¨Ê±Æ÷5Í¨µÀ1ÊäÈë²¶»ñÅäÖÃ
 *ÊäÈë²ÎÊýarr£º×Ô¶¯ÖØ×°Öµ(TIM2,TIM5ÊÇ32Î»µÄ!!)
 *ÊäÈë²ÎÊýpsc£ºÊ±ÖÓÔ¤·ÖÆµÊ
*/
void ad7606_busy_cap(u32 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef  TIM5_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5Ê±ÖÓÊ¹ÄÜ    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//Ê¹ÄÜPORTAÊ±ÖÓ	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//¸´ÓÃ¹¦ÄÜ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//ËÙ¶È100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //ÏÂÀ­
	GPIO_Init(GPIOA,&GPIO_InitStructure); //³õÊ¼»¯PA0

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); //PA0¸´ÓÃÎ»¶¨Ê±Æ÷5
  
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //ÏòÉÏ¼ÆÊýÄ£Ê½
	TIM_TimeBaseStructure.TIM_Period=arr;   //×Ô¶¯ÖØ×°ÔØÖµ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	

	//³õÊ¼»¯TIM5ÊäÈë²¶»ñ²ÎÊý
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	Ñ¡ÔñÊäÈë¶Ë IC1Ó³Éäµ½TI1ÉÏ
  TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;	//ÉÏÉýÑØ²¶»ñ
  TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //Ó³Éäµ½TI1ÉÏ
  TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //ÅäÖÃÊäÈë·ÖÆµ,²»·ÖÆµ 
  TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ÅäÖÃÊäÈëÂË²¨Æ÷ ²»ÂË²¨
  TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);//ÔÊÐí¸üÐÂÖÐ¶Ï ,ÔÊÐíCC1IE²¶»ñÖÐ¶Ï	
	
  TIM_Cmd(TIM5,ENABLE ); 	//Ê¹ÄÜ¶¨Ê±Æ÷5

  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//ÇÀÕ¼ÓÅÏÈ¼¶2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//×ÓÓÅÏÈ¼¶2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);	//¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯VIC¼Ä´æÆ÷¡¢
	
	
}
/*
*SPI1 ¶ÁÐ´Ò»¸ö×Ö½Ú
*TxData:ÒªÐ´ÈëµÄ×Ö½Ú
*·µ»ØÖµ:¶ÁÈ¡µ½µÄ×ÖÊý
*/
u16 SPI3_Read2Byte(void)
{		 			 
	u8 retry=0;
	SPI_Cmd(SPI3, ENABLE); //Ê¹ÄÜSPIÍâÉè
	AD_CS=0;
	SPI_I2S_SendData(SPI3,0x0000);
	while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_RXNE) == RESET)
	{
		retry++;
		if(retry>200) return 0x1010;
	}
	AD_CS=1;
	SPI_Cmd(SPI3, DISABLE); //Ê¹ÄÜSPIÍâÉè
 	return SPI_I2S_ReceiveData(SPI3);	    
}
//¶¨Ê±Æ÷5ÖÐ¶Ï·þÎñ³ÌÐò	 
void TIM5_IRQHandler(void)
{ 		    
	u8 i;
	u16 data[8];
	if(TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//²¶»ñ1·¢Éú²¶»ñÊÂ¼þ
	{
		//AD_IRQ_FLAG=1;
		for(i = 0;i<8;i++)
		{
			data[i]=SPI3_Read2Byte();
		}
		for(i=0;i<8;i++){
		printf("%x´ÎµÄADÖµÎª:%x\r\n",i,data[i]);
		delay_us(1);
			}				
	}
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //Çå³ýÖÐ¶Ï±êÖ¾Î»
	//AD_IRQ_FLAG=0;
}
