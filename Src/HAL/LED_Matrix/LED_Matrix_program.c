/*
 * LED_Matrix_program.c
 *
 *  Created on:	Sep 6, 2022
 *  Author:		Ali Emad Ali
 *	
 *	Resources:
 *		*
 *
 */


#include "BIT_MATH.h"
#include "STD_TYPES.h"
#include <stdlib.h>

#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "Shift_Register_interface.h"
#include "Delay_interface.h"

#include "LED_Matrix_interface.h"

void LED_MATRIX_voidInitParallel(
	LED_MATRIX_PARALLEL_t* ledMatrixPtr, u8 _rows, u8 _cols, u16 fps,
	LED_MATRIX_activeType _activeType, DIO_PIN rowPinArr[], DIO_PIN colPinArr[]
	)
{
	ledMatrixPtr->rows = _rows;
	ledMatrixPtr->cols = _cols;

	ledMatrixPtr->rowActiveLevel =
			(_activeType == LED_MATRIX_activeType_colHighRowLow) ?
				DIO_OUTPUTLEVEL_low : DIO_OUTPUTLEVEL_high;

	ledMatrixPtr->rowIntervalTicks = RCC_u32GetSysClk() / fps / _rows;

	ledMatrixPtr->rowPinArr = malloc(_rows * sizeof(u8));
	ledMatrixPtr->rowPortArr = malloc(_rows * sizeof(DIO_PORT));

	ledMatrixPtr->colPinArr = malloc(_cols * sizeof(u8));
	ledMatrixPtr->colPortArr = malloc(_cols * sizeof(DIO_PORT));

	ledMatrixPtr->rowsImg = malloc(_rows * sizeof(u16));
	ledMatrixPtr->imgAllocated = true;

	for (u8 i=0; i<_rows; i++)
	{
		ledMatrixPtr->rowPortArr[i] = rowPinArr[i] / 16;
		ledMatrixPtr->rowPinArr[i] = rowPinArr[i] % 16;
		DIO_voidSetPinMode(ledMatrixPtr->rowPortArr[i], ledMatrixPtr->rowPinArr[i], DIO_MODE_GPO);
		DIO_voidSetPinOutputType(ledMatrixPtr->rowPortArr[i], ledMatrixPtr->rowPinArr[i], DIO_OUTPUTTYPE_pushPull);
		DIO_voidSetPinOutputSpeed(ledMatrixPtr->rowPortArr[i], ledMatrixPtr->rowPinArr[i], DIO_OUTPUTSPEED_veryHigh);

		// initially turn off all LEDs:
		DIO_voidSetPinOutputLevel(ledMatrixPtr->rowPortArr[i], ledMatrixPtr->rowPinArr[i], !(ledMatrixPtr->rowActiveLevel));
	}

	for (u8 i=0; i<_rows; i++)
	{
		ledMatrixPtr->colPortArr[i] = colPinArr[i] / 16;
		ledMatrixPtr->colPinArr[i] = colPinArr[i] % 16;
		DIO_voidSetPinMode(ledMatrixPtr->colPortArr[i], ledMatrixPtr->colPinArr[i], DIO_MODE_GPO);
		DIO_voidSetPinOutputSpeed(ledMatrixPtr->colPortArr[i], ledMatrixPtr->colPinArr[i], DIO_OUTPUTSPEED_veryHigh);
	}
}

void LED_MATRIX_voidDrawRowParallel(LED_MATRIX_PARALLEL_t* ledMatrixPtr, u8 row)
{
	// enable row:
	DIO_voidSetPinOutputLevel(
		ledMatrixPtr->rowPortArr[row],
		ledMatrixPtr->rowPinArr[row],
		ledMatrixPtr->rowActiveLevel
		);

	// lightening this row's LEDs according to the stored image:
	for (u8 col = 0; col < ledMatrixPtr->cols; col++)
	{
		DIO_voidSetPinOutputLevel(
			ledMatrixPtr->colPortArr[col],
			ledMatrixPtr->colPinArr[col],
			ledMatrixPtr->rowActiveLevel ^ GET_BIT(ledMatrixPtr->rowsImg[row], col)
			);
	}
}

void LED_MATRIX_voidDisableRowParallel(LED_MATRIX_PARALLEL_t* ledMatrixPtr, u8 row)
{
	DIO_voidSetPinOutputLevel(
		ledMatrixPtr->rowPortArr[row],
		ledMatrixPtr->rowPinArr[row],
		!(ledMatrixPtr->rowActiveLevel)
		);

	// (optional) disable cols to avoid shade repeatance in next row:
	for (u8 col = 0; col < ledMatrixPtr->cols; col++)
	{
		DIO_voidSetPinOutputLevel(
			ledMatrixPtr->colPortArr[col],
			ledMatrixPtr->colPinArr[col],
			ledMatrixPtr->rowActiveLevel
			);
	}
}

void LED_MATRIX_voidDrawBlockingParallel(LED_MATRIX_PARALLEL_t* ledMatrixPtr, u32 frames)
{
	while(frames--)
	{
		// for every row:
		for (u8 row = 0; row < ledMatrixPtr->rows; row++)
		{
			// enable row:
			LED_MATRIX_voidDrawRowParallel(ledMatrixPtr, row);

			// wait multiplexing time:
			Delay_voidBlockingDelayTicks(ledMatrixPtr->rowIntervalTicks);

			// disable row:
			LED_MATRIX_voidDisableRowParallel(ledMatrixPtr, row);
		}
	}
}

void LED_MATRIX_voidStoreImgParallel(LED_MATRIX_PARALLEL_t* ledMatrixPtr, u16 _rowsImg[])
{
	/*	allocate if not allocated	*/
	if (!ledMatrixPtr->imgAllocated)
	{
		ledMatrixPtr->rowsImg = malloc(ledMatrixPtr->rows * sizeof(u16));
		ledMatrixPtr->imgAllocated = true;
	}

	/*	copy	*/
	for (u8 row = 0; row < ledMatrixPtr->rows; row++)
		ledMatrixPtr->rowsImg[row] = _rowsImg[row];

	/*	clear counter	*/
	ledMatrixPtr->nextRow = 0;
}

/*	freeing allocated image - if there is - is user's responsibility!	*/
void LED_MATRIX_voidLinkImgParallel(LED_MATRIX_PARALLEL_t* ledMatrixPtr, u16* _rowsImgPtr)
{
	ledMatrixPtr->rowsImg = _rowsImgPtr;

	/*	clear counter	*/
	ledMatrixPtr->nextRow = 0;
}

void LED_MATRIX_voidFreeImgParallel(LED_MATRIX_PARALLEL_t* ledMatrixPtr)
{
	if (ledMatrixPtr->imgAllocated)
	{
		free(ledMatrixPtr->rowsImg);
		ledMatrixPtr->imgAllocated = false;
	}
}

void LED_MATRIX_voidDrawNextRowParallel(LED_MATRIX_PARALLEL_t* ledMatrixPtr)
{
	/*	disable current row:	*/
	u8 currentRow =
		(ledMatrixPtr->nextRow == 0) ?
				ledMatrixPtr->rows - 1 : ledMatrixPtr->nextRow - 1;

	LED_MATRIX_voidDisableRowParallel(ledMatrixPtr, currentRow);

	/*	draw next row:	*/
	LED_MATRIX_voidDrawRowParallel(ledMatrixPtr, ledMatrixPtr->nextRow);

	/*	increment next row, check row OVF:	*/
	ledMatrixPtr->nextRow++;

	if (ledMatrixPtr->nextRow == ledMatrixPtr->rows)
		ledMatrixPtr->nextRow = 0;
}

void LED_MATRIX_voidInitSerial(
	LED_MATRIX_SERIAL_t* ledMatrixPtr, u8 _rows, u8 _cols, u16 fps,
	LED_MATRIX_activeType _activeType, SIPO_SHIFT_REGISTER_t* _shiftRegsiterPtr
	)
{
	ledMatrixPtr->rows = _rows;
	ledMatrixPtr->cols = _cols;

	ledMatrixPtr->rowActiveLevel =
		(_activeType == LED_MATRIX_activeType_colHighRowLow) ?
			DIO_OUTPUTLEVEL_low : DIO_OUTPUTLEVEL_high;

	ledMatrixPtr->rowIntervalTicks = RCC_u32GetSysClk() / fps / _rows;

	ledMatrixPtr->rowsImg = malloc(_rows * sizeof(u16));
	ledMatrixPtr->imgAllocated = true;

	ledMatrixPtr->shiftRegsiterPtr = _shiftRegsiterPtr;

	ledMatrixPtr->nextRow = 0;

	// initially all LEDs are off:
	LED_MATRIX_voidBlankSerial(ledMatrixPtr);
}

void LED_MATRIX_voidBlankSerial(LED_MATRIX_SERIAL_t* ledMatrixPtr)
{
	/*	give all rows a state that is inverse to their active one	*/
	u8 level = !(ledMatrixPtr->rowActiveLevel);

	for (u8 row=0; row < ledMatrixPtr->rows; row++)
	{
		SHIFT_REGISTER_voidShiftSIPO(ledMatrixPtr->shiftRegsiterPtr, level);
	}

	/*	latch	*/
	SHIFT_REGISTER_voidLatchSIPO(ledMatrixPtr->shiftRegsiterPtr);

	/*	reset row counter	*/
	ledMatrixPtr->nextRow = 0;
}

void LED_MATRIX_voidStoreImgSerial(LED_MATRIX_SERIAL_t* ledMatrixPtr, u16 _rowsImg[])
{
	/*	allocate if not allocated	*/
	if (!ledMatrixPtr->imgAllocated)
	{
		ledMatrixPtr->rowsImg = malloc(ledMatrixPtr->rows * sizeof(u16));
		ledMatrixPtr->imgAllocated = true;
	}

	/*	copy	*/
	for (u8 row = 0; row < ledMatrixPtr->rows; row++)
		ledMatrixPtr->rowsImg[row] = _rowsImg[row];

	/*	clear counter	*/
	ledMatrixPtr->nextRow = 0;
}

void LED_MATRIX_voidDrawRowSerial(LED_MATRIX_SERIAL_t* ledMatrixPtr, u8 row)
{
	/*	assuming that each of rows and cols won't be more than 16 in quantity	*/
	// enable row (disables all other rows):
	u32 shiftRegVal = (ledMatrixPtr->rowActiveLevel) << row;

	// lightening this row's LEDs according to the stored image:
	for (u8 col = 0; col < ledMatrixPtr->cols; col++)
	{
		u8 bit = ledMatrixPtr->rowActiveLevel ^ GET_BIT(ledMatrixPtr->rowsImg[row], col);
		shiftRegVal |= bit << (ledMatrixPtr->rows + col);
	}

	/*	load and latch	*/
	SHIFT_REGISTER_voidLoadFromU32SIPO(ledMatrixPtr->shiftRegsiterPtr, shiftRegVal, ledMatrixPtr->rows + ledMatrixPtr->cols);

	/*	latch	*/
	SHIFT_REGISTER_voidLatchSIPO(ledMatrixPtr->shiftRegsiterPtr);
}

void LED_MATRIX_voidDrawBlockingSerial(LED_MATRIX_SERIAL_t* ledMatrixPtr, u32 frames)
{
	while(frames--)
	{
		// for every row:
		for (u8 row = 0; row < ledMatrixPtr->rows; row++)
		{
			// draw row:
			LED_MATRIX_voidDrawRowSerial(ledMatrixPtr, row);

			// wait multiplexing time:
			Delay_voidBlockingDelayTicks(ledMatrixPtr->rowIntervalTicks);
		}
	}
}

void LED_MATRIX_voidDrawNextRowSerial(LED_MATRIX_SERIAL_t* ledMatrixPtr)
{
	/*	enable next row (disables all other row):	*/
	LED_MATRIX_voidDrawRowSerial(ledMatrixPtr, ledMatrixPtr->nextRow);

	/*	increment next row, check row OVF:	*/
	ledMatrixPtr->nextRow++;

	if (ledMatrixPtr->nextRow == ledMatrixPtr->rows)
		ledMatrixPtr->nextRow = 0;
}


