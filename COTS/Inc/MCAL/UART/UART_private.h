/*
 * UART_private.h
 *
 *  Created on:	Sep 18, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */

#ifndef _UART_PRIVATE_H_
#define _UART_PRIVATE_H_


typedef struct{
	u32 SR;
	u32 DR;
	u32 BRR;
	u32 CR1;
	u32 CR2;
	u32 CR3;
	u32 GTPR;
}UART_t;

#if TARGET_ID == STM32F401x
#define UART1	((volatile UART_t*) 0x40011000)
#define UART2	((volatile UART_t*) 0x40004400)
#define UART6	((volatile UART_t*) 0x40011400)

volatile UART_t* UART[] = {UART1, UART2, UART6};

#elif TARGET_ID == STM32F10x
#define UART1	((volatile UART_t*) 0x40013800)
#define UART2	((volatile UART_t*) 0x40004400)
#define UART3	((volatile UART_t*) 0x40004800)
#define UART4	((volatile UART_t*) 0x40004C00)
#define UART5	((volatile UART_t*) 0x40005000)

volatile UART_t* const UART[] = {UART1, UART2, UART3, UART4, UART5};

#endif



#endif /* _UART_PRIVATE_H_ */
