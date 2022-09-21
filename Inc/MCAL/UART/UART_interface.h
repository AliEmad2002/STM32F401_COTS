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

typedef enum{
	UART_overSamplingMode_by16,
	UART_overSamplingMode_by8
}UART_overSamplingMode_t;

typedef enum{
	UART_mode_std,
	UART_mode_other	// Smartcard, IrDA, LIN
}UART_mode_t;

typedef enum{
	UART_parity_even,
	UART_parity_odd
}UART_parity_t;

typedef enum{
	UART_wakeUpMethod_idleLine,
	UART_wakeUpMethod_addressMask
}UART_wakeUpMethod_t;

typedef enum{
	UART_wordLength_8,
	UART_wordLength_9
}UART_wordLength_t;

typedef enum{
	UART_stopBits_one,
	UART_stopBits_half,
	UART_stopBits_two,
	UART_stopBits_oneAndHalf
}UART_stopBits_t;

typedef enum{
	UART_unitNumber_1,
	UART_unitNumber_2,
	UART_unitNumber_3,
	UART_unitNumber_4,
	UART_unitNumber_5,
	UART_unitNumber_6
}UART_unitNumber_t;

typedef enum{
	a
}UART_error_t;

typedef enum{
	UART_Interrupt_RXNE
}UART_Interrupt_t;

void UART_voidInit(
	UART_unitNumber_t UARTn, UART_mode_t mode, UART_overSamplingMode_t over8,
	u32 baudRate, b8 RxEn, b8 TxEn, UART_wordLength_t wordLen,
	UART_stopBits_t stopBits
	);

void UART_voidSetReceiverActive(UART_unitNumber_t UARTn);

void UART_voidSetReceiverMute(UART_unitNumber_t UARTn);

void UART_voidSetWordLength(UART_unitNumber_t UARTn, UART_wordLength_t wordLen);

void UART_voidUSARTEnable(UART_unitNumber_t UARTn);

void UART_voidUSARTDisable(UART_unitNumber_t UARTn);

void UART_voidSetStopBits(UART_unitNumber_t UARTn, UART_stopBits_t sb);

UART_error_t UART_enumReciveByte(UART_unitNumber_t UARTn, char* bytePtr);

UART_error_t UART_enumSendByte(UART_unitNumber_t UARTn, char byte);

void UART_voidEnableInterrupt(UART_unitNumber_t UARTn, UART_Interrupt_t interrupt);

void  UART_voidSetCallBack(UART_unitNumber_t UARTn, UART_Interrupt_t interrupt, void(*callBack)(void));

void UART_voidSendBreakChar(UART_unitNumber_t UARTn);

void UART_voidEnableParity(UART_unitNumber_t UARTn, UART_parity_t mode);

void UART_voidDisableParity(UART_unitNumber_t UARTn);

void UART_voidSetWakeUpMethod(UART_unitNumber_t UARTn, UART_wakeUpMethod_t method);

void UART_voidSetNodeAddress(UART_unitNumber_t UARTn, u8 address);

void UART_voidEnableHalfDuplex(UART_unitNumber_t UARTn);

void UART_voidDisableHalfDuplex(UART_unitNumber_t UARTn);

void UART_voidEnableNoiseSelectionFlag(UART_unitNumber_t UARTn);

void UART_voidDisableNoiseSelectionFlag(UART_unitNumber_t UARTn);

#endif /* _UART_INTERFACE_H_ */
