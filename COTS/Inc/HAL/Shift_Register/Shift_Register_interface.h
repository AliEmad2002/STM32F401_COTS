/*
 * Shift_Register_interface.h
 *
 *  Created on:	Sep 10, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */

#ifndef _SHIFT_REGISTER_INTERFACE_H_
#define _SHIFT_REGISTER_INTERFACE_H_

typedef enum{
	SR_ClockTriggeringEdge_falling,
	SR_ClockTriggeringEdge_rising,
}SR_ClockTriggeringEdge_t;

typedef struct{
	/* pin number of the shift register shift pin	*/
	u8 clkPin;
	/* port of the shift register shift pin	*/
	GPIO_PortName_t clkPort;
	/*	triggering edge of clk pin	*/
	SR_ClockTriggeringEdge_t clkTrigEdge;
	/*	representing clk max freq	*/
	u32 minClkTicks;

	/* pin number of the shift register input pin	*/
	u8 dataInPin;
	/* port number of the shift register input pin	*/
	GPIO_PortName_t dataInPort;
	/*	number of ticks of which data pin must be stablized before pulsing a clk	*/
	u32 minDataInTicks;

	/* pin number of the shift register latch pin	*/
	u8 latchPin;
	/* port number of the shift register latch pin	*/
	GPIO_PortName_t latchPort;
	/*	latch pin active level	*/
	GPIO_OutputLevel_t latchActiveType;
	/* min number of ticks enough to latch	*/
	u32 minLatchTicks;
}SR_SerialInParallelOut_t;

/*
 * initializes a serial input parallel output sift register.
 */
void SR_voidInitSerialInParallelOut(
	SR_SerialInParallelOut_t* regPtr,
	GPIO_Pin_t _clkPin, GPIO_Pin_t _dataInPin, GPIO_Pin_t _latchPin,
	SR_ClockTriggeringEdge_t _clkTrigEdge, GPIO_OutputLevel_t _latchActiveType,
	u32 maxClkFrequency, u32 _dataInStableTimeNanoSec, u32 latchStableTimeNanoSec
	);

/*	**********************************************************	*/
/*	*********************D A T A  C O N T R O L***************	*/
/*	**********************************************************	*/
/*
 * writes the states of the serial input pin.
 */
void SR_voidWriteInputSerialInParallelOut(SR_SerialInParallelOut_t* regPtr, b8 dataBit);

/*	**********************************************************	*/
/*	********************C L O C K  C O N T R O L***************	*/
/*	**********************************************************	*/
/*
 * disables clock pin (writes it with a logic inverse to its active logic).
 */
void SR_voidClkDisableSerialInParallelOut(SR_SerialInParallelOut_t* regPtr);

/*
 * enables clock pin (writes it with its active logic).
 */
void SR_voidClkEnableSerialInParallelOut(SR_SerialInParallelOut_t* regPtr);

/*
 * triggers a clock pulse on the clock pin.
 */
void SR_voidClkSerialInParallelOut(SR_SerialInParallelOut_t* regPtr);

/*	**********************************************************	*/
/*	*******************L A T C H  C O N T R O L***************	*/
/*	**********************************************************	*/
/*
 * enables latch.
 */
void SR_voidEnableLatchSerialInParallelOut(SR_SerialInParallelOut_t* regPtr);

/*
 * disables latch.
 */
void SR_voidDisableLatchSerialInParallelOut(SR_SerialInParallelOut_t* regPtr);

/*
 * latches the data in the shift register to the output pins (enables then disables it).
 */
void SR_voidLatchSerialInParallelOut(SR_SerialInParallelOut_t* regPtr);

/*	**********************************************************	*/
/*	******************O V E R  A L L  C O N T R O L************	*/
/*	**********************************************************	*/
/*
 * shifts the whole register by one, adding a new bit.
 * does not latch to output pins.
 */
void SR_voidShiftSerialInParallelOut(SR_SerialInParallelOut_t* regPtr, b8 dataBit);

/*
 * shifts the whole register by "numberOfBitsToBeLoaded" bits, from "byteArr[]".
 * does not latch to output pins.
 */
void SR_voidLoadByteArrSerialInParallelOut(SR_SerialInParallelOut_t* regPtr, u8 byteArr[], u16 numberOfBitsToBeLoaded);

/*
 * shifts the whole register by "numberOfBitsToBeLoaded" bits, from "val".
 * does not latch to output pins.
 */
void SR_voidLoadFromU32SerialInParallelOut(SR_SerialInParallelOut_t* regPtr, u32 val, u8 numberOfBitsToBeLoaded);

/*	copies reg2 to reg1, reg1 should be previously allocated	*/
void SR_voidCopySerialInParallelOut(SR_SerialInParallelOut_t* regPtr1, SR_SerialInParallelOut_t* regPtr2);

#endif /* _SHIFT_REGISTER_INTERFACE_H_ */
