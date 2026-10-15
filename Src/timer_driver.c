/*
 * timer_driver.c
 *
 *  Created on: 13-Oct-2026
 *      Author: Sushant Sharma
 */

#include "stm32l4s5xx.h"
#include "timer_driver.h"
#include "interrupt_driver.h"

static volatile uint32_t timer_ms = 0U;
static timer_callback_t timer_callback = 0U;

void TIM2_IRQHandler(void)
{
    if ((TIM2->SR & (1U << 0)) != 0U)
    {
    	TIM2->SR = ~(1U << 0);

        timer_ms++;

        if (timer_callback != 0U)
        {
            timer_callback();
        }
    }
}

void timer_init(uint32_t prescaler, uint32_t period)
{
	if (prescaler > 65535U)
	    {
	        return;
	    }

    RCC->APB1ENR1 |= (1U << 0);

    TIM2->CR1 &= ~(1U << 0);
    TIM2->DIER &= ~(1U << 0);

    TIM2->PSC = prescaler;
    TIM2->ARR = period;
    TIM2->CNT = 0U;

    timer_ms = 0U;

    TIM2->EGR = (1U << 0);

    TIM2->SR = 0U;

    TIM2->DIER |= (1U << 0);
    interrupt_enable(TIM2_IRQn);
}

void timer_start(void)
{
    TIM2->CR1 |= (1U << 0);
}

void timer_stop(void)
{
    TIM2->CR1 &= ~(1U << 0);
}

uint32_t timer_get_ms(void)
{
    return timer_ms;
}

void timer_delay_ms(uint32_t ms)
{
    uint32_t start = timer_get_ms();

    while ((uint32_t)(timer_get_ms() - start) < ms)
    {
    }
}

void timer_set_callback(timer_callback_t callback)
{
    timer_callback = callback;
}
