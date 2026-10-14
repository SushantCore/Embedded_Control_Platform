/*
 * error_handler.h
 *
 *  Created on: 14-Oct-2026
 *      Author: Sushant Sharma
 */

#ifndef ERROR_HANDLER_H_
#define ERROR_HANDLER_H_

#include <stdint.h>

typedef enum
{
    ERROR_NONE = 0U,
    ERROR_INVALID_ARGUMENT,
    ERROR_UART_FAILURE,
    ERROR_TIMER_FAILURE,
    ERROR_GPIO_FAILURE,
    ERROR_CONFIGURATION_FAILURE,
    ERROR_SYSTEM_FAILURE
} error_code_t;

void error_handler(
    error_code_t code,
    const char *file,
    int line
);

#define ERROR_HANDLE(code) \
    error_handler((code), __FILE__, __LINE__)

void HardFault_Handler(void);

#endif /* ERROR_HANDLER_H_ */
