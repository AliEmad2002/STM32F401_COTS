/*
 * UART_program.c
 *
 *  Created on:	Sep 18, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */

/*	LIB	*/
#include <math.h>
#include "Std_Types.h"
#include "Bit_Math.h"
#include "Target_config.h"
#include "Error_Handler_interface.h"
#include "Delay_interface.h"

/*	MCAL	*/
#include "RCC_interface.h"
#include "STK_interface.h"

/*	SELF	*/
#include "UART_private.h"
#include "UART_interface.h"
#include "UART_config.h"


#ifndef NULL
#define NULL (void*)0
#endif

typedef struct{
	void (*RXNE)(void);
}callBacks_t;

callBacks_t callBacksArr[3];

void UART_voidInit(
	UART_UnitNumber_t UARTn,
	#if TARGET_ID == STM32F401x
	UART_Mode_t mode, UART_OverSamplingMode_t over8,
	#endif
	u32 baudRate, b8 RxEn, b8 TxEn, UART_WordLength_t wordLen,
	UART_StopBits_t stopBits
	)
{
	volatile UART_t* UARTxPtr = UART[UARTn];
	/*	=========================================	*/
	/*	===============	SET DIVISOR	=============	*/
	/*	=========================================	*/
	/*
	 * since:
	 * 	baudRate = F_cpu / (8 * (2 - over8) * USARTDIV)
	 * then:
	 * 	USARTDIV = F_cpu / (8 * (2 - over8) * baudRate)
	 * note:
	 * 	the 8 in the previous formula is replaced by 16 when mode is "UART_mode_other".
	 *
	 * and:
	 * 	DIV_fraction = USARTDIV fraction multiplied by divisor's value.
	 * 	DIV_mantissa = USARTDIV mantissa.
	 * note:
	 * 	DIV_fraction[0:3] OVF resultant carry should be added to the DIV_mantissa.
	 */

	#if TARGET_ID == STM32F401x
	const RCC_Bus_t busArr[] = {RCC_Bus_APB2, RCC_Bus_APB1, RCC_Bus_APB2};
	#elif TARGET_ID == STM32F10x
	const RCC_Bus_t busArr[] = {RCC_Bus_APB2, RCC_Bus_APB1, RCC_Bus_APB1, RCC_Bus_APB1, RCC_Bus_APB1};
	#endif

	RCC_Bus_t bus = busArr[UARTn];

	#if TARGET_ID == STM32F401x
	f32 usartDiv = (f32)RCC_u32GetBusClk(bus) / (f32)(8 * (u32)(2 - over8) * baudRate);
	u8 divisor = (mode == UART_Mode_std) ? 8 : 16;
	#elif TARGET_ID == STM32F10x
	f32 usartDiv = (f32)RCC_u32GetBusClk(bus) / (f32)(16 * baudRate);
	u8 divisor = 16;
	#endif


	u32 divMantessa = (u32)usartDiv;
	u8 divFrac = round((usartDiv - (f32)divMantessa) * (f32)divisor);
	if (divFrac == 16)
	{
		divFrac = 15;
		divMantessa++;
	}
	#if TARGET_ID == STM32F401x
	WRT_BIT(UARTxPtr->CR1, 15, over8);
	#endif

	UARTxPtr->BRR = (u16)(divFrac | (divMantessa << 4));	// (casted to u16 to protect reset values of reserved bits)

	/*	=====================================================	*/
	/*	===============	SET Rx AND Tx SETTINGS	=============	*/
	/*	=====================================================	*/
	if (RxEn)
	{
		UART_voidSetReceiverActive(UARTn);
		SET_BIT(UARTxPtr->CR1, 2);
	}
	else
		CLR_BIT(UARTxPtr->CR1, 2);

	if (TxEn)
		SET_BIT(UARTxPtr->CR1, 3);
	else
		CLR_BIT(UARTxPtr->CR1, 3);

	/*	=============================================	*/
	/*	===============	SET WORD LENGTH	=============	*/
	/*	=============================================	*/
	UART_voidSetWordLength(UARTn, wordLen);

	/*	=============================================	*/
	/*	===============	SET STOP BITS	=============	*/
	/*	=============================================	*/
	UART_voidSetStopBits(UARTn, stopBits);

	/*	=========================================	*/
	/*	===============	ENABLE HW	=============	*/
	/*	=========================================	*/
	UART_voidUSARTEnable(UARTn);
}

void UART_voidFastInit(UART_UnitNumber_t UARTn, u32 baudRate)
{
	UART_voidInit(
		UARTn,
		#if TARGET_ID == STM32F401x
		UART_Mode_std, UART_OverSamplingMode_by8,
		#endif
		baudRate, true, true, UART_WordLength_8, UART_StopBits_one
		);
}

void UART_voidSetReceiverActive(UART_UnitNumber_t UARTn)
{
	CLR_BIT(UART[UARTn]->CR1, 1);
}

void UART_voidSetReceiverMute(UART_UnitNumber_t UARTn)
{
	SET_BIT(UART[UARTn]->CR1, 1);
}

void UART_voidSetWordLength(UART_UnitNumber_t UARTn, UART_WordLength_t wordLen)
{
	if (wordLen == UART_WordLength_8)
		CLR_BIT(UART[UARTn]->CR1, 12);
	else
		SET_BIT(UART[UARTn]->CR1, 12);
}

void UART_voidUSARTEnable(UART_UnitNumber_t UARTn)
{
	SET_BIT(UART[UARTn]->CR1, 13);
}

void UART_voidUSARTDisable(UART_UnitNumber_t UARTn)
{
	CLR_BIT(UART[UARTn]->CR1, 13);
}

void UART_voidSetStopBits(UART_UnitNumber_t UARTn, UART_StopBits_t sb)
{
	EDT_REG(UART[UARTn]->CR2 ,12, sb, 2);
}

UART_Error_t UART_enumReciveByte(UART_UnitNumber_t UARTn, char* bytePtr)
{
	/*	wait for read data register to be empty (data to be received)	*/
	while (!GET_BIT(UART[UARTn]->SR, 5));

	*bytePtr = UART[UARTn]->DR;

	return 0;
}

UART_Error_t UART_enumSendByte(UART_UnitNumber_t UARTn, char byte)
{
	/*	wait for previous transmission complete - if any -	*/
	while (!GET_BIT(UART[UARTn]->SR, 6));

	/*	load byte to data register to be transmitted	*/
	UART[UARTn]->DR = byte;

	return 0;
}

/*	sends byte and validates echo	*/
void UART_voidSendByteEcho(UART_UnitNumber_t UARTn, char byte)
{
	/*	wait for previous transmission complete - if any -	*/
	while (!GET_BIT(UART[UARTn]->SR, 6));

	/*	load byte to data register to be transmitted	*/
	UART[UARTn]->DR = byte;

	/*	wait for read data register to be empty (data to be received)	*/
	while (!GET_BIT(UART[UARTn]->SR, 5));

	/*	receive echo	*/
	char echo = UART[UARTn]->DR;

	/*	validate	*/
	if (echo != byte)
	{
		UART_NON_VALID_ECHO_HANDLER;
	}
}

/*	sends a string and validates echo of each byte	*/
void UART_voidSendStringEcho(UART_UnitNumber_t UARTn, char* str)
{
	for (u16 i = 0; str[i] != 0; i++)
	{
		UART_voidSendByteEcho(UARTn, str[i]);
	}
}

void UART_voidFlushDataReceiveRegister(UART_UnitNumber_t UARTn)
{
	volatile char temp;
	/*	applying overrun error flag clear sequence	*/
	/*	reading SR register	*/
	temp = UART[UARTn]->SR;
	/*	reading DR register	*/
	temp += UART[UARTn]->DR;
}

/*	(sysTick must be enabled and started tick measure)	*/
b8 UART_b8ReceiveByteTimeout(UART_UnitNumber_t UARTn, char* bytePtr, u16 msTimeout)
{
	volatile u64 startTime = STK_u64GetElapsedTicks();
	volatile u64 lastTimeStamp = startTime;
	volatile u64 tickInterval = ((u64)STK_u32GetTicksPerSecond() * (u64)msTimeout) / 1000;

	while(lastTimeStamp - startTime < tickInterval)
	{
		if (GET_BIT(UART[UARTn]->SR, 5))
		{
			*bytePtr = UART[UARTn]->DR;
			return true;
		}
		lastTimeStamp = STK_u64GetElapsedTicks();
	}

	return false;
}

b8 UART_b8ReceiveStringTimeout(UART_UnitNumber_t UARTn, char* str, u32 msTimeout, char* terminatorStr)
{
	/*	init timing variables	*/
	volatile u64 startTime = STK_u64GetElapsedTicks();
	volatile u64 lastTimeStamp = startTime;
	volatile u64 tickInterval = ((u64)STK_u32GetTicksPerSecond() * (u64)msTimeout) / 1000;

	u16 i = 0;
	u16 j = 0;
	while(lastTimeStamp - startTime < tickInterval)
	{
		if (GET_BIT(UART[UARTn]->SR, 5))
		{
			/*	on receive, concatenate to str	*/
			str[i] = UART[UARTn]->DR;

			/*	compare just received byte with terminatorStr[j]	*/
			/*	if they match	*/
			if (str[i] == terminatorStr[j])
			{
				/*	if the last byte in terminatorStr was matched	*/
				if (terminatorStr[j+1] == '\0')
				{
					str[++i] = '\0';
					return true;
				}
				/*	else if not yet	*/
				else
					j++;
			}
			/*	else if they don't	*/
			else
			{
				/*	will start comparison sequence again	*/
				j = 0;
			}
			i++;
		}
		lastTimeStamp = STK_u64GetElapsedTicks();
	}

	return false;
}

void UART_voidSendString(UART_UnitNumber_t UARTn, char* str)
{
	for (u16 i = 0; str[i] != 0; i++)
	{
		/*	wait for previous transmission complete - if any -	*/
		while (!GET_BIT(UART[UARTn]->SR, 6));

		/*	load byte to data register to be transmitted	*/
		UART[UARTn]->DR = str[i];
	}
}

u16 UART_u16ReadString(UART_UnitNumber_t UARTn, char* str, char terminator, u16 terminatorCount)
{
	u16 i = 0;
	while(1)
	{
		UART_enumReciveByte(UARTn, str + i);

		if(str[i] == terminator)
		{
			terminatorCount--;
			if (terminatorCount == 0)
			{
				str[++i] = '\0';
				return i;
			}
		}

		i++;
	}
}

void UART_voidEnableInterrupt(UART_UnitNumber_t UARTn, UART_Interrupt_t interrupt)
{
	switch(interrupt)
	{
	case UART_Interrupt_RXNE:
		SET_BIT(UART[UARTn]->CR1, 5);
		break;
	}
}

void  UART_voidSetCallBack(UART_UnitNumber_t UARTn, UART_Interrupt_t interrupt, void(*callBack)(void))
{
	switch(interrupt)
	{
	case UART_Interrupt_RXNE:
		callBacksArr[UARTn].RXNE = callBack;
		break;
	}
}

void USART1_IRQHandler(void)
{
	/*	search by SW for the event source	*/
	if ((UART1->SR) & ((1<<3) | (1<<5)))
	{
		callBacksArr[0].RXNE();
		/*	clearing sequence	*/
		(void)UART1->SR;
		(void)UART1->DR;
	}
}














