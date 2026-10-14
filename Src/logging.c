/*
 * logging.c
 *
 *  Created on: 14-Oct-2026
 *      Author: Sushant Sharma
 */

#include <stdio.h>

#include "logging.h"
#include "uart_driver.h"
#include "timer_driver.h"

void logging_init(void)
{
    LOG_INFO("Logging initialized");
}

void log_write(
    log_level_t level,
    const char *message,
    const char *file,
    int line
)
{
    char output[160];
    const char *level_text;

    if (message == 0 || file == 0)
    {
        return;
    }

    switch (level)
    {
        case LOG_LEVEL_INFO:
            level_text = "INFO";
            break;

        case LOG_LEVEL_WARN:
            level_text = "WARN";
            break;

        case LOG_LEVEL_ERROR:
            level_text = "ERROR";
            break;

        default:
            level_text = "UNKNOWN";
            break;
    }

    snprintf(
        output,
        sizeof(output),
        "[%lu ms] [%s] %s (%s:%d)\r\n",
        (unsigned long)timer_get_ms(),
        level_text,
        message,
        file,
        line
    );

    uart_send_string(output);
}
