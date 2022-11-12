/*
 * BKP_private.h
 *
 *  Created on: Oct 8, 2022 2:16:13 AM
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_MCAL_BKP_BKP_PRIVATE_H_
#define INCLUDE_MCAL_BKP_BKP_PRIVATE_H_


typedef struct{
	u32 reserved1;
	u32 data1[10];	// can only use first 16 bits!
	// cont. later...
}BKP_t;



#define BKP		((volatile BKP_t*)0x40006C00)

#endif /* INCLUDE_MCAL_BKP_BKP_PRIVATE_H_ */
