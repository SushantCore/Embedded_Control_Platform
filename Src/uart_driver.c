/*
 * uart_driver.c
 *
 *  Created on: 14-Oct-2026
 *      Author: Sushant Sharma
 */

#include "stm32l4s5xx.h"
#include "uart_driver.h"
#include "interrupt_driver.h"

#define UART1_TX_PIN       6U
#define UART1_RX_PIN       7U
#define UART1_CLOCK_HZ     4000000U

static uart_rx_callback_t uart_rx_callback = 0;

void uart_init(uint32_t baud_rate)
{
    uint32_t baud_divisor;

    if (baud_rate == 0U)
    {
        return;
    }

    baud_divisor = (UART1_CLOCK_HZ + (baud_rate / 2U)) / baud_rate;

    if (baud_divisor == 0U || baud_divisor > 0xFFFFU)
    {
        return;
    }

    RCC->AHB2ENR |= (1U << 1);
    RCC->APB2ENR |= (1U << 14);

    RCC->CCIPR &= ~(3U << 0);

    GPIOB->MODER &= ~((3U << (UART1_TX_PIN * 2U)) |
                      (3U << (UART1_RX_PIN * 2U)));

    GPIOB->MODER |=  ((2U << (UART1_TX_PIN * 2U)) |
                      (2U << (UART1_RX_PIN * 2U)));

    GPIOB->AFR[0] &= ~((0xFU << (UART1_TX_PIN * 4U)) |
                       (0xFU << (UART1_RX_PIN * 4U)));

    GPIOB->AFR[0] |=  ((7U << (UART1_TX_PIN * 4U)) |
                       (7U << (UART1_RX_PIN * 4U)));

    GPIOB->OTYPER &= ~((1U << UART1_TX_PIN) |
                       (1U << UART1_RX_PIN));

    GPIOB->PUPDR &= ~((3U << (UART1_TX_PIN * 2U)) |
                      (3U << (UART1_RX_PIN * 2U)));

    USART1->CR1 = 0U;
    USART1->CR2 = 0U;
    USART1->CR3 = 0U;

    USART1->BRR = baud_divisor;
    USART1->ICR = 0xFFFFFFFFU;

    USART1->CR1 = (1U << 3) | (1U << 2);

    USART1->CR1 |= (1U << 0);
}

void uart_send_byte(uint8_t data)
{
    while ((USART1->ISR & (1U << 7)) == 0U)
    {
    }

    USART1->TDR = data;
}

void uart_send_string(const char *str)
{
    if (str == 0)
    {
        return;
    }

    while (*str != '\0')
    {
        uart_send_byte((uint8_t)*str);
        str++;
    }
}

uint8_t uart_receive_byte(void)
{
    while ((USART1->ISR & (1U << 5)) == 0U)
    {
    }

    return (uint8_t)USART1->RDR;
}

void uart_enable_rx_interrupt(uart_rx_callback_t callback)
{
    uart_rx_callback = callback;

    USART1->ICR = 0xFFFFFFFFU;

    USART1->CR1 |= (1U << 5);

    interrupt_enable(USART1_IRQn);
}

void USART1_IRQHandler(void)
{
    uint32_t status = USART1->ISR;

    if ((status & (1U << 5)) != 0U)
    {
        uint8_t data = (uint8_t)USART1->RDR;

        if (uart_rx_callback != 0)
        {
            uart_rx_callback(data);
        }
    }

    if ((status & ((1U << 3) | (1U << 2) | (1U << 1))) != 0U)
    {
        USART1->ICR = (1U << 3) | (1U << 2) | (1U << 1);
    }
}
