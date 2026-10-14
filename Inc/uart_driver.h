/*
 * uart_driver.h
 *
 *  Created on: 14-Oct-2026
 *      Author: Sushant Sharma
 */

#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

#include <stdint.h>

typedef void (*uart_rx_callback_t)(uint8_t data);

void uart_init(uint32_t baud_rate);

void uart_send_byte(uint8_t data);
void uart_send_string(const char *str);

uint8_t uart_receive_byte(void);

void uart_enable_rx_interrupt(uart_rx_callback_t callback);

#endif /* UART_DRIVER_H_ */
