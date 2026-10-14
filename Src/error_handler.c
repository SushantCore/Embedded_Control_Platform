/*
 * error_handler.c
 *
 *  Created on: 14-Oct-2026
 *      Author: Sushant Sharma
 */

#include <stdio.h>
#include "stm32l4s5xx.h"
#include "error_handler.h"
#include "logging.h"

void error_handler(
    error_code_t code,
    const char *file,
    int line
)
{
    char message[100];

    switch (code)
    {
        case ERROR_INVALID_ARGUMENT:
            snprintf(message, sizeof(message),
                     "Invalid argument");
            break;

        case ERROR_UART_FAILURE:
            snprintf(message, sizeof(message),
                     "UART failure");
            break;

        case ERROR_TIMER_FAILURE:
            snprintf(message, sizeof(message),
                     "Timer failure");
            break;

        case ERROR_GPIO_FAILURE:
            snprintf(message, sizeof(message),
                     "GPIO failure");
            break;

        case ERROR_CONFIGURATION_FAILURE:
            snprintf(message, sizeof(message),
                     "Configuration failure");
            break;

        case ERROR_SYSTEM_FAILURE:
            snprintf(message, sizeof(message),
                     "System failure");
            break;

        case ERROR_NONE:
        default:
            snprintf(message, sizeof(message),
                     "Unknown or unspecified error");
            break;
    }

    log_write(LOG_LEVEL_ERROR, message, file, line);
}

void HardFault_Handler(void)
{
    __disable_irq();

    while (1)
    {
        /* Halt for debugger inspection. */
    }
}
