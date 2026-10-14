/*
 * timer_driver.c
 *
 *  Created on: 13-Oct-2026
 *      Author: Sushant Sharma
 */
/*
 * timer_driver.c
 *
 * TIM2-based millisecond timebase for STM32L4S5.
 */

#include "stm32l4s5xx.h"
#include "timer_driver.h"
#include "interrupt_driver.h"

static volatile uint32_t timer_ms = 0U;
static timer_callback_t timer_callback = 0U;

/**
 * @brief TIM2 update interrupt handler.
 */
void TIM2_IRQHandler(void)
{
    if ((TIM2->SR & (1U << 0)) != 0U)
    {
        /* Clear the update interrupt flag. */
    	TIM2->SR = ~(1U << 0);

        timer_ms++;

        /* Keep interrupt callbacks short and non-blocking. */
        if (timer_callback != 0U)
        {
            timer_callback();
        }
    }
}

/**
 * @brief Initialize TIM2.
 * @param prescaler Prescaler register value.
 * @param period Auto-reload register value.
 */
void timer_init(uint32_t prescaler, uint32_t period)
{
	if (prescaler > 65535U)
	    {
	        return;
	    }

	/* Enable the TIM2 peripheral clock. */
    RCC->APB1ENR1 |= (1U << 0);

    /* Disable timer and update interrupts during configuration. */
    TIM2->CR1 &= ~(1U << 0);
    TIM2->DIER &= ~(1U << 0);

    /* Configure timer registers. */
    TIM2->PSC = prescaler;
    TIM2->ARR = period;
    TIM2->CNT = 0U;

    timer_ms = 0U;

    /* Load the prescaler and auto-reload registers. */
    TIM2->EGR = (1U << 0);

    /* Clear pending status flags. */
    TIM2->SR = 0U;

    /* Enable update interrupt and its NVIC entry. */
    TIM2->DIER |= (1U << 0);
    interrupt_enable(TIM2_IRQn);
}

/**
 * @brief Start TIM2.
 */
void timer_start(void)
{
    TIM2->CR1 |= (1U << 0);
}

/**
 * @brief Stop TIM2.
 */
void timer_stop(void)
{
    TIM2->CR1 &= ~(1U << 0);
}

/**
 * @brief Get elapsed milliseconds.
 */
uint32_t timer_get_ms(void)
{
    return timer_ms;
}

/**
 * @brief Wait for the requested number of milliseconds.
 * @note Requires TIM2 to be running and its interrupt to be serviced.
 */
void timer_delay_ms(uint32_t ms)
{
    uint32_t start = timer_get_ms();

    while ((uint32_t)(timer_get_ms() - start) < ms)
    {
        /* Busy-wait. */
    }
}

/**
 * @brief Register the callback invoked on each update interrupt.
 */
void timer_set_callback(timer_callback_t callback)
{
    timer_callback = callback;
}
