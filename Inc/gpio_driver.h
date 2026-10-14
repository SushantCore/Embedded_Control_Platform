/*
 * gpio_driver.h
 *
 *  Created on: 12-Oct-2026
 *      Author: Sushant Sharma
 */

#ifndef GPIO_DRIVER_H_
#define GPIO_DRIVER_H_

#include <stdint.h>
#include "stm32l4s5xx.h"

/* GPIO operating modes */
typedef enum
{
    GPIO_MODE_INPUT = 0U,
    GPIO_MODE_OUTPUT = 1U,
    GPIO_MODE_ALTERNATE = 2U,
    GPIO_MODE_ANALOG = 3U
} gpio_mode_t;

/* GPIO output states */
typedef enum
{
    GPIO_LOW = 0U,
    GPIO_HIGH = 1U
} gpio_state_t;

/* GPIO driver API */
void gpio_init(GPIO_TypeDef *port, uint16_t pin, gpio_mode_t mode);
void gpio_write(GPIO_TypeDef *port, uint16_t pin, gpio_state_t state);
uint8_t gpio_read(GPIO_TypeDef *port, uint16_t pin);
void gpio_toggle(GPIO_TypeDef *port, uint16_t pin);

#endif /* GPIO_DRIVER_H_ */
