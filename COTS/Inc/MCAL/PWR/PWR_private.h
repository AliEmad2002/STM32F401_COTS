/*
 * PWR_private.h
 *
 *  Created on: Oct 8, 2022 1:24:11 PM
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_MCAL_PWR_PWR_PRIVATE_H_
#define INCLUDE_MCAL_PWR_PWR_PRIVATE_H_


typedef struct{
	u32 CR;
}PWR_t;


#define PWR		((volatile PWR_t*)0x40007000)


#endif /* INCLUDE_MCAL_PWR_PWR_PRIVATE_H_ */
