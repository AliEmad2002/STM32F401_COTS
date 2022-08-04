/*
 * LCD_program.h
 *
 *  Created on:	Jul 28, 2022
 *  Author:		Ali Emad Ali
 *
 *  References:
 *  	XIAMEN AMOTEC DISPLAY LCD MODULE DATASHEET: https://www.sparkfun.com/datasheets/LCD/ADM1602K-NSW-FBS-3.3v.pdf
 *  	https://circuitdigest.com/article/16x2-lcd-display-module-pinout-datasheet
 */
 

#include "LIB/BIT_MATH.h"
#include "LIB/STD_TYPES.h"

#include "MCAL/GPIO/GPIO_interface.h"
#include "HAL/LCD/LCD_interface.h"
#include "TEMPDELAY.h"

void LCD_voidInit(
	LCD_t* lcdPtr,				LCD_MODE _mode,
	DIO_PIN _dataPinArr[],		DIO_PIN _rsPin,
	DIO_PIN _rwPin,				DIO_PIN _enPin,
	HLCD_CURSORMODE cursorMode,	DIO_OUTPUTSPEED pinOutputSpeed
	)
{
	/*	store and init data pins	*/
	for (u8 i=0; i<4; i++)
	{
		lcdPtr->dataPortArr[i]	= _dataPinArr[i] / 16;
		lcdPtr->dataPinArr[i]	= _dataPinArr[i] % 16;
		DIO_voidSetPinMode(lcdPtr->dataPortArr[i], lcdPtr->dataPinArr[i], DIO_MODE_GPO);
		DIO_voidSetPinOutputType(lcdPtr->dataPortArr[i], lcdPtr->dataPinArr[i], DIO_OUTPUTTYPE_pushPull);
		DIO_voidSetPinOutputSpeed(lcdPtr->dataPortArr[i], lcdPtr->dataPinArr[i], pinOutputSpeed);
	}
	if (_mode == LCD_MODE_8bits)
		for (u8 i=4; i<8; i++)
		{
			lcdPtr->dataPortArr[i]	= _dataPinArr[i] / 16;
			lcdPtr->dataPinArr[i]	= _dataPinArr[i] % 16;
			DIO_voidSetPinMode(lcdPtr->dataPortArr[i], lcdPtr->dataPinArr[i], DIO_MODE_GPO);
			DIO_voidSetPinOutputType(lcdPtr->dataPortArr[i], lcdPtr->dataPinArr[i], DIO_OUTPUTTYPE_pushPull);
			DIO_voidSetPinOutputSpeed(lcdPtr->dataPortArr[i], lcdPtr->dataPinArr[i], pinOutputSpeed);
		}
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/

	/*	store and init control pins	*/
	lcdPtr->rsPort	= _rsPin / 16;
	lcdPtr->rsPin	= _rsPin % 16;
	DIO_voidSetPinMode(lcdPtr->rsPort, lcdPtr->rsPin, DIO_MODE_GPO);
	DIO_voidSetPinOutputType(lcdPtr->rsPort, lcdPtr->rsPin, DIO_OUTPUTTYPE_pushPull);
	DIO_voidSetPinOutputSpeed(lcdPtr->rsPort, lcdPtr->rsPin, pinOutputSpeed);

	lcdPtr->rwPort	= _rwPin / 16;
	lcdPtr->rwPin	= _rwPin % 16;
	DIO_voidSetPinMode(lcdPtr->rwPort, lcdPtr->rwPin, DIO_MODE_GPO);
	DIO_voidSetPinOutputType(lcdPtr->rwPort, lcdPtr->rwPin, DIO_OUTPUTTYPE_pushPull);
	DIO_voidSetPinOutputSpeed(lcdPtr->rwPort, lcdPtr->rwPin, pinOutputSpeed);
	
	lcdPtr->enPort	= _enPin / 16;
	lcdPtr->enPin	= _enPin % 16;
	DIO_voidSetPinMode(lcdPtr->enPort, lcdPtr->enPin, DIO_MODE_GPO);
	DIO_voidSetPinOutputType(lcdPtr->enPort, lcdPtr->enPin, DIO_OUTPUTTYPE_pushPull);
	DIO_voidSetPinOutputSpeed(lcdPtr->enPort, lcdPtr->enPin, pinOutputSpeed);
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/
	
	/*	give LCD-module some time to start up before communicating with it	*/
	//MCLOCK_voidDelayMS(500) ;
	tempDelay(500);
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/

	/*	save parameters	*/
	lcdPtr->mode = _mode;
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/

	/*	init mode	*/
	if (_mode == LCD_MODE_4bits)
	{
		LCD_voidSendCMD(lcdPtr, 0x02);
		LCD_voidSendCMD(lcdPtr, 0x28);
	}
	else
		LCD_voidSendCMD(lcdPtr, 0x38);
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/
	
	/*	init cursor mode	*/
	LCD_voidSetCursorMode(lcdPtr, cursorMode);
	LCD_voidSendCMD(lcdPtr, 0x06); // auto shifting cursor
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/
	
	/*	clear display	*/
	LCD_voidClearDisplay(lcdPtr);
	/*	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	*/
	
	/*	make courser go home	*/
	LCD_voidSendCMD(lcdPtr, 0x80) ;
}

void LCD_voidSetCursorMode(LCD_t* lcdPtr, HLCD_CURSORMODE cursorMode)
{
	LCD_voidSendCMD(lcdPtr, cursorMode);
}

void LCD_voidClearDisplay(LCD_t* lcdPtr)
{
	LCD_voidSendCMD(lcdPtr, 0x01) ;
	/*	here execution time is more that other instructions	*/
	tempDelay(1) ;
}

void LCD_voidSendCMD(LCD_t* lcdPtr, u8 cmd)
{
	/*	select the command register of the LCD-module	*/
	DIO_voidSetPinOutputLevel(lcdPtr->rsPort, lcdPtr->rsPin, DIO_OUTPUTLEVEL_low);
	
	/*	write cmd to the bus, and LCD-module latch it	*/
	LCD_voidWriteBusAndLatch(lcdPtr, cmd);
	
	/*	wait execution time	*/
	tempDelay(3);
}

void LCD_voidSendData(LCD_t* lcdPtr, u8 data)
{
	/*	select the data register of the LCD-module	*/
	DIO_voidSetPinOutputLevel(lcdPtr->rsPort, lcdPtr->rsPin, DIO_OUTPUTLEVEL_high);
	
	/*	write data to the bus, and LCD-module latch it	*/
	LCD_voidWriteBusAndLatch(lcdPtr, data);
}

void LCD_voidWriteBusAndLatch(LCD_t* lcdPtr, u8 busVal)
{
	/*	select write mode	*/
	DIO_voidSetPinOutputLevel(lcdPtr->rwPort, lcdPtr->rwPin, DIO_OUTPUTLEVEL_low);
	
	if (lcdPtr->mode == LCD_MODE_8bits)
	{
		/*	write the data at once	*/
		for (u8 i=0; i<8; i++)
			DIO_voidSetPinOutputLevel(lcdPtr->dataPortArr[i], lcdPtr->dataPinArr[i], GET_BIT(busVal, i));
		LCD_voidLatch(lcdPtr);
	}
	else
	{
		/*	write the data in two times	*/
		/*	higher 4 bits first	*/
		for (u8 i=0; i<4; i++)
			DIO_voidSetPinOutputLevel(lcdPtr->dataPortArr[i], lcdPtr->dataPinArr[i], GET_BIT(busVal, i+4));
		LCD_voidLatch(lcdPtr);
		/*	lower 4 bits then	*/
		for (u8 i=0; i<4; i++)
			DIO_voidSetPinOutputLevel(lcdPtr->dataPortArr[i], lcdPtr->dataPinArr[i], GET_BIT(busVal, i));
		LCD_voidLatch(lcdPtr);
	}
}

void LCD_voidLatch(LCD_t* lcdPtr)
{
	/*	make EN pin HIGH, then LOW, to make the LCD-module latch the data bus	*/
	DIO_voidSetPinOutputLevel(lcdPtr->enPort, lcdPtr->enPin, DIO_OUTPUTLEVEL_high);
	tempDelay(1) ;
	DIO_voidSetPinOutputLevel(lcdPtr->enPort, lcdPtr->enPin, DIO_OUTPUTLEVEL_low);
}

void LCD_voidPrintString(LCD_t* lcdPtr, char* str)
{
	for (u8 i=0; str[i]!='\0'; i++)
		LCD_voidSendData(lcdPtr, str[i]);
}
