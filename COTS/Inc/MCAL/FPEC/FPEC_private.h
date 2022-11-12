/*
 * FPEC_private.h
 *
 *  Created on: Oct 18, 2022
 *      Author: Ali Emad Ali
 */

#ifndef _FPEC_PRIVATE_H_
#define _FPEC_PRIVATE_H_

typedef struct{
	volatile u32 ACR;
	volatile u32 KEYR;
	volatile u32 OPTKEYR;
	volatile u32 SR;
	volatile u32 CR;
	volatile u32 AR;
	volatile u32 reserved;
	volatile u32 OBR;
	volatile u32 WRPR;
}FPEC_t;

#define FPEC	((volatile FPEC_t*)0x40022000)

#define FPEC_KEY1	 0x45670123
#define FPEC_KEY2	 0xCDEF89AB

typedef struct{
	u16 data[1024];
}FPEC_Page_t;

typedef struct{
	FPEC_Page_t pageArr[64];
}FPEC_MainMemory_t;

#define	FPEC_MAIN_MEMORY	((volatile FPEC_MainMemory_t*)0x08000000)

/*	ACR bits	*/
#define FPEC_ACR_LATENCY_0		0

/*	CR bits	*/
#define FPEC_CR_PG				0
#define FPEC_CR_PER				1
#define FPEC_CR_MER				2
#define FPEC_CR_START			6
#define FPEC_CR_LOCK			7

#endif /* _FPEC_PRIVATE_H_ */
