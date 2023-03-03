/*
 * UART_interface.h
 *
 *  Created on:	Sep 18, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */

#ifndef _UART_INTERFACE_H_
#define _UART_INTERFACE_H_

#include "UART_config.h"

typedef enum{
	UART_OverSamplingMode_by16,
	UART_OverSamplingMode_by8
}UART_OverSamplingMode_t;

typedef enum{
	UART_Mode_std,
	UART_Mode_other	// Smartcard, IrDA, LIN
}UART_Mode_t;

typedef enum{
	UART_Parity_even,
	UART_Parity_odd
}UART_Parity_t;

typedef enum{
	UART_WakeUpMethod_idleLine,
	UART_WakeUpMethod_addressMask
}UART_WakeUpMethod_t;

typedef enum{
	UART_WordLength_8,
	UART_WordLength_9
}UART_WordLength_t;

typedef enum{
	UART_StopBits_one,
	UART_StopBits_half,
	UART_StopBits_two,
	UART_StopBits_oneAndHalf
}UART_StopBits_t;

#if TARGET_ID == STM32F401x
typedef enum{
	UART_UnitNumber_1,
	UART_UnitNumber_2,
	UART_UnitNumber_6
}UART_UnitNumber_t;

#elif TARGET_ID == STM32F10x
typedef enum{
	UART_UnitNumber_1,
	UART_UnitNumber_2,
	//UART_UnitNumber_3
	//UART_UnitNumber_4,
	//UART_UnitNumber_5
}UART_UnitNumber_t;
#endif

typedef enum{
	a
}UART_Error_t;

typedef enum{
	UART_Interrupt_RXNE,
	UART_Interrupt_ORE
}UART_Interrupt_t;

void UART_voidInit(
	UART_UnitNumber_t UARTn,
	#if TARGET_ID == STM32F401x
	UART_Mode_t mode, UART_OverSamplingMode_t over8,
	#endif
	u32 baudRate, b8 RxEn, b8 TxEn, UART_WordLength_t wordLen,
	UART_StopBits_t stopBits
	);

b8 UART_b8IsRccClockEnabled(UART_UnitNumber_t UARTn);

void UART_voidEnableRccClock(UART_UnitNumber_t UARTn);

void UART_voidFastInit(UART_UnitNumber_t UARTn, u32 baudRate, u8 map);

void UART_voidSetReceiverActive(UART_UnitNumber_t UARTn);

void UART_voidSetReceiverMute(UART_UnitNumber_t UARTn);

void UART_voidSetWordLength(UART_UnitNumber_t UARTn, UART_WordLength_t wordLen);

void UART_voidUSARTEnable(UART_UnitNumber_t UARTn);

void UART_voidUSARTDisable(UART_UnitNumber_t UARTn);

void UART_voidSetStopBits(UART_UnitNumber_t UARTn, UART_StopBits_t sb);

UART_Error_t UART_enumReciveByte(UART_UnitNumber_t UARTn, char* bytePtr);

UART_Error_t UART_enumSendByte(UART_UnitNumber_t UARTn, char byte);

void UART_voidSendByteEcho(UART_UnitNumber_t UARTn, char byte);

/*	sends a string and validates echo of each byte	*/
void UART_voidSendStringEcho(UART_UnitNumber_t UARTn, char* str);

void UART_voidFlushDataReceiveRegister(UART_UnitNumber_t UARTn);

b8 UART_b8ReceiveByteTimeout(UART_UnitNumber_t UARTn, char* bytePtr, u16 msTimeout);

void UART_voidReceiveUntilByte(UART_UnitNumber_t UARTn, char* str, char term);

b8 UART_b8ReceiveStringTimeout(
	UART_UnitNumber_t UARTn, char* str, u32 msTimeout, char* terminatorStr,
	u16* lenPtr);

void UART_voidSendString(UART_UnitNumber_t UARTn, char* str);

u16 UART_u16ReadString(UART_UnitNumber_t UARTn, char* str, char terminator, u16 terminatorCount);

void UART_voidEnableInterrupt(UART_UnitNumber_t UARTn, UART_Interrupt_t interrupt);

void UART_voidDisableInterrupt(UART_UnitNumber_t UARTn, UART_Interrupt_t interrupt);

void  UART_voidSetCallBack(UART_UnitNumber_t UARTn, UART_Interrupt_t interrupt, void(*callBack)(void));

void UART_voidSendBreakChar(UART_UnitNumber_t UARTn);

void UART_voidEnableParity(UART_UnitNumber_t UARTn, UART_Parity_t mode);

void UART_voidDisableParity(UART_UnitNumber_t UARTn);

void UART_voidSetWakeUpMethod(UART_UnitNumber_t UARTn, UART_WakeUpMethod_t method);

void UART_voidSetNodeAddress(UART_UnitNumber_t UARTn, u8 address);

void UART_voidEnableHalfDuplex(UART_UnitNumber_t UARTn);

void UART_voidDisableHalfDuplex(UART_UnitNumber_t UARTn);

void UART_voidEnableNoiseSelectionFlag(UART_UnitNumber_t UARTn);

void UART_voidDisableNoiseSelectionFlag(UART_UnitNumber_t UARTn);

typedef struct{
	char str[UART_MAX_BUFFER_STR_LEN];
	u8 len;
	b8 isCompleted;
}UART_Buffer_t;

void UART_voidInitBuffer(UART_Buffer_t* bufPtr);

#endif /* _UART_INTERFACE_H_ */
