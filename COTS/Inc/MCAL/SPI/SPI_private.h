/*
 * SPI_private.h
 *
 *  Created on: Oct 1, 2022 10:24:30 PM
 *      Author: Ali Emad Ali
 */

#ifndef _SPI_SPI_PRIVATE_H_
#define _SPI_SPI_PRIVATE_H_


typedef struct{
	volatile u32 CR1		;
	volatile u32 CR2		;
	volatile u32 SR         ;
	volatile u16 DR         ;
	volatile u16 reserved   ;
	volatile u32 CRCPR      ;
	volatile u32 RXCRCR     ;
	volatile u32 TXCRCR     ;
	volatile u32 I2SCFGR    ;
	volatile u32 I2SPR      ;
}SPI_t;

#define SPI1	((volatile SPI_t*)0x40013000)
#define SPI2	((volatile SPI_t*)0x40003800)
#define SPI3	((volatile SPI_t*)0x40003C00)

static volatile SPI_t* const SPI[] = {SPI1, SPI2, SPI3};


#define SPI_CR1_CPHA		0
#define SPI_CR1_CPOL		1
#define SPI_CR1_MSTR		2
#define SPI_CR1_BR_0		3
#define SPI_CR1_SPE			6
#define SPI_CR1_LSBFIRST	7
#define SPI_CR1_SSI			8
#define SPI_CR1_SSM			9
#define SPI_CR1_RXONLY		10
#define SPI_CR1_DFF			11
#define SPI_CR1_CRCNEXT		12
#define SPI_CR1_CRCEN		13
#define SPI_CR1_BIDIOE		14
#define SPI_CR1_BIDIMODE	15

#define SPI_CR2_RXDMAEN		0
#define SPI_CR2_TXDMAEN     1
#define SPI_CR2_SSOE        2
#define SPI_CR2_ERRIE       5
#define SPI_CR2_RXNEIE		6
#define SPI_CR2_TXEIE		7




#endif /* _SPI_SPI_PRIVATE_H_ */
