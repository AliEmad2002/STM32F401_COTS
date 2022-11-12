/*
 * Delay_interface.h
 *
 *  Created on:	Jul 28, 2022
 *  Author:		Ali Emad Ali
 *
 *
 */

#ifndef _DELAY_INTERFACE_H_
#define _DELAY_INTERFACE_H_

void Delay_voidBlockingDelayMs(u32 ms);

void Delay_voidBlockingDelayUs(u32 us);

void Delay_voidBlockingDelayTicks(u64 ticks);

/*
 * this is a special function that runs @ 16MHz, no iterations, no math,
 * just optimized to "NOP" for 2 us, i.e.: 32 clock cycles.
 */
void Delay_voidDelay2Us(void);

#endif /* _DELAY_INTERFACE_H_ */
