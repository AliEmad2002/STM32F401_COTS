/*
 * BKP_interface.h
 *
 *  Created on: Oct 8, 2022 2:16:02 AM
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_MCAL_BKP_BKP_INTERFACE_H_
#define INCLUDE_MCAL_BKP_BKP_INTERFACE_H_


void BKP_voidStoreData(u16 data, u8 location);

u16 BKP_u16GetData(u8 location);

#endif /* INCLUDE_MCAL_BKP_BKP_INTERFACE_H_ */
