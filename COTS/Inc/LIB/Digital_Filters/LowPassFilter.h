/*
 * LowPassFilter.h
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

#ifndef INCLUDE_LIB_DIGITAL_FILTERS_LOWPASSFILTER_H_
#define INCLUDE_LIB_DIGITAL_FILTERS_LOWPASSFILTER_H_

typedef struct{
	f32 c[2];	// constants of the filter.
	f32 xPrev;
	f32 yPrev;
	f32(*getSample)(void);
	void(*callback)(f32);
	u8 timerUnitNumber;
}LowPassFilter_t;

/*
 * Initialized with:
 * 		-	critical frequency (wc).
 * 		-	sampling frequency (Fs).
 * 		-	gain               (k).
 * 		-	getter function to get the new sample.
 * 		-	callback function to be called after each sample, with filtered value
 * 			passed to it.
 * 		-	timer unit to be used.
 */
void LowPassFilter_voidInit(
	LowPassFilter_t* f, f32 wc, f32 fs, f32 k,
	f32(*getter)(void), void(*callback)(f32),
	u8 timerUnitNumber, u8 priGroup, u8 priSub
);

/*
 * This function must be put by user in update_handler of the used timer.
 * (This is an inline function)
 */
void LowPassFilter_voidFilter(LowPassFilter_t* f);

void LowPassFilter_voidEnableFilter(LowPassFilter_t* f);

void LowPassFilter_voidDisableFilter(LowPassFilter_t* f);


#endif /* INCLUDE_LIB_DIGITAL_FILTERS_LOWPASSFILTER_H_ */
