/*
 * Error_Handler_interface.h
 *
 *  Created on: Oct 12, 2022
 *      Author: Ali Emad Ali
 */

#ifndef INCLUDE_LIB_ERROR_HANDLER_INTERFACE_H_
#define INCLUDE_LIB_ERROR_HANDLER_INTERFACE_H_

typedef enum{
	/*
	 * 'DISCNUM' is the value that defines sub-group length of discontinuous
	 * mode on regular channels. located in ADC_CR1[13:15]
	 */
	ErrorHandler_ErrorSource_ADC_WrongDISCNUM,
	ErrorHandler_ErrorSource_ADC_WrongRegSeqLen,
	ErrorHandler_ErrorSource_ADC_WrongInjSeqLen
}ErrorHandler_ErrorSource_t;


void ErrorHandler_voidExecute(ErrorHandler_ErrorSource_t errSource);


#endif /* INCLUDE_LIB_ERROR_HANDLER_INTERFACE_H_ */
