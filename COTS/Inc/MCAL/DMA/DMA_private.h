/*
 * DMA_private.h
 *
 *  Created on: Oct 11, 2022
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_MCAL_DMA_DMA_PRIVATE_H_
#define INCLUDE_MCAL_DMA_DMA_PRIVATE_H_

typedef struct{
	volatile u32	CR;
	volatile u32 	NDTR;
	volatile u32	PAR;
	volatile u32	MAR;
	volatile u32 	reserved1;
}DMA_Channel_t;

typedef struct{
	volatile u32 				ISR;
	volatile u32 				IFCR;
	volatile DMA_Channel_t		c[7];
}DMA_t;


/*	CCR bits	*/
#define DMA_CCR_EN			0
#define DMA_CCR_DIR			4
#define DMA_CCR_CIRC		5
#define DMA_CCR_PINC		6
#define DMA_CCR_MINC		7
#define DMA_CCR_PSIZE_0		8
#define DMA_CCR_MSIZE_0		10
#define DMA_CCR_PL_0		12
#define DMA_CCR_MEM2MEM		14


#define DMA1	((volatile DMA_t*)0x40020000)
#define DMA2	((volatile DMA_t*)0x40020400)

volatile DMA_t* const DMA[] = {DMA1, DMA2};

#define DMA_NOT_IN_RANGE_ERR_CODE		0

#endif /* INCLUDE_MCAL_DMA_DMA_PRIVATE_H_ */
