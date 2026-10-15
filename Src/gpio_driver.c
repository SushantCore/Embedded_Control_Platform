/*
 * gpio_driver.c
 *
 *  Created on: 12-Oct-2026
 *      Author: Sushant Sharma
 */

#include "gpio_driver.h"

static uint8_t gpio_port_valid(GPIO_TypeDef *port)
{
    return (port == GPIOA ||
            port == GPIOB ||
            port == GPIOC ||
            port == GPIOD ||
            port == GPIOE ||
            port == GPIOF ||
            port == GPIOG ||
            port == GPIOH);
}

static void gpio_enable_clock(GPIO_TypeDef *port)
{
    if (port == GPIOA)
        RCC->AHB2ENR |= (1U << 0);
    else if (port == GPIOB)
        RCC->AHB2ENR |= (1U << 1);
    else if (port == GPIOC)
        RCC->AHB2ENR |= (1U << 2);
    else if (port == GPIOD)
        RCC->AHB2ENR |= (1U << 3);
    else if (port == GPIOE)
        RCC->AHB2ENR |= (1U << 4);
    else if (port == GPIOF)
        RCC->AHB2ENR |= (1U << 5);
    else if (port == GPIOG)
        RCC->AHB2ENR |= (1U << 6);
    else if (port == GPIOH)
        RCC->AHB2ENR |= (1U << 7);
}

void gpio_init(GPIO_TypeDef *port, uint16_t pin, gpio_mode_t mode)
{
    uint32_t shift;

    if (!gpio_port_valid(port) || pin > 15U || mode > GPIO_MODE_ANALOG)
        return;

    gpio_enable_clock(port);

    shift = (uint32_t)pin * 2U;

    port->MODER &= ~(3U << shift);

    port->MODER |= ((uint32_t)mode << shift);
}

void gpio_write(GPIO_TypeDef *port, uint16_t pin, gpio_state_t state)
{
    if (!gpio_port_valid(port) || pin > 15U || state > GPIO_HIGH)
        return;

    if (state == GPIO_HIGH)
    {
        port->BSRR = (1U << pin);
    }
    else
    {
        port->BSRR = (1U << (pin + 16U));
    }
}

uint8_t gpio_read(GPIO_TypeDef *port, uint16_t pin)
{
    if (!gpio_port_valid(port) || pin > 15U)
        return 0U;

    return (uint8_t)((port->IDR >> pin) & 1U);
}

void gpio_toggle(GPIO_TypeDef *port, uint16_t pin)
{
    if (!gpio_port_valid(port) || pin > 15U)
        return;

    port->ODR ^= (1U << pin);
}
