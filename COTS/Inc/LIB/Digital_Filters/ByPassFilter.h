/*
 * ByPassFilter.h
 *
 *  Created on: May 27, 2023
 *      Author: Ali Emad
 *
 * 	This structure uses built-in timer, and its interrupt. The timer interrupt
 * 	is given zero priority (Highest) to guarantee proper sampling time.
 *
 * 	Please check that "ByPassFilter_voidFilter()" have execution time smaller
 * 	than 1/Fs. Otherwise the filter would not work properly.
 */

#ifndef INCLUDE_LIB_DIGITAL_FILTERS_BYPASSFILTER_H_
#define INCLUDE_LIB_DIGITAL_FILTERS_BYPASSFILTER_H_


typedef struct{
	f32 c[3];	// constants of the filter.
	f32 xPrev[2];
	f32 yPrev[2];
	f32(*getSample)(void);
	void(*callback)(f32);
	u8 timerUnitNumber;
}ByPassFilter_t;

/*
 * Initialized with:
 * 		-	middle frequency   (wm).
 * 		-	sampling frequency (Fs).
 * 		-	gain               (k).
 * 		-	quality factor     (Q).
 * 		-	getter function to get the new sample.
 * 		-	callback function to be called after each sample, with filtered value
 * 			passed to it.
 * 		-	timer unit to be used.
 * 		-	NVIC priority of the timer handler.
 */
void ByPassFilter_voidInit(
	ByPassFilter_t* f, f32 wm, f32 fs, f32 k, f32 q,
	f32(*getter)(void), void(*callback)(f32),
	u8 timerUnitNumber, u8 priGroup, u8 priSub
);

/*
 * This function must be put by user in update_handler of the used timer.
 * (This is an inline function)
 */
void ByPassFilter_voidFilter(ByPassFilter_t* f);

void ByPassFilter_voidEnableFilter(ByPassFilter_t* f);

void ByPassFilter_voidDisableFilter(ByPassFilter_t* f);


#endif /* INCLUDE_LIB_DIGITAL_FILTERS_BYPASSFILTER_H_ */
