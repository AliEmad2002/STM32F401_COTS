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
	CLK_TRIG_EDGE_falling,
	CLK_TRIG_EDGE_rising,
}CLK_TRIG_EDGE_t;

typedef struct{
	/* pin number of the shift register shift pin	*/
	u8 clkPin;
	/* port of the shift register shift pin	*/
	DIO_PORT clkPort;
	/*	triggering edge of clk pin	*/
	CLK_TRIG_EDGE_t clkTrigEdge;
	/*	representing clk max freq	*/
	u32 minClkTicks;

	/* pin number of the shift register input pin	*/
	u8 dataInPin;
	/* port number of the shift register input pin	*/
	DIO_PORT dataInPort;
	/*	number of ticks of which data pin must be stablized before pulsing a clk	*/
	u32 minDataInTicks;

	/* pin number of the shift register latch pin	*/
	u8 latchPin;
	/* port number of the shift register latch pin	*/
	DIO_PORT latchPort;
	/*	latch pin active level	*/
	DIO_OUTPUTTYPE latchActiveType;
	/* min number of ticks enough to latch	*/
	u32 minLatchTicks;
}SIPO_SHIFT_REGISTER_t;

/*
 * initializes a serial input parallel output sift register.
 */
void SHIFT_REGISTER_voidInitSIPO(
	SIPO_SHIFT_REGISTER_t* regPtr,
	DIO_PIN _clkPin, DIO_PIN _dataInPin, DIO_PIN _latchPin,
	CLK_TRIG_EDGE_t _clkTrigEdge, DIO_OUTPUTTYPE _latchActiveType,
	u32 maxClkFrequency, u32 _dataInStableTimeNanoSec, u32 latchStableTimeNanoSec
	);

/*	**********************************************************	*/
/*	*********************D A T A  C O N T R O L***************	*/
/*	**********************************************************	*/
/*
 * writes the states of the serial input pin.
 */
void SHIFT_REGISTER_voidWriteInputSIPO(SIPO_SHIFT_REGISTER_t* regPtr, b8 dataBit);

/*	**********************************************************	*/
/*	********************C L O C K  C O N T R O L***************	*/
/*	**********************************************************	*/
/*
 * disables clock pin (writes it with a logic inverse to its active logic).
 */
void SHIFT_REGISTER_voidClkDisableSIPO(SIPO_SHIFT_REGISTER_t* regPtr);

/*
 * enables clock pin (writes it with its active logic).
 */
void SHIFT_REGISTER_voidClkEnableSIPO(SIPO_SHIFT_REGISTER_t* regPtr);

/*
 * triggers a clock pulse on the clock pin.
 */
void SHIFT_REGISTER_voidClkSIPO(SIPO_SHIFT_REGISTER_t* regPtr);

/*	**********************************************************	*/
/*	*******************L A T C H  C O N T R O L***************	*/
/*	**********************************************************	*/
/*
 * enables latch.
 */
void SHIFT_REGISTER_voidEnableLatchSIPO(SIPO_SHIFT_REGISTER_t* regPtr);

/*
 * disables latch.
 */
void SHIFT_REGISTER_voidDisableLatchSIPO(SIPO_SHIFT_REGISTER_t* regPtr);

/*
 * latches the data in the shift register to the output pins (enables then disables it).
 */
void SHIFT_REGISTER_voidLatchSIPO(SIPO_SHIFT_REGISTER_t* regPtr);

/*	**********************************************************	*/
/*	******************O V E R  A L L  C O N T R O L************	*/
/*	**********************************************************	*/
/*
 * shifts the whole register by one, adding a new bit.
 * does not latch to output pins.
 */
void SHIFT_REGISTER_voidShiftSIPO(SIPO_SHIFT_REGISTER_t* regPtr, b8 dataBit);

/*
 * shifts the whole register by "numberOfBitsToBeLoaded" bits, from "byteArr[]".
 * does not latch to output pins.
 */
void SHIFT_REGISTER_voidLoadByteArrSIPO(SIPO_SHIFT_REGISTER_t* regPtr, u8 byteArr[], u16 numberOfBitsToBeLoaded);

/*
 * shifts the whole register by "numberOfBitsToBeLoaded" bits, from "val".
 * does not latch to output pins.
 */
void SHIFT_REGISTER_voidLoadFromU32SIPO(SIPO_SHIFT_REGISTER_t* regPtr, u32 val, u8 numberOfBitsToBeLoaded);

#endif /* _SHIFT_REGISTER_INTERFACE_H_ */
