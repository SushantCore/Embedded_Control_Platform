/*
 * pwm_driver.c
 *
 *  Created on: 14-Oct-2026
 *      Author: Sushant Sharma
 */

#include "stm32l4s5xx.h"
#include "pwm_driver.h"

#define PWM_TIMER_CLOCK_HZ  4000000U
#define PWM_LED_PIN         14U
#define PWM_TIMER_MAX_ARR   65535U

static uint32_t pwm_period = 0U;
static uint8_t pwm_initialized = 0U;

void pwm_init(uint32_t frequency)
{
    uint32_t prescaler;
    uint32_t period;

    if (frequency == 0U || frequency > PWM_TIMER_CLOCK_HZ)
    {
        return;
    }

    prescaler = 0U;

    period = PWM_TIMER_CLOCK_HZ / frequency;

    if (period == 0U)
    {
        return;
    }

    if (period > (PWM_TIMER_MAX_ARR + 1U))
    {
        prescaler = (period + PWM_TIMER_MAX_ARR) /
                    (PWM_TIMER_MAX_ARR + 1U);

        period = PWM_TIMER_CLOCK_HZ /
                 ((prescaler + 1U) * frequency);
    }

    if (period == 0U || period > (PWM_TIMER_MAX_ARR + 1U))
    {
        return;
    }

    prescaler = (PWM_TIMER_CLOCK_HZ /
                 (frequency * period)) - 1U;

    if (prescaler > PWM_TIMER_MAX_ARR)
    {
        return;
    }

    period -= 1U;
    RCC->AHB2ENR |= (1U << 1);
    RCC->APB2ENR |= (1U << 16);

    GPIOB->MODER &= ~(3U << (PWM_LED_PIN * 2U));
    GPIOB->MODER |=  (2U << (PWM_LED_PIN * 2U));

    GPIOB->AFR[1] &= ~(0xFU << ((PWM_LED_PIN - 8U) * 4U));
    GPIOB->AFR[1] |=  (14U << ((PWM_LED_PIN - 8U) * 4U));

    GPIOB->OTYPER &= ~(1U << PWM_LED_PIN);
    GPIOB->PUPDR &= ~(3U << (PWM_LED_PIN * 2U));

    TIM15->CR1 &= ~(1U << 0);
    TIM15->CCER &= ~(1U << 0);

    TIM15->PSC = prescaler;
    TIM15->ARR = period;
    TIM15->CNT = 0U;

    TIM15->CCMR1 &= ~((7U << 4) | (1U << 3));
    TIM15->CCMR1 |=  ((6U << 4) | (1U << 3));

    TIM15->CCR1 = 0U;

    TIM15->CR1 |= (1U << 7);
    TIM15->EGR = (1U << 0);
    TIM15->SR = 0U;

    TIM15->BDTR |= (1U << 15);
    TIM15->CCER |= (1U << 0);

    pwm_period = period;
    pwm_initialized = 1U;
}

void pwm_set_duty(uint8_t channel, uint8_t duty_percent)
{
    uint32_t compare;

    if (pwm_initialized == 0U ||
        channel != 1U ||
        duty_percent > 100U)
    {
        return;
    }

    if (duty_percent == 100U)
    {
        if (pwm_period == PWM_TIMER_MAX_ARR)
        {
            TIM15->CCER &= ~(1U << 0);
            GPIOB->BSRR = (1U << PWM_LED_PIN);
            return;
        }

        compare = pwm_period + 1U;
    }
    else
    {
        compare = ((pwm_period + 1U) * duty_percent) / 100U;
    }

    TIM15->CCER |= (1U << 0);
    TIM15->CCR1 = compare;
}

void pwm_start(uint8_t channel)
{
    if (pwm_initialized != 0U && channel == 1U)
    {
        TIM15->CR1 |= (1U << 0);
    }
}

void pwm_stop(uint8_t channel)
{
    if (channel == 1U)
    {
        TIM15->CR1 &= ~(1U << 0);
    }
}
