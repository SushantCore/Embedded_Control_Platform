/*
 * config.h
 *
 *  Created on: 14-Oct-2026
 *      Author: Sushant Sharma
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdint.h>

typedef enum
{
    CONFIG_OK = 0,
    CONFIG_ERROR_INVALID_ARGUMENT,
    CONFIG_ERROR_NOT_FOUND,
    CONFIG_ERROR_FULL
} config_status_t;

void config_init(void);

config_status_t config_set(const char *key, int32_t value);

config_status_t config_get(const char *key, int32_t *value);

#endif /* CONFIG_H_ */
