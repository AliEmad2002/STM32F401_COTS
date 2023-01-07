/*
 * RGB_LED_program.c
 *
 *  Created on: Oct 22, 2022
 *      Author: Ali Emad Ali
 */


/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"

/*	MCAL	*/
#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "TIM_interface.h"

/*	SELF	*/
#include "RGB_LED_interface.h"


/*	inits RGB_LED object	*/
void RGB_LED_voidInit(
	RGB_LED_t* ledPtr,
	u8 redTimUnitNum, u8 greenTimUnitNum, u8 blueTimUnitNum,
	TIM_Channel_t redTimCh, TIM_Channel_t greenTimCh, TIM_Channel_t blueTimCh,
	u8 redAFIOTimMap, u8 greenAFIOTimMap, u8 blueAFIOTimMap,
	u16 freqHz
	)
{
	ledPtr->timUnitNumArr[0] = redTimUnitNum;
	ledPtr->timUnitNumArr[1] = greenTimUnitNum;
	ledPtr->timUnitNumArr[2] = blueTimUnitNum;

	ledPtr->timChArr[0] = redTimCh;
	ledPtr->timChArr[1] = greenTimCh;
	ledPtr->timChArr[2] = blueTimCh;

	u8 tempTimMapArr[3] = {
		redAFIOTimMap, greenAFIOTimMap, blueAFIOTimMap
	};

	for (u8 i = 0; i < 3; i++)
	{
		TIM_voidDisableCounter(ledPtr->timUnitNumArr[i]);

		(void)TIM_u64InitPWM(
			ledPtr->timUnitNumArr[i], ledPtr->timChArr[i],
			TIM_OutputCompareMode_PWM1, freqHz);

		TIM_voidInitOutputPin(
			ledPtr->timUnitNumArr[i], ledPtr->timChArr[i], tempTimMapArr[i]);

		TIM_voidEnableCounter(ledPtr->timUnitNumArr[i]);
	}


	RGB_LED_voidSetColor(ledPtr, (RGB_LED_Color_t){0, 0, 0});
}

/*	sets color	*/
void RGB_LED_voidSetColor(RGB_LED_t* ledPtr, RGB_LED_Color_t color)
{
	TIM_voidSetDutyCycle(
		ledPtr->timUnitNumArr[0], ledPtr->timChArr[0], color.red);

	TIM_voidSetDutyCycle(
		ledPtr->timUnitNumArr[1], ledPtr->timChArr[1], color.green);

	TIM_voidSetDutyCycle(
		ledPtr->timUnitNumArr[2], ledPtr->timChArr[2], color.blue);
}




















