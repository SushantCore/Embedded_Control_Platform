/*
 * uart_driver.c
 *
 *  Created on: 14-Oct-2026
 *      Author: Sushant Sharma
 */

/*
 * uart_driver.c
 *
 * Bare-metal USART1 driver for STM32L4S5.
 * TX: PB6, RX: PB7, AF7.
 */

#include "stm32l4s5xx.h"
#include "uart_driver.h"
#include "interrupt_driver.h"

#define UART1_TX_PIN       6U
#define UART1_RX_PIN       7U
#define UART1_CLOCK_HZ     4000000U

static uart_rx_callback_t uart_rx_callback = 0;

/**
 * @brief Initialize USART1 for 8-N-1 communication.
 * @param baud_rate Requested baud rate.
 */
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

    /* Enable GPIOB and USART1 clocks. */
    RCC->AHB2ENR |= (1U << 1);
    RCC->APB2ENR |= (1U << 14);

    /* Select PCLK2 as USART1 clock source. */
    RCC->CCIPR &= ~(3U << 0);

    /* Configure PB6 and PB7 as alternate-function pins. */
    GPIOB->MODER &= ~((3U << (UART1_TX_PIN * 2U)) |
                      (3U << (UART1_RX_PIN * 2U)));

    GPIOB->MODER |=  ((2U << (UART1_TX_PIN * 2U)) |
                      (2U << (UART1_RX_PIN * 2U)));

    /* AF7 selects USART1 on these pins. */
    GPIOB->AFR[0] &= ~((0xFU << (UART1_TX_PIN * 4U)) |
                       (0xFU << (UART1_RX_PIN * 4U)));

    GPIOB->AFR[0] |=  ((7U << (UART1_TX_PIN * 4U)) |
                       (7U << (UART1_RX_PIN * 4U)));

    /* Configure push-pull outputs and disable internal pulls. */
    GPIOB->OTYPER &= ~((1U << UART1_TX_PIN) |
                       (1U << UART1_RX_PIN));

    GPIOB->PUPDR &= ~((3U << (UART1_TX_PIN * 2U)) |
                      (3U << (UART1_RX_PIN * 2U)));

    /* Disable USART1 and its interrupts during configuration. */
    USART1->CR1 = 0U;
    USART1->CR2 = 0U;
    USART1->CR3 = 0U;

    /* Configure baud rate and clear pending status flags. */
    USART1->BRR = baud_divisor;
    USART1->ICR = 0xFFFFFFFFU;

    /* Enable transmitter and receiver. */
    USART1->CR1 = (1U << 3) | (1U << 2);

    /* Enable USART1. */
    USART1->CR1 |= (1U << 0);
}

/**
 * @brief Transmit one byte using polling.
 */
void uart_send_byte(uint8_t data)
{
    while ((USART1->ISR & (1U << 7)) == 0U)
    {
        /* Wait for the transmit data register to become empty. */
    }

    USART1->TDR = data;
}

/**
 * @brief Transmit a null-terminated string.
 */
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

/**
 * @brief Receive one byte using polling.
 */
uint8_t uart_receive_byte(void)
{
    while ((USART1->ISR & (1U << 5)) == 0U)
    {
        /* Wait for received data. */
    }

    return (uint8_t)USART1->RDR;
}

/**
 * @brief Enable interrupt-driven reception.
 */
void uart_enable_rx_interrupt(uart_rx_callback_t callback)
{
    uart_rx_callback = callback;

    /* Clear pending receive data and error flags. */
    USART1->ICR = 0xFFFFFFFFU;

    /* Enable receive-data interrupt. */
    USART1->CR1 |= (1U << 5);

    interrupt_enable(USART1_IRQn);
}

/**
 * @brief USART1 interrupt handler.
 */
void USART1_IRQHandler(void)
{
    uint32_t status = USART1->ISR;

    /* Handle received data. */
    if ((status & (1U << 5)) != 0U)
    {
        uint8_t data = (uint8_t)USART1->RDR;

        if (uart_rx_callback != 0)
        {
            uart_rx_callback(data);
        }
    }

    /* Clear common USART error flags when present. */
    if ((status & ((1U << 3) | (1U << 2) | (1U << 1))) != 0U)
    {
        USART1->ICR = (1U << 3) | (1U << 2) | (1U << 1);
    }
}
