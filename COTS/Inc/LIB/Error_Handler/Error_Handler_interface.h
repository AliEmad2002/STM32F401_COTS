/*
 * Error_Handler_interface.h
 *
 *  Created on: Oct 12, 2022
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_LIB_ERROR_HANDLER_INTERFACE_H_
#define INCLUDE_LIB_ERROR_HANDLER_INTERFACE_H_

typedef enum{
	ErrorHandler_ErrorSourceDriver_ADC
}ErrorHandler_ErrorSourceDriver_t;

typedef enum{
	/*
	 * 'DISCNUM' is the value that defines sub-group length of discontinuous
	 * mode on regular channels. located in ADC_CR1[13:15]
	 */
	ErrorHandler_ADC_ErrorSource_WrongDISCNUM
}ErrorHandler_ADC_ErrorSource_t;


void ErrorHandler_voidExecute(u32 errCode);


#endif /* INCLUDE_LIB_ERROR_HANDLER_INTERFACE_H_ */
