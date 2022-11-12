/*
 * FPEC_interface.h
 *
 *  Created on: Oct 18, 2022
 *      Author: Ali Emad Ali
 */

#ifndef _FPEC_INTERFACE_H_
#define _FPEC_INTERFACE_H_


typedef enum{
	FPEC_Latency_Zero,
	FPEC_Latency_One,
	FPEC_Latency_Two
}FPEC_Latency_t;

typedef enum{
	FPEC_Flag_BSY		= 0,
	FPEC_Flag_PGERR		= 2,
	FPEC_Flag_WRPRTERR	= 4,
	FPEC_Flag_EOP		= 5
}FPEC_Flag_t;

b8 FPEC_b8IsLocked(void);

b8 FPEC_b8ReadFlag(FPEC_Flag_t flag);

void FPEC_voidClearFlag(FPEC_Flag_t flag);

void FPEC_voidUnlock(void);

void FPEC_voidLock(void);

void FPEC_voidEnableMassEraseMode(void);

void FPEC_voidDisableMassEraseMode(void);

void FPEC_voidEnablePageEraseMode(void);

void FPEC_voidDisablePageEraseMode(void);

void FPEC_voidStartEraseOperation(void);

void FPEC_voidEnableProgrammingMode(void);

void FPEC_voidDisableProgrammingMode(void);

void FPEC_voidSelectFlashAccessLatency(FPEC_Latency_t latency);

#define FPEC_PAGE_ADDRESS(page)				(0x08000000 + 1024 * (page))

#define FPEC_HALF_WORD_ADDRESS(page, i)		(FPEC_PAGE_ADDRESS(page) + 2 * (i))

#define FPEC_WORD_ADDRESS(page, i)			(FPEC_PAGE_ADDRESS(page) + 4 * (i))

/*
 * before erasing user should:
 * 	-	unlock the FPEC.
 * 	-	enable page erasing mode.
 * 	locking FPEC and deactivating page erasing mode
 * 	after erasing is user's responsibility.
 */
void FPEC_voidErasePage(u8 pageNumber);

void FPEC_voidEraseMultiplePages(u8 startPageNumber, u8 endPageNumber);

void FPEC_voidSafeProgramHalfWord(u32 flashAddress, u16 data);

void FPEC_voidSafeProgramWord(u32 flashAddress, u32 data);

/*
 * before programming user should:
 * 	-	unlock the FPEC.
 * 	-	enable programming mode.
 * 	locking FPEC and deactivating programming mode
 * 	after programming is user's responsibility.
 */
void FPEC_voidProgramHalfWord(u32 flashAddress, u16 data);
void FPEC_voidProgramWord(u32 flashAddress, u32 data);
void FPEC_voidProgram(u32 flashAddress, u16 data[], u16 len);

/*
 * before reading user should:
 * 	-	unlock the FPEC.
 * locking FPEC after reading is user's responsibility.
 */
u16 FPEC_u16ReadHalfWord(u32 flashAddress);
u32 FPEC_u32ReadWord(u32 flashAddress);


#endif /* _FPEC_INTERFACE_H_ */
