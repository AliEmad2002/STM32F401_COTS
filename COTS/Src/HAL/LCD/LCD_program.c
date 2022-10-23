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
 
/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"
#include "Delay_interface.h"

/*	MCAL	*/
#include "GPIO_interface.h"

/*	SELF	*/
#include "LCD_interface.h"


const u8 DDRAMADD[] = {0x80, 0xC0, 0x90, 0xD0};

void LCD_voidInit(
	LCD_t* lcdPtr,					LCD_Mode_t _mode,
	GPIO_Pin_t _dataPinArr[],		GPIO_Pin_t _rsPin,
	GPIO_Pin_t _rwPin,				GPIO_Pin_t _enPin,
	HLCD_CursorMode_t cursorMode
	)
{
	/*	store and init data pins	*/
	u8 numberOfPins = (_mode == LCD_Mode_8bits) ? 8 : 4;
	for (u8 i=0; i<numberOfPins; i++)
	{
		lcdPtr->dataPortArr[i]	= _dataPinArr[i] / 16;
		lcdPtr->dataPinArr[i]	= _dataPinArr[i] % 16;
		GPIO_voidSetPinGpoPushPull(lcdPtr->dataPortArr[i], lcdPtr->dataPinArr[i]);
	}

	/*	store and init control pins	*/
	lcdPtr->rsPort	= _rsPin / 16;
	lcdPtr->rsPin	= _rsPin % 16;
	GPIO_voidSetPinGpoPushPull(lcdPtr->rsPort, lcdPtr->rsPin);

	lcdPtr->rwPort	= _rwPin / 16;
	lcdPtr->rwPin	= _rwPin % 16;
	GPIO_voidSetPinGpoPushPull(lcdPtr->rwPort, lcdPtr->rwPin);
	
	lcdPtr->enPort	= _enPin / 16;
	lcdPtr->enPin	= _enPin % 16;
	GPIO_voidSetPinGpoPushPull(lcdPtr->enPort, lcdPtr->enPin);
	
	/*	give LCD-module some time to start up before communicating with it	*/
	Delay_voidBlockingDelayMs(500);

	/*	save parameters	*/
	lcdPtr->mode = _mode;

	/*	init mode	*/
	if (_mode == LCD_Mode_4bits)
	{
		LCD_voidSendCMD(lcdPtr, 0x02);
		LCD_voidSendCMD(lcdPtr, 0x28);
	}
	else
		LCD_voidSendCMD(lcdPtr, 0x38);
	
	/*	init cursor mode	*/
	LCD_voidSetCursorMode(lcdPtr, cursorMode);
	LCD_voidSendCMD(lcdPtr, 0x06); // auto shifting cursor

	/*	clear display	*/
	LCD_voidClearDisplay(lcdPtr);
}

void LCD_voidSetCursorMode(LCD_t* lcdPtr, HLCD_CursorMode_t cursorMode)
{
	LCD_voidSendCMD(lcdPtr, cursorMode);
}

void LCD_voidClearDisplay(LCD_t* lcdPtr)
{
	LCD_voidSendCMD(lcdPtr, 0x01) ;
	lcdPtr->count = 0;
	/*	here execution time is more that other instructions	*/
	Delay_voidBlockingDelayMs(2);
}

void LCD_voidSendCMD(LCD_t* lcdPtr, u8 cmd)
{
	LCD_voidWaitForCurrentInstruction(lcdPtr);

	/*	select the command register of the LCD-module	*/
	GPIO_voidSetPinOutputLevel(lcdPtr->rsPort, lcdPtr->rsPin, GPIO_OutputLevel_Low);
	
	/*	write cmd to the bus, and LCD-module latch it	*/
	LCD_voidWriteBusAndLatch(lcdPtr, cmd);
	
	/*	wait execution time	*/
	//Delay_voidBlockingDelayMs(3);
}

void LCD_voidSendData(LCD_t* lcdPtr, u8 data)
{
	LCD_voidWaitForCurrentInstruction(lcdPtr);

	/*	select the data register of the LCD-module	*/
	GPIO_voidSetPinOutputLevel(lcdPtr->rsPort, lcdPtr->rsPin, GPIO_OutputLevel_High);
	
	/*	write data to the bus, and LCD-module latch it	*/
	LCD_voidWriteBusAndLatch(lcdPtr, data);
}

void LCD_voidWriteBusAndLatch(LCD_t* lcdPtr, u8 busVal)
{
	/*	select write mode	*/
	GPIO_voidSetPinOutputLevel(lcdPtr->rwPort, lcdPtr->rwPin, GPIO_OutputLevel_Low);
	
	if (lcdPtr->mode == LCD_Mode_8bits)
	{
		/*	write the data at once	*/
		for (u8 i=0; i<8; i++)
			GPIO_voidSetPinOutputLevel(lcdPtr->dataPortArr[i], lcdPtr->dataPinArr[i], GET_BIT(busVal, i));
		LCD_voidLatch(lcdPtr);
	}
	else
	{
		/*	write the data in two times	*/
		/*	higher 4 bits first	*/
		for (u8 i=0; i<4; i++)
			GPIO_voidSetPinOutputLevel(lcdPtr->dataPortArr[i], lcdPtr->dataPinArr[i], GET_BIT(busVal, i+4));
		LCD_voidLatch(lcdPtr);
		/*	lower 4 bits then	*/
		for (u8 i=0; i<4; i++)
			GPIO_voidSetPinOutputLevel(lcdPtr->dataPortArr[i], lcdPtr->dataPinArr[i], GET_BIT(busVal, i));
		LCD_voidLatch(lcdPtr);
	}
}

void LCD_voidLatch(LCD_t* lcdPtr)
{
	/*	make EN pin HIGH, then LOW, to make the LCD-module latch the data bus	*/
	GPIO_voidSetPinOutputLevel(lcdPtr->enPort, lcdPtr->enPin, GPIO_OutputLevel_High);
	Delay_voidBlockingDelayMs(1) ;
	GPIO_voidSetPinOutputLevel(lcdPtr->enPort, lcdPtr->enPin, GPIO_OutputLevel_Low);
}

void LCD_voidPrintString(LCD_t* lcdPtr, char* str)
{
	for (u8 i=0; str[i]!='\0'; i++)
	{
		if (lcdPtr->count % 16 == 0)	// new line:
		{
			if (lcdPtr->count == 64)
				LCD_voidClearDisplay(lcdPtr);
			LCD_voidSendCMD(lcdPtr, DDRAMADD[lcdPtr->count/16]);
		}

		if (str[i] == '\n')
		{
			if (lcdPtr->count == 64)
			{
				LCD_voidClearDisplay(lcdPtr);
				continue;
			}
			u8 line = lcdPtr->count/16 + 1;
			LCD_voidSendCMD(lcdPtr, DDRAMADD[line]);
			lcdPtr->count = line * 16;
			continue;
		}

		LCD_voidSendData(lcdPtr, str[i]);
		lcdPtr->count++;
	}
}

void LCD_voidPrintCharAt(LCD_t* lcdPtr, char ch, u8 row, u8 col)
{
	// move courser to col, row:
	LCD_voidSendCMD(lcdPtr, DDRAMADD[row] + col);

	// print:
	LCD_voidSendData(lcdPtr, ch);

	// change count:
	lcdPtr->count = 16 * row + col + 1;
}

void LCD_voidWaitForCurrentInstruction(LCD_t* lcdPtr)
{
	u8 n = (lcdPtr->mode == LCD_Mode_8bits) ? 8 : 4;

	// make data pins inputs (pull them up for convenience):
	for (u8 i=0; i<n; i++)
		GPIO_voidSetPinInputPullUp(lcdPtr->dataPortArr[i], lcdPtr->dataPinArr[i]);

	// make the LCD module output busy flag on DB7:
	GPIO_voidSetPinOutputLevel(lcdPtr->rsPort, lcdPtr->rsPin, GPIO_OutputLevel_Low);
	GPIO_voidSetPinOutputLevel(lcdPtr->rwPort, lcdPtr->rwPin, GPIO_OutputLevel_High);

	// initially make en pin high (to give -ve edges in the next while loop):
	GPIO_voidSetPinOutputLevel(lcdPtr->enPort, lcdPtr->enPin, GPIO_OutputLevel_High);

	// wait for busy flag to get low:
	while(GPIO_b8ReadPinDigital(lcdPtr->dataPortArr[n-1], lcdPtr->dataPinArr[n-1]))
	{
		// give -ve edges on en pin to refresh:
		GPIO_voidSetPinOutputLevel(lcdPtr->enPort, lcdPtr->enPin, GPIO_OutputLevel_Low);
		GPIO_voidSetPinOutputLevel(lcdPtr->enPort, lcdPtr->enPin, GPIO_OutputLevel_High);
	}

	// make data pins outputs again:
	for (u8 i=0; i<n; i++)
		GPIO_voidSetPinGpoPushPull(lcdPtr->dataPortArr[i], lcdPtr->dataPinArr[i]);
}

void LCD_voidLoadCustomChar (LCD_t* lcdPtr, u8 loc, u8 cChar[])
{
    unsigned char i;
    if(loc<8)
    {
    	LCD_voidSendCMD(lcdPtr, 64 + (loc*8));
    	for(i=0;i<8;i++)
    	{
    		LCD_voidSendData(lcdPtr, cChar[i]);
    	}
    }
}
