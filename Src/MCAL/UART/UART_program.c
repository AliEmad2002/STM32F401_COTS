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
#include "BIT_MATH.h"
#include "STD_TYPES.h"
#include <math.h>

/*	MCAL	*/
#include "RCC_interface.h"

/*	SELF	*/
#include "UART_private.h"
#include "UART_interface.h"


#ifndef NULL
#define NULL (void*)0
#endif

typedef struct{
	void (*RXNE)(void);
}callBacks_t;

callBacks_t callBacksArr[6];


volatile UART_t* UART_PtrGetAddress(UART_unitNumber_t UARTn)
{
	volatile UART_t* ptr = 0;

	switch (UARTn)
	{
	case UART_unitNumber_1:
		ptr = UART1;
		break;
	case UART_unitNumber_2:
		ptr = UART2;
		break;
	case UART_unitNumber_3:
		ptr = UART3;
		break;
	case UART_unitNumber_4:
		ptr = UART4;
		break;
	case UART_unitNumber_5:
		ptr = UART5;
		break;
	case UART_unitNumber_6:
		ptr = UART6;
		break;
	}

	return ptr;
}

void UART_voidInit(
	UART_unitNumber_t UARTn, UART_mode_t mode, UART_overSamplingMode_t over8,
	u32 baudRate, b8 RxEn, b8 TxEn, UART_wordLength_t wordLen,
	UART_stopBits_t stopBits
	)
{
	volatile UART_t* UARTxPtr = UART_PtrGetAddress(UARTn);
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
	u8 divisor = (mode == UART_mode_std) ? 8 : 16;
	f32 usartDiv = (f32)RCC_u32GetSysClk() / (f32)(8 * (u32)(2 - over8) * baudRate);

	u32 divMantessa = (u32)usartDiv;
	u8 divFrac = round((usartDiv - (f32)divMantessa) * (f32)divisor);
	if (divFrac == 16)
	{
		divFrac = 15;
		divMantessa++;
	}

	WRT_BIT(UARTxPtr->CR1, 15, over8);

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

void UART_voidSetReceiverActive(UART_unitNumber_t UARTn)
{
	volatile UART_t* UARTxPtr = UART_PtrGetAddress(UARTn);
	CLR_BIT(UARTxPtr->CR1, 1);
}

void UART_voidSetReceiverMute(UART_unitNumber_t UARTn)
{
	volatile UART_t* UARTxPtr = UART_PtrGetAddress(UARTn);
	SET_BIT(UARTxPtr->CR1, 1);
}

void UART_voidSetWordLength(UART_unitNumber_t UARTn, UART_wordLength_t wordLen)
{
	volatile UART_t* UARTxPtr = UART_PtrGetAddress(UARTn);
	if (wordLen == UART_wordLength_8)
		CLR_BIT(UARTxPtr->CR1, 12);
	else
		SET_BIT(UARTxPtr->CR1, 12);
}

void UART_voidUSARTEnable(UART_unitNumber_t UARTn)
{
	volatile UART_t* UARTxPtr = UART_PtrGetAddress(UARTn);
	SET_BIT(UARTxPtr->CR1, 13);
}

void UART_voidUSARTDisable(UART_unitNumber_t UARTn)
{
	volatile UART_t* UARTxPtr = UART_PtrGetAddress(UARTn);
	CLR_BIT(UARTxPtr->CR1, 13);
}

void UART_voidSetStopBits(UART_unitNumber_t UARTn, UART_stopBits_t sb)
{
	volatile UART_t* UARTxPtr = UART_PtrGetAddress(UARTn);
	EDT_REG(UARTxPtr->CR2 ,12, sb, 2);
}

UART_error_t UART_enumReciveByte(UART_unitNumber_t UARTn, char* bytePtr)
{
	volatile UART_t* UARTxPtr = UART_PtrGetAddress(UARTn);
	/*	wait for read data register to be empty (data to be received)	*/
	while (!GET_BIT(UARTxPtr->SR, 5));

	*bytePtr = UARTxPtr->DR;

	return 0;
}

UART_error_t UART_enumSendByte(UART_unitNumber_t UARTn, char byte)
{
	volatile UART_t* UARTxPtr = UART_PtrGetAddress(UARTn);
	/*	wait for previous transmission complete - if any -	*/
	while (!GET_BIT(UARTxPtr->SR, 6));

	/*	load byte to data register to be transmitted	*/
	UARTxPtr->DR = byte;

	return 0;
}

void UART_voidEnableInterrupt(UART_unitNumber_t UARTn, UART_Interrupt_t interrupt)
{
	volatile UART_t* UARTxPtr = UART_PtrGetAddress(UARTn);

	switch (interrupt)
	{
	case UART_Interrupt_RXNE:
		SET_BIT(UARTxPtr->CR1, 5);
		break;
	}
}

void  UART_voidSetCallBack(UART_unitNumber_t UARTn, UART_Interrupt_t interrupt, void(*callBack)(void))
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














