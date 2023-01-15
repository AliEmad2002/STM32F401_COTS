/*
 * NVIC_interface.h
 *
 *  Created on:	Aug 6, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */

#ifndef NVIC_INTERFACE_H_
#define NVIC_INTERFACE_H_

#if TARGET_ID == STM32F401x
typedef enum{
	NVIC_Interrupt_WWDG					= 0,
	NVIC_Interrupt_EXTI16_PVD			= 1,
	NVIC_Interrupt_EXTI21_TAMP_STAMP	= 2,
	NVIC_Interrupt_EXTI22_RTC_WKUP		= 3,
	NVIC_Interrupt_FLASH				= 4,
	NVIC_Interrupt_RCC					= 5,
	NVIC_Interrupt_EXTI0				= 6,
	NVIC_Interrupt_EXTI1				= 7,
	NVIC_Interrupt_EXTI2				= 8,
	NVIC_Interrupt_EXTI3				= 9,
	NVIC_Interrupt_EXTI4				= 10,
	NVIC_Interrupt_DMA1_Stream0			= 11,
	NVIC_Interrupt_DMA1_Stream1			= 12,
	NVIC_Interrupt_DMA1_Stream2			= 13,
	NVIC_Interrupt_DMA1_Stream3			= 14,
	NVIC_Interrupt_DMA1_Stream4			= 15,
	NVIC_Interrupt_DMA1_Stream5			= 16,
	NVIC_Interrupt_DMA1_Stream6			= 17,
	NVIC_Interrupt_ADC					= 18,
	NVIC_Interrupt_EXTI5To9				= 23,
	NVIC_Interrupt_TIM1BRK_TIM9			= 24,
	NVIC_Interrupt_TIM1UP_TIM10			= 25,
	NVIC_Interrupt_TIM1TRG_COM_TIM11	= 26,
	NVIC_Interrupt_TIM1CC				= 27,
	NVIC_Interrupt_TIM2					= 28,
	NVIC_Interrupt_TIM3					= 29,
	NVIC_Interrupt_TIM4					= 30,
	NVIC_Interrupt_I2C1EV				= 31,
	NVIC_Interrupt_I2C1ER				= 32,
	NVIC_Interrupt_I2C2EV				= 33,
	NVIC_Interrupt_I2C2ER				= 34,
	NVIC_Interrupt_SPI1					= 35,
	NVIC_Interrupt_SPI2					= 36,
	NVIC_Interrupt_USART1				= 37,
	NVIC_Interrupt_USART2				= 38,
	NVIC_Interrupt_EXTI10To15			= 40,
	NVIC_Interrupt_EXTI17_RTCalarm		= 41,
	NVIC_Interrupt_EXTI18_OTGWKUP		= 42,
	NVIC_Interrupt_DMA1_Stream7			= 47,
	NVIC_Interrupt_SDIO					= 49,
	NVIC_Interrupt_TIM5					= 50,
	NVIC_Interrupt_SPI3					= 51,
	NVIC_Interrupt_DMA2_Stream0			= 56,
	NVIC_Interrupt_DMA2_Stream1			= 57,
	NVIC_Interrupt_DMA2_Stream2			= 58,
	NVIC_Interrupt_DMA2_Stream3			= 59,
	NVIC_Interrupt_DMA2_Stream4			= 60,
	NVIC_Interrupt_OTGFS				= 67,
	NVIC_Interrupt_DMA2_Stream5			= 68,
	NVIC_Interrupt_DMA2_Stream6			= 69,
	NVIC_Interrupt_DMA2_Stream7			= 70,
	NVIC_Interrupt_USART6				= 71,
	NVIC_Interrupt_I2C3EV				= 72,
	NVIC_Interrupt_I2C3ER				= 73,
	NVIC_Interrupt_FPU					= 81,
	NVIC_Interrupt_SPI4					= 84,
	/*	CORE	*/
	NVIC_Interrupt_HardFault,
	NVIC_Interrupt_MemManage,
	NVIC_Interrupt_BusFault,
	NVIC_Interrupt_UsageFault,
	NVIC_Interrupt_SVCall,
	NVIC_Interrupt_Debug_Monitor,
	NVIC_Interrupt_PendSV,
	NVIC_Interrupt_Systick
}NVIC_Interrupt_t;

#elif TARGET_ID == STM32F10x
typedef enum{
	NVIC_Interrupt_WWDG				,
	NVIC_Interrupt_PVD				,
	NVIC_Interrupt_Tamper           ,
	NVIC_Interrupt_RTC              ,
	NVIC_Interrupt_Flash            ,
	NVIC_Interrupt_RCC              ,
	NVIC_Interrupt_EXTI0            ,
	NVIC_Interrupt_EXTI1            ,
	NVIC_Interrupt_EXTI2            ,
	NVIC_Interrupt_EXTI3            ,
	NVIC_Interrupt_EXTI4            ,
	NVIC_Interrupt_DMA1_Ch1         ,
	NVIC_Interrupt_DMA1_Ch2         ,
	NVIC_Interrupt_DMA1_Ch3         ,
	NVIC_Interrupt_DMA1_Ch4         ,
	NVIC_Interrupt_DMA1_Ch5         ,
	NVIC_Interrupt_DMA1_Ch6         ,
	NVIC_Interrupt_DMA1_Ch7         ,
	NVIC_Interrupt_ADC1_2           ,
	NVIC_Interrupt_USBHP_CANTx      ,
	NVIC_Interrupt_USBLP_CAN1Rx0    ,
	NVIC_Interrupt_CAN_Rx1          ,
	NVIC_Interrupt_CAN_SCE          ,
	NVIC_Interrupt_EXTI5To9         ,
	NVIC_Interrupt_TIM1BRK          ,
	NVIC_Interrupt_TIM1UP           ,
	NVIC_Interrupt_TIM1TRG_COM      ,
	NVIC_Interrupt_TIM1CC           ,
	NVIC_Interrupt_TIM2             ,
	NVIC_Interrupt_TIM3             ,
	NVIC_Interrupt_TIM4             ,
	NVIC_Interrupt_I2C1EV           ,
	NVIC_Interrupt_I2C1ER           ,
	NVIC_Interrupt_I2C2EV           ,
	NVIC_Interrupt_I2C2ER           ,
	NVIC_Interrupt_SPI1             ,
	NVIC_Interrupt_SPI2             ,
	NVIC_Interrupt_USART1           ,
	NVIC_Interrupt_USART2           ,
	NVIC_Interrupt_USART3           ,
	NVIC_Interrupt_EXTI10To15       ,
	NVIC_Interrupt_RTC_Alarm        ,
	NVIC_Interrupt_USBWakeup        ,
	NVIC_Interrupt_TIM8BRK          ,
	NVIC_Interrupt_TIM8UP           ,
	NVIC_Interrupt_TIM8TRG_COM      ,
	NVIC_Interrupt_TIM8CC           ,
	NVIC_Interrupt_ADC3             ,
	NVIC_Interrupt_FSMC             ,
	NVIC_Interrupt_SDIO             ,
	NVIC_Interrupt_TIM5             ,
	NVIC_Interrupt_SPI3             ,
	NVIC_Interrupt_UART4            ,
	NVIC_Interrupt_UART5            ,
	NVIC_Interrupt_TIM6             ,
	NVIC_Interrupt_TIM7             ,
	NVIC_Interrupt_DMA2_Ch1         ,
	NVIC_Interrupt_DMA2_Ch2         ,
	NVIC_Interrupt_DMA2_Ch3         ,
	NVIC_Interrupt_DMA2_Ch4And5     ,
	/*	CORE	*/
	NVIC_Interrupt_HardFault,
	NVIC_Interrupt_MemManage,
	NVIC_Interrupt_BusFault,
	NVIC_Interrupt_UsageFault,
	NVIC_Interrupt_SVCall,
	NVIC_Interrupt_Debug_Monitor,
	NVIC_Interrupt_PendSV,
	NVIC_Interrupt_Systick
}NVIC_Interrupt_t;
#endif

void NVIC_voidEnableInterrupt(NVIC_Interrupt_t interrupt);

void NVIC_voidDisableInterrupt(NVIC_Interrupt_t interrupt);

void NVIC_voidSetPendingFlag(NVIC_Interrupt_t interrupt);

void NVIC_voidClearPendingFlag(NVIC_Interrupt_t interrupt);

b8 NVIC_b8GetActiveFlag(NVIC_Interrupt_t interrupt);

void NVIC_voidSetInterruptPriority(volatile NVIC_Interrupt_t interrupt, u8 group, u8 sub);

#endif /* NVIC_INTERFACE_H_ */
