/*
 * RGB_LED_interface.h
 *
 *  Created on: Oct 22, 2022
 *      Author: Ali Emad Ali
 */

#ifndef RGB_LED_INTERFACE_H_
#define RGB_LED_INTERFACE_H_


/*	this driver uses internal timer units	*/
typedef struct{
	u8 timUnitNumArr[3];
	TIM_Channel_t timChArr[3];
}RGB_LED_t;

/*	inits RGB_LED object	*/
void RGB_LED_voidInit(
	RGB_LED_t* ledPtr,
	u8 redTimUnitNum, u8 greenTimUnitNum, u8 blueTimUnitNum,
	TIM_Channel_t redTimCh, TIM_Channel_t greenTimCh, TIM_Channel_t blueTimCh,
	u8 redAFIOTimMap, u8 greenAFIOTimMap, u8 blueAFIOTimMap,
	u16 freqHz
	);

typedef struct{
	u16 red;
	u16 green;
	u16 blue;
}RGB_LED_Color_t;

/*	sets color	*/
void RGB_LED_voidSetColor(RGB_LED_t* ledPtr, RGB_LED_Color_t color);


#endif /* RGB_LED_INTERFACE_H_ */
