/*
 * Core_Regs_GP.h
 *
 *  Created on: Mar 18, 2023
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_MCAL_CORE_REGS_CORE_REGS_GP_H_
#define INCLUDE_MCAL_CORE_REGS_CORE_REGS_GP_H_

#include "Std_Types.h"
#include "Bit_Math.h"

/*******************************************************************************
 * Writing:
 ******************************************************************************/
ALWAYS_INLINE_STATIC void Core_Regs_voidWriteGPR0(u32 val)
{
	__asm volatile("MOV R0, %0" :: "r" (val) :);
}

ALWAYS_INLINE_STATIC void Core_Regs_voidWriteGPR1(u32 val)
{
	__asm volatile("MOV R1, %0" :: "r" (val) :);
}

ALWAYS_INLINE_STATIC void Core_Regs_voidWriteGPR2(u32 val)
{
	__asm volatile("MOV R2, %0" :: "r" (val) :);
}

ALWAYS_INLINE_STATIC void Core_Regs_voidWriteGPR3(u32 val)
{
	__asm volatile("MOV R3, %0" :: "r" (val) :);
}

ALWAYS_INLINE_STATIC void Core_Regs_voidWriteGPR4(u32 val)
{
	__asm volatile("MOV R4, %0" :: "r" (val) :);
}

ALWAYS_INLINE_STATIC void Core_Regs_voidWriteGPR5(u32 val)
{
	__asm volatile("MOV R5, %0" :: "r" (val) :);
}

ALWAYS_INLINE_STATIC void Core_Regs_voidWriteGPR6(u32 val)
{
	__asm volatile("MOV R6, %0" :: "r" (val) :);
}

ALWAYS_INLINE_STATIC void Core_Regs_voidWriteGPR7(u32 val)
{
	__asm volatile("MOV R7, %0" :: "r" (val) :);
}

ALWAYS_INLINE_STATIC void Core_Regs_voidWriteGPR8(u32 val)
{
	__asm volatile("MOV R8, %0" :: "r" (val) :);
}

ALWAYS_INLINE_STATIC void Core_Regs_voidWriteGPR9(u32 val)
{
	__asm volatile("MOV R9, %0" :: "r" (val) :);
}

ALWAYS_INLINE_STATIC void Core_Regs_voidWriteGPR10(u32 val)
{
	__asm volatile("MOV R10, %0" :: "r" (val) :);
}

ALWAYS_INLINE_STATIC void Core_Regs_voidWriteGPR11(u32 val)
{
	__asm volatile("MOV R11, %0" :: "r" (val) :);
}

ALWAYS_INLINE_STATIC void Core_Regs_voidWriteGPR12(u32 val)
{
	__asm volatile("MOV R12, %0" :: "r" (val) :);
}

ALWAYS_INLINE_STATIC void Core_Regs_voidWriteGPR(u8 regNumber, u32 val)
{
	switch(regNumber)
	{
	case 0:
		__asm volatile("MOV R0, %0" :: "r" (val) :);
		break;

	case 1:
		__asm volatile("MOV R1, %0" :: "r" (val) :);
		break;

	case 2:
		__asm volatile("MOV R2, %0" :: "r" (val) :);
		break;

	case 3:
		__asm volatile("MOV R3, %0" :: "r" (val) :);
		break;

	case 4:
		__asm volatile("MOV R4, %0" :: "r" (val) :);
		break;

	case 5:
		__asm volatile("MOV R5, %0" :: "r" (val) :);
		break;

	case 6:
		__asm volatile("MOV R6, %0" :: "r" (val) :);
		break;

	case 7:
		__asm volatile("MOV R7, %0" :: "r" (val) :);
		break;

	case 8:
		__asm volatile("MOV R8, %0" :: "r" (val) :);
		break;

	case 9:
		__asm volatile("MOV R9, %0" :: "r" (val) :);
		break;

	case 10:
		__asm volatile("MOV R10, %0" :: "r" (val) :);
		break;

	case 11:
		__asm volatile("MOV R11, %0" :: "r" (val) :);
		break;

	case 12:
		__asm volatile("MOV R12, %0" :: "r" (val) :);
		break;
	}
}

/*******************************************************************************
 * Reading:
 ******************************************************************************/
ALWAYS_INLINE_STATIC u32 Core_Regs_u32ReadGPR0(void)
{
	volatile u32 val = 0;
	volatile u32* valPtr = &val;

	__asm volatile("STR R0, [%0]" :: "r" (valPtr) :);

	return val;
}

ALWAYS_INLINE_STATIC u32 Core_Regs_u32ReadGPR1(void)
{
	volatile u32 val = 0;
	volatile u32* valPtr = &val;

	__asm volatile("STR R1, [%0]" :: "r" (valPtr) :);

	return val;
}

ALWAYS_INLINE_STATIC u32 Core_Regs_u32ReadGPR2(void)
{
	volatile u32 val = 0;
	volatile u32* valPtr = &val;

	__asm volatile("STR R2, [%0]" :: "r" (valPtr) :);

	return val;
}

ALWAYS_INLINE_STATIC u32 Core_Regs_u32ReadGPR3(void)
{
	volatile u32 val = 0;
	volatile u32* valPtr = &val;

	__asm volatile("STR R3, [%0]" :: "r" (valPtr) :);

	return val;
}

ALWAYS_INLINE_STATIC u32 Core_Regs_u32ReadGPR4(void)
{
	volatile u32 val = 0;
	volatile u32* valPtr = &val;

	__asm volatile("STR R4, [%0]" :: "r" (valPtr) :);

	return val;
}

ALWAYS_INLINE_STATIC u32 Core_Regs_u32ReadGPR5(void)
{
	volatile u32 val = 0;
	volatile u32* valPtr = &val;

	__asm volatile("STR R5, [%0]" :: "r" (valPtr) :);

	return val;
}

ALWAYS_INLINE_STATIC u32 Core_Regs_u32ReadGPR6(void)
{
	volatile u32 val = 0;
	volatile u32* valPtr = &val;

	__asm volatile("STR R6, [%0]" :: "r" (valPtr) :);

	return val;
}

ALWAYS_INLINE_STATIC u32 Core_Regs_u32ReadGPR7(void)
{
	volatile u32 val = 0;
	volatile u32* valPtr = &val;

	__asm volatile("STR R7, [%0]" :: "r" (valPtr) :);

	return val;
}

ALWAYS_INLINE_STATIC u32 Core_Regs_u32ReadGPR8(void)
{
	volatile u32 val = 0;
	volatile u32* valPtr = &val;

	__asm volatile("STR R8, [%0]" :: "r" (valPtr) :);

	return val;
}

ALWAYS_INLINE_STATIC u32 Core_Regs_u32ReadGPR9(void)
{
	volatile u32 val = 0;
	volatile u32* valPtr = &val;

	__asm volatile("STR R9, [%0]" :: "r" (valPtr) :);

	return val;
}

ALWAYS_INLINE_STATIC u32 Core_Regs_u32ReadGPR10(void)
{
	volatile u32 val = 0;
	volatile u32* valPtr = &val;

	__asm volatile("STR R10, [%0]" :: "r" (valPtr) :);

	return val;
}

ALWAYS_INLINE_STATIC u32 Core_Regs_u32ReadGPR11(void)
{
	volatile u32 val = 0;
	volatile u32* valPtr = &val;

	__asm volatile("STR R11, [%0]" :: "r" (valPtr) :);

	return val;
}

ALWAYS_INLINE_STATIC u32 Core_Regs_u32ReadGPR12(void)
{
	volatile u32 val = 0;
	volatile u32* valPtr = &val;

	__asm volatile("STR R12, [%0]" :: "r" (valPtr) :);

	return val;
}

ALWAYS_INLINE_STATIC u32 Core_Regs_u32ReadGPR(u8 regNumber)
{
	volatile u32 val = 0;
	volatile u32* valPtr = &val;

	switch(regNumber)
	{
	case 0:
		__asm volatile("STR R0, [%0]" :: "r" (valPtr) :);
		break;

	case 1:
		__asm volatile("STR R1, [%0]" :: "r" (valPtr) :);
		break;

	case 2:
		__asm volatile("STR R2, [%0]" :: "r" (valPtr) :);
		break;

	case 3:
		__asm volatile("STR R3, [%0]" :: "r" (valPtr) :);
		break;

	case 4:
		__asm volatile("STR R4, [%0]" :: "r" (valPtr) :);
		break;

	case 5:
		__asm volatile("STR R5, [%0]" :: "r" (valPtr) :);
		break;

	case 6:
		__asm volatile("STR R6, [%0]" :: "r" (valPtr) :);
		break;

	case 7:
		__asm volatile("STR R7, [%0]" :: "r" (valPtr) :);
		break;

	case 8:
		__asm volatile("STR R8, [%0]" :: "r" (valPtr) :);
		break;

	case 9:
		__asm volatile("STR R9, [%0]" :: "r" (valPtr) :);
		break;

	case 10:
		__asm volatile("STR R10, [%0]" :: "r" (valPtr) :);
		break;

	case 11:
		__asm volatile("STR R11, [%0]" :: "r" (valPtr) :);
		break;

	case 12:
		__asm volatile("STR R12, [%0]" :: "r" (valPtr) :);
		break;
	}

	return val;
}



#endif /* INCLUDE_MCAL_CORE_REGS_CORE_REGS_GP_H_ */
