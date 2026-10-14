/*
 * timer_driver.h
 *
 *  Created on: 13-Oct-2026
 *      Author: Sushant Sharma
 */

#ifndef TIMER_DRIVER_H_
#define TIMER_DRIVER_H_

#include <stdint.h>

typedef void (*timer_callback_t)(void);

void timer_init(uint32_t prescaler, uint32_t period);
void timer_start(void);
void timer_stop(void);

uint32_t timer_get_ms(void);
void timer_delay_ms(uint32_t ms);
void timer_set_callback(timer_callback_t callback);

#endif /* TIMER_DRIVER_H_ */
