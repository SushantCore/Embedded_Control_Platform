/*
 * pwm_driver.h
 *
 *  Created on: 14-Oct-2026
 *      Author: Sushant Sharma
 */

#ifndef PWM_DRIVER_H_
#define PWM_DRIVER_H_

#include <stdint.h>

void pwm_init(uint32_t frequency);
void pwm_set_duty(uint8_t channel, uint8_t duty_percent);
void pwm_start(uint8_t channel);
void pwm_stop(uint8_t channel);

#endif /* PWM_DRIVER_H_ */
