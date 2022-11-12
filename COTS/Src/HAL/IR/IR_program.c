/*
 * IR_program.c
 *
 *  Created on: Oct 30, 2022
 *      Author: Ali Emad Ali
 */


/*	LIB	*/
#include "Std_Types.h"
#include "Bit_Math.h"

/*	MCAL	*/
#include "GPIO_interface.h"
#include "STK_interface.h"
#include "EXTI_interface.h"
#include "NVIC_interface.h"

/*	SELF	*/
#include "IR_config.h"
#include "IR_interface.h"

/*	static global variables, defined for speed of comparison in run time.	*/
static u32 ticks9000usMax = 0;
static u32 ticks9000usMin = 0;
static u32 ticks4500usMax = 0;
static u32 ticks4500usMin = 0;
static u32 ticks560usMax  = 0;
static u32 ticks560usMin  = 0;
static u32 ticks1690usMax = 0;
static u32 ticks1690usMin = 0;
static u32 ticks40500usMax = 0;
static u32 ticks40500usMin = 0;
static u32 ticks2250usMax = 0;
static u32 ticks2250usMin = 0;

static void init_time_limits(void)
{
	u32 ticksPerSecond = STK_u32GetTicksPerSecond();

	ticks9000usMax =
		((u64)(9000 + TOLERANCE_9_MS) * (u64)ticksPerSecond) / 1000000;

	ticks9000usMin =
		((u64)(9000 - TOLERANCE_9_MS) * (u64)ticksPerSecond) / 1000000;

	ticks4500usMax =
		((u64)(4500 + TOLERANCE_4500_US) * (u64)ticksPerSecond) / 1000000;

	ticks4500usMin =
		((u64)(4500 - TOLERANCE_4500_US) * (u64)ticksPerSecond) / 1000000;

	ticks560usMax =
		((u64)(560 + TOLERANCE_560_US) * (u64)ticksPerSecond) / 1000000;

	ticks560usMin =
		((u64)(560 - TOLERANCE_560_US) * (u64)ticksPerSecond) / 1000000;

	ticks1690usMax =
		((u64)(1690 + TOLERANCE_1690_US) * (u64)ticksPerSecond) / 1000000;

	ticks1690usMin =
		((u64)(1690 - TOLERANCE_1690_US) * (u64)ticksPerSecond) / 1000000;

	ticks40500usMax =
		((u64)(40500 + TOLERANCE_40500_US) * (u64)ticksPerSecond) / 1000000;

	ticks40500usMin =
		((u64)(40500 - TOLERANCE_40500_US) * (u64)ticksPerSecond) / 1000000;

	ticks2250usMax =
		((u64)(40500 + TOLERANCE_40500_US) * (u64)ticksPerSecond) / 1000000;

	ticks2250usMin =
		((u64)(40500 - TOLERANCE_2250_US) * (u64)ticksPerSecond) / 1000000;
}

/*	active level, depends on configured idle level	*/
#define ACTIVE_LEVEL	(!IDLE_LEVEL)

/*
 * checks if certain number of ticks is equal to x microseconds, considering
 * tolerance configured in "IR.config" file.
 */
#define IS_9000US(ticks)	\
	((ticks9000usMin <= (ticks)  &&  (ticks) <= ticks9000usMax) ? true : false)

#define IS_4500US(ticks)	\
	((ticks4500usMin <= (ticks)  &&  (ticks) <= ticks4500usMax) ? true : false)

#define IS_560US(ticks)		\
	((ticks560usMin <= (ticks)  &&  (ticks) <= ticks560usMax) ? true : false)

#define IS_1690US(ticks)	\
	((ticks1690usMin <= (ticks)  &&  (ticks) <= ticks1690usMax) ? true : false)

#define IS_40500US(ticks)	\
	((ticks40500usMin <= (ticks)  &&  (ticks) <= ticks40500usMax) ? true : false)

#define IS_2250US(ticks)	\
	((ticks2250usMin <= (ticks)  &&  (ticks) <= ticks2250usMax) ? true : false)

/*
 * calls the "IR->receiveCompleteCallback()", clears data
 * queue/register bit counter. and clears "recvStat".
 */
#define MSG_COMPLETED_ROUTINE(IR)  {            \
	IR->receiveCompleteCallback();              \
	IR->bitCounter = 0;                         \
	IR->recvStat = 0;							\
	IR->level = IDLE_LEVEL;}					\

/*
 * pushes new bit to the data word. and checks if 32 bits has been received,
 * if so, it executes "MSG_COMPLETED_ROUTINE" , else wise it writes
 * "IR->recvStat" with a value of 0x02.
 * "IR" is a pointer to an object of structure "IR_Receiver_t".
 */
#define PUSH_BIT(IR, bit)	{							\
	WRT_BIT((IR)->data, ((IR)->bitCounter)++, (bit));	\
	if (IR->bitCounter == 32)							\
	{                                                   \
		MSG_COMPLETED_ROUTINE(IR);						\
	}                                                   \
	else                                                \
		IR->recvStat = 2;}                              \

/*
 * look at documentation of "void (*EXTICallback) (void)" in "IR_Receiver_t".
 */
void IR_voidEXTICallbackTamplate(IR_Receiver_t* IR)
{
	/*	take timestamp of transition	*/
	u64 transitionTimeStamp = STK_u64GetElapsedTicks();

	/*	calculate dt	*/
	u64 dt = transitionTimeStamp - IR->lastTimeStamp;

	/*	store new timestamp	*/
	IR->lastTimeStamp = transitionTimeStamp;

	/*	logic invert current level	*/
	IR->level = !IR->level;

	if(IS_9000US(dt))
	{
		IR->recvStat = 1;
	}

	else if(IS_4500US(dt)  &&  IR->level == IDLE_LEVEL  &&  IR->recvStat == 1)
	{
		IR->recvStat = 2;
	}

	else if(IS_560US(dt))
	{
		if (IR->level == ACTIVE_LEVEL  &&  IR->recvStat == 2)
		{
			IR->recvStat = 3;
		}
		else if (IR->level == IDLE_LEVEL  &&  IR->recvStat == 3)
		{
			PUSH_BIT(IR, 0);
		}
		else
		{
			IR->recvStat = 0;
		}
	}

	else if(IS_1690US(dt)  &&  IR->level == IDLE_LEVEL  &&  IR->recvStat == 3)
	{
		PUSH_BIT(IR, 1);
	}

	else if(IS_2250US(dt)  &&  IR->level == IDLE_LEVEL  &&  IR->recvStat == 0)
	{
		IR->recvStat = 4;
	}

	else
	{
		IR->recvStat = 0;
		IR->level = IDLE_LEVEL;
	}
}

/*
 * for info about the arguments, refer to the documentation of the structure
 * "IR_Receiver_t".
 *
 * EXTI, AFIO, GPIOx peripherals should be RCC clock enabled.
 *
 * EXTI line interrupt is advised to be in higher groups.
 */
void IR_voidInit(
	IR_Receiver_t* IR, u8 extiLine, GPIO_PortName_t port,
	void (*receiveCompleteCallback) (void),
	void (*EXTICallback) (void)
	)
{
	/*	first time only, init "ticking params"	*/
	if (ticks9000usMax == 0)
	{
		init_time_limits();
	}

	/*	store inputs	*/
	IR->extiLine = extiLine;
	IR->receiveCompleteCallback = receiveCompleteCallback;
	IR->EXTICallback = EXTICallback;
	IR->lastTimeStamp = 0;

	/*	set EXTI line callback	*/
	EXTI_voidSetCallBack(extiLine, EXTICallback);

	/*	init object's data	*/
	IR->bitCounter = 0;
	IR->data = 0;
	IR->recvStat = IR_ReceiveStatus_Out;
	IR->level = IDLE_LEVEL;

	/*	init EXTI pin	*/
	AFIO_voidSetExtiLineSrc(extiLine, port);
	GPIO_voidSetPinMode(port, extiLine, GPIO_Mode_Input_Pull);
	GPIO_voidSetPinOutputSpeed(port, extiLine, GPIO_OutputSpeed_Null);
	GPIO_voidSetPinOutputLevel(port, extiLine, GPIO_OutputLevel_High);

	/*	make EXTI trigger on both edges	*/
	EXTI_voidSetTriggeringEdge(extiLine, EXTI_Trigger_bothEdges);

	/*	enable EXTI line and its interrupt	*/
	EXTI_voidEnableLine(extiLine);
	EXTI_voidEnableLineInterrupt(extiLine);
}




























