/*
 * interrupt_driver.h
 *
 *  Created on: 14-Oct-2026
 *      Author: Sushant Sharma
 */

#ifndef INTERRUPT_DRIVER_H_
#define INTERRUPT_DRIVER_H_

#include <stdint.h>
#include "stm32l4s5xx.h"

void interrupt_enable(IRQn_Type irq);
void interrupt_disable(IRQn_Type irq);

#endif /* INTERRUPT_DRIVER_H_ */
