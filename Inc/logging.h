/*
 * logging.h
 *
 *  Created on: 14-Oct-2026
 *      Author: Sushant Sharma
 */

#ifndef LOGGING_H_
#define LOGGING_H_

typedef enum
{
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} log_level_t;

void logging_init(void);

void log_write(
    log_level_t level,
    const char *message,
    const char *file,
    int line
);

#define LOG_INFO(message) \
    log_write(LOG_LEVEL_INFO, message, __FILE__, __LINE__)

#define LOG_WARN(message) \
    log_write(LOG_LEVEL_WARN, message, __FILE__, __LINE__)

#define LOG_ERROR(message) \
    log_write(LOG_LEVEL_ERROR, message, __FILE__, __LINE__)

#endif /* LOGGING_H_ */
