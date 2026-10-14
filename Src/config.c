/*
 * config.c
 *
 *  Created on: 14-Oct-2026
 *      Author: Sushant Sharma
 */

#include <string.h>

#include "config.h"

#define CONFIG_MAX_ENTRIES  10U
#define CONFIG_KEY_LENGTH   16U

typedef struct
{
    char key[CONFIG_KEY_LENGTH];
    int32_t value;
    uint8_t used;
} config_entry_t;

static config_entry_t config_table[CONFIG_MAX_ENTRIES];

void config_init(void)
{
    memset(config_table, 0, sizeof(config_table));
}

config_status_t config_set(const char *key, int32_t value)
{
    uint32_t i;
    uint32_t free_index = CONFIG_MAX_ENTRIES;

    if (key == 0 || key[0] == '\0')
    {
        return CONFIG_ERROR_INVALID_ARGUMENT;
    }

    if (strlen(key) >= CONFIG_KEY_LENGTH)
    {
        return CONFIG_ERROR_INVALID_ARGUMENT;
    }

    for (i = 0U; i < CONFIG_MAX_ENTRIES; i++)
    {
        if (config_table[i].used != 0U &&
            strcmp(config_table[i].key, key) == 0)
        {
            config_table[i].value = value;
            return CONFIG_OK;
        }

        if (config_table[i].used == 0U &&
            free_index == CONFIG_MAX_ENTRIES)
        {
            free_index = i;
        }
    }

    if (free_index == CONFIG_MAX_ENTRIES)
    {
        return CONFIG_ERROR_FULL;
    }

    strcpy(config_table[free_index].key, key);
    config_table[free_index].value = value;
    config_table[free_index].used = 1U;

    return CONFIG_OK;
}

config_status_t config_get(const char *key, int32_t *value)
{
    uint32_t i;

    if (key == 0 || value == 0 || key[0] == '\0')
    {
        return CONFIG_ERROR_INVALID_ARGUMENT;
    }

    for (i = 0U; i < CONFIG_MAX_ENTRIES; i++)
    {
        if (config_table[i].used != 0U &&
            strcmp(config_table[i].key, key) == 0)
        {
            *value = config_table[i].value;
            return CONFIG_OK;
        }
    }

    return CONFIG_ERROR_NOT_FOUND;
}
