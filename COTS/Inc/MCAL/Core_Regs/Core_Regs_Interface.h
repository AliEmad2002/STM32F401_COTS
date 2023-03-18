/*
 * Core_Regs_Interface.h
 *
 *  Created on: Mar 18, 2023
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_MCAL_CORE_REGS_CORE_REGS_INTERFACE_H_
#define INCLUDE_MCAL_CORE_REGS_CORE_REGS_INTERFACE_H_

#include "Std_Types.h"
#include "Bit_Math.h"

/*******************************************************************************
 * accessing general-purpose registers (R0-R12):
 ******************************************************************************/
#include "Core_Regs_GP.h"

/*******************************************************************************
 * accessing stack pointer (R13):
 * Notice that accessing main stack pointer (MSB) requires privileged access.
 ******************************************************************************/
ALWAYS_INLINE_STATIC void Core_Regs_voidWritePSP(u32 val)
{
	__asm volatile("MSR psp, %0" :: "r" (val) :);
}

ALWAYS_INLINE_STATIC u32 Core_Regs_u32ReadPSP(void)
{
	u32 val;
	__asm volatile("MRS %0, psp" : "=r" (val));
	return val;
}

ALWAYS_INLINE_STATIC void Core_Regs_voidWriteMSP(u32 val)
{
	__asm volatile("MSR msp, %0" :: "r" (val) :);
}

ALWAYS_INLINE_STATIC u32 Core_Regs_u32ReadMSP(void)
{
	u32 val;
	__asm volatile("MRS %0, msp" : "=r" (val));
	return val;
}

/*******************************************************************************
 * accessing link register (R14):
 ******************************************************************************/
ALWAYS_INLINE_STATIC void Core_Regs_voidWriteLR(u32 val)
{
	__asm volatile("MOV LR, %0" :: "r" (val) :);
}

ALWAYS_INLINE_STATIC u32 Core_Regs_u32ReadLR(void)
{
	u32 val;
	__asm volatile("MOV %0, LR" : "=r" (val));
	return val;
}

/*******************************************************************************
 * accessing program counter (R15):
 ******************************************************************************/
ALWAYS_INLINE_STATIC void Core_Regs_voidWritePC(u32 val)
{
	__asm volatile("BX %0" :: "r" (val) :);
	//__asm volatile("MOV PC, %0" :: "r" (val) :);
}

ALWAYS_INLINE_STATIC u32 Core_Regs_u32ReadPC(void)
{
	u32 val;
	__asm volatile("MOV %0, PC" : "=r" (val));
	return val;
}

/*******************************************************************************
 * accessing program status register (R16):
 * (may require privileged access).
 ******************************************************************************/
ALWAYS_INLINE_STATIC void Core_Regs_voidWriteAPSR(u32 val)
{
	__asm volatile("MSR APSR_nzcvq, %0" :: "r" (val) :);
}

ALWAYS_INLINE_STATIC u32 Core_Regs_u32ReadPSR(void)
{
	u32 val;
	__asm volatile("MRS %0, PSR" : "=r" (val));
	return val;
}

/*******************************************************************************
 * accessing PRIMASK register (R17):
 * Notice: requires privileged access.
 ******************************************************************************/
ALWAYS_INLINE_STATIC void Core_Regs_voidWritePRIMASK(b8 val)
{
	__asm volatile("MSR PRIMASK, %0" :: "r" (val) :);
}

ALWAYS_INLINE_STATIC b8 Core_Regs_b8ReadPRIMASK(void)
{
	u32 val;
	__asm volatile("MRS %0, PRIMASK" : "=r" (val));
	return val;
}

/*******************************************************************************
 * accessing FAULTMASK register (R18):
 * Notice: requires privileged access.
 ******************************************************************************/
ALWAYS_INLINE_STATIC void Core_Regs_voidWriteFAULTMASK(b8 val)
{
	__asm volatile("MSR FAULTMASK, %0" :: "r" (val) :);
}

ALWAYS_INLINE_STATIC b8 Core_Regs_b8ReadFAULTMASK(void)
{
	u32 val;
	__asm volatile("MRS %0, FAULTMASK" : "=r" (val));
	return val;
}

/*******************************************************************************
 * accessing BASEPRI register (R19):
 * Notice: requires privileged access.
 ******************************************************************************/
ALWAYS_INLINE_STATIC void Core_Regs_voidWriteBASEPRI(u8 val)
{
	__asm volatile("MSR BASEPRI, %0" :: "r" (val) :);
}

ALWAYS_INLINE_STATIC u8 Core_Regs_u8ReadBASEPRI(void)
{
	u32 val;
	__asm volatile("MRS %0, BASEPRI" : "=r" (val));
	return val;
}

/*******************************************************************************
 * accessing control register (R20):
 * Notice: requires privileged access.
 ******************************************************************************/
ALWAYS_INLINE_STATIC void Core_Regs_voidWriteCR(u8 val)
{
	__asm volatile("MSR CONTROL, %0" :: "r" (val) :);
}

ALWAYS_INLINE_STATIC u8 Core_Regs_u8ReadCR(void)
{
	u32 val;
	__asm volatile("MRS %0, CONTROL" : "=r" (val));
	return val;
}


#endif /* INCLUDE_MCAL_CORE_REGS_CORE_REGS_INTERFACE_H_ */








