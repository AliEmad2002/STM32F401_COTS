/*
 * FPEC_program.c
 *
 *  Created on: Oct 18, 2022
 *      Author: Ali Emad Ali
 */


/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"

/*	SELF	*/
#include "FPEC_private.h"
#include "FPEC_interface.h"


b8 FPEC_b8IsLocked(void)
{
	return GET_BIT(FPEC->CR, FPEC_CR_LOCK);
}

b8 FPEC_b8ReadFlag(FPEC_Flag_t flag)
{
	return GET_BIT(FPEC->SR, flag);
}

void FPEC_voidClearFlag(FPEC_Flag_t flag)
{
	SET_BIT(FPEC->SR, flag);
}

void FPEC_voidUnlock(void)
{
	FPEC->KEYR = FPEC_KEY1;
	FPEC->KEYR = FPEC_KEY2;
}

void FPEC_voidLock(void)
{
	SET_BIT(FPEC->CR, FPEC_CR_LOCK);
}

void FPEC_voidEnableMassEraseMode(void)
{
	SET_BIT(FPEC->CR, FPEC_CR_MER);
}

void FPEC_voidDisableMassEraseMode(void)
{
	CLR_BIT(FPEC->CR, FPEC_CR_MER);
}

void FPEC_voidEnablePageEraseMode(void)
{
	SET_BIT(FPEC->CR, FPEC_CR_PER);
}

void FPEC_voidDisablePageEraseMode(void)
{
	CLR_BIT(FPEC->CR, FPEC_CR_PER);
}

void FPEC_voidStartEraseOperation(void)
{
	SET_BIT(FPEC->CR, FPEC_CR_START);
}

void FPEC_voidEnableProgrammingMode(void)
{
	SET_BIT(FPEC->CR, FPEC_CR_PG);
}

void FPEC_voidDisableProgrammingMode(void)
{
	CLR_BIT(FPEC->CR, FPEC_CR_PG);
}

void FPEC_voidSelectFlashAccessLatency(FPEC_Latency_t latency)
{
	EDT_REG(FPEC->ACR, FPEC_ACR_LATENCY_0, latency, 3);
}

/*
 * before erasing user should:
 * 	-	unlock the FPEC.
 * 	-	enable page erasing mode.
 * 	deactivating page erasing mode after erasing
 * 	is user's responsibility.
 */
void FPEC_voidErasePage(u8 pageNumber)
{
	/*	wait for busy flag	*/
	while(FPEC_b8ReadFlag(FPEC_Flag_BSY));

	/*	write page address on FPEC address register	*/
	FPEC->AR = FPEC_PAGE_ADDRESS(pageNumber);

	/*	start erase operation	*/
	FPEC_voidStartEraseOperation();

	/*	wait for busy flag	*/
	while(FPEC_b8ReadFlag(FPEC_Flag_BSY));

	/*	clear End Of Operation flag	*/
	FPEC_voidClearFlag(FPEC_Flag_EOP);
}

void FPEC_voidEraseMultiplePages(u8 startPageNumber, u8 endPageNumber)
{
	for (; startPageNumber <= endPageNumber; startPageNumber++)
	{
		FPEC_voidErasePage(startPageNumber);
	}
}

void FPEC_voidSafeProgramHalfWord(u32 flashAddress, u16 data)
{
	/*	unlock FPEC - if locked - 	*/
	if (FPEC_b8IsLocked())
		FPEC_voidUnlock();

	/*	enable programming mode	*/
	FPEC_voidEnableProgrammingMode();

	/*	program	*/
	FPEC_voidProgramHalfWord(flashAddress, data);

	/*	disable programming mode	*/
	FPEC_voidDisableProgrammingMode();

	/*	lock	*/
	FPEC_voidLock();
}

void FPEC_voidSafeProgramWord(u32 flashAddress, u32 data)
{
	/*	unlock FPEC - if locked - 	*/
	if (FPEC_b8IsLocked())
		FPEC_voidUnlock();

	/*	enable programming mode	*/
	FPEC_voidEnableProgrammingMode();

	/*	program	*/
	FPEC_voidProgramWord(flashAddress, data);

	/*	disable programming mode	*/
	FPEC_voidDisableProgrammingMode();

	/*	lock	*/
	FPEC_voidLock();
}

/*
 * before programming user should:
 * 	-	unlock the FPEC.
 * 	-	enable programming mode.
 * 	deactivating programming mode after programming
 * 	is user's responsibility.
 */
void FPEC_voidProgramHalfWord(u32 flashAddress, u16 data)
{
	/*	wait for busy flag	*/
	while(FPEC_b8ReadFlag(FPEC_Flag_BSY));

	/*	start write operation	*/
	*((volatile u16*)flashAddress) = data;

	/*	wait for busy flag	*/
	while(FPEC_b8ReadFlag(FPEC_Flag_BSY));

	/*	clear End Of Operation flag	*/
	FPEC_voidClearFlag(FPEC_Flag_EOP);
}

void FPEC_voidProgramWord(u32 flashAddress, u32 data)
{
	/*	wait for busy flag	*/
	while(FPEC_b8ReadFlag(FPEC_Flag_BSY));

	/*	write lower hWord	*/
	*((volatile u16*)flashAddress) = data & 0xFFFF;

	/*	wait for busy flag	*/
	while(FPEC_b8ReadFlag(FPEC_Flag_BSY));

	/*	write higher hWord	*/
	*((volatile u16*)(flashAddress+2)) = data >> 16;

	/*	wait for busy flag	*/
	while(FPEC_b8ReadFlag(FPEC_Flag_BSY));

	/*	clear End Of Operation flag	*/
	FPEC_voidClearFlag(FPEC_Flag_EOP);
}

void FPEC_voidProgram(u32 flashAddress, u16 data[], u16 len)
{
	/*	wait for busy flag	*/
	while(FPEC_b8ReadFlag(FPEC_Flag_BSY));

	/*	start write operation	*/
	volatile u16* flashHalfWordPtr = (volatile u16*)flashAddress;

	for (u16 i = 0; i < len; i++)
	{
		flashHalfWordPtr[i] = data[i];
		/*	wait for busy flag	*/
		while(FPEC_b8ReadFlag(FPEC_Flag_BSY));
		/*	clear End Of Operation flag	*/
		FPEC_voidClearFlag(FPEC_Flag_EOP);
	}
}

/*
 * before reading user should:
 * 	-	unlock the FPEC.
 * locking FPEC after reading is user's responsibility.
 */
u16 FPEC_u16ReadHalfWord(u32 flashAddress)
{
	/*	wait for busy flag	*/
	while(FPEC_b8ReadFlag(FPEC_Flag_BSY));

	/*	read data	*/
	u16 data = *((volatile u16*)flashAddress);

	/*	wait for busy flag	*/
	while(FPEC_b8ReadFlag(FPEC_Flag_BSY));

	/*	clear End Of Operation flag	*/
	FPEC_voidClearFlag(FPEC_Flag_EOP);

	return data;
}

u32 FPEC_u32ReadWord(u32 flashAddress)
{
	/*	wait for busy flag	*/
	while(FPEC_b8ReadFlag(FPEC_Flag_BSY));

	u32 data;

	/*	read lower hWord	*/
	data = *((volatile u16*)flashAddress);

	/*	wait for busy flag	*/
	while(FPEC_b8ReadFlag(FPEC_Flag_BSY));

	/*	read higher hWord	*/
	data |= ((*((volatile u16*)(flashAddress + 2))) << 16);

	/*	wait for busy flag	*/
	while(FPEC_b8ReadFlag(FPEC_Flag_BSY));

	/*	clear End Of Operation flag	*/
	FPEC_voidClearFlag(FPEC_Flag_EOP);

	return data;
}














