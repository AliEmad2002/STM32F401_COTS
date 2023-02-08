/*
 * MDAC_program.c
 *
 *  Created on: Oct 13, 2022
 *      Author: Ali Emad Ali
 */

/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"

/*	SELF	*/
#include "MDAC_interface.h"
#include "MDAC_private.h"


inline void MDAC_voidEnableChannel(MDAC_ChannelNumber_t ch)
{
	SET_BIT(MDAC->CR, 16 * ch);
}

inline void MDAC_voidDisableChannel(MDAC_ChannelNumber_t ch)
{
	CLR_BIT(MDAC->CR, 16 * ch);
}

inline void MDAC_voidEnableOutputBuffer(MDAC_ChannelNumber_t ch)
{
	CLR_BIT(MDAC->CR, 16 * ch + 1);
}

inline void MDAC_voidDisableOutputBuffer(MDAC_ChannelNumber_t ch)
{
	SET_BIT(MDAC->CR, 16 * ch + 1);
}

inline void MDAC_voidEnableTrigger(MDAC_ChannelNumber_t ch)
{
	SET_BIT(MDAC->CR, 16 * ch + 2);
}

inline void MDAC_voidDisableTrigger(MDAC_ChannelNumber_t ch)
{
	CLR_BIT(MDAC->CR, 16 * ch + 2);
}

inline void MDAC_voidSetDHR12R1(u16 data)
{
	MDAC->DHR12R1 = data;
}









