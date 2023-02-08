/*
 * MDAC_interface.h
 *
 *  Created on: Oct 13, 2022
 *      Author: Ali Emad Ali
 *
 * Notice: Medium density blue-pill does not have DAC.
 * (Reference: DS5319 Rev 18 13/116)
 */

#ifndef INCLUDE_MCAL_MDAC_MDAC_INTERFACE_H_
#define INCLUDE_MCAL_MDAC_MDAC_INTERFACE_H_

typedef enum{
	MDAC_ChannelNumber_1,
	MDAC_ChannelNumber_2
}MDAC_ChannelNumber_t;

/*	enables channel	*/
void MDAC_voidEnableChannel(MDAC_ChannelNumber_t ch);

/*	disables channel	*/
void MDAC_voidDisableChannel(MDAC_ChannelNumber_t ch);

/*	enables channel's output buffer	*/
void MDAC_voidEnableOutputBuffer(MDAC_ChannelNumber_t ch);

/*	disables channel's output buffer	*/
void MDAC_voidDisableOutputBuffer(MDAC_ChannelNumber_t ch);

/*	enables channel's trigger	*/
void MDAC_voidEnableTrigger(MDAC_ChannelNumber_t ch);

/*	disables channel's trigger	*/
void MDAC_voidDisableTrigger(MDAC_ChannelNumber_t ch);

/*
 * writes 12-bit right aligned data to the data holding register of channel 1
 */
void MDAC_voidSetDHR12R1(u16 data);

#endif /* INCLUDE_MCAL_MDAC_MDAC_INTERFACE_H_ */
