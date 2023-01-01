/*
 * ADC_config.h
 *
 *  Created on: Jan 1, 2023
 *      Author: Ali Emad Ali
 *
 * Notes:
 * - All parameters defined in this file are unit specified by the
 * very last word in them. i.e.: unit of the parameter named "ADC_V_25_VOLTS" is
 * [Volts].
 *
 * All parameters are expected to be of type "f32". Don't worry, driver does not
 * extensively use floating-point operations, these values are for compiler to
 * evaluate some integer values - unless user uses floating-return functions -.
 */

#ifndef INCLUDE_MCAL_ADC_ADC_CONFIG_H_
#define INCLUDE_MCAL_ADC_ADC_CONFIG_H_

/*
 * ADC Vref.
 * expected type is "f32"
 */
# define ADC_V_REF_VOLTS		((f32)(3.3f))

/*
 * value that ADC reads from internal temperature sensor when real temperature
 * is 25 degrees celsius.
 * Could be obtained by experiment or from "Temperature sensor characteristics"
 * section in datasheet: "DS5319 Rev 18", P78/116.
 */
#define ADC_V_25_VOLTS	((f32)(1.43f))

/*
 * Average Slope for curve between Temperature vs. VSENSE.
 * (in volts)
 */
#define ADC_AVG_SLOPE_VOLTS	((f32)(0.0043f))


#endif /* INCLUDE_MCAL_ADC_ADC_CONFIG_H_ */
