/*
 * interrupt_driver.c
 *
 *  Created on: 14-Oct-2026
 *      Author: Sushant Sharma
 */

#include "interrupt_driver.h"

void interrupt_enable(IRQn_Type irq)
{
    if (irq < 0)
    {
        return;
    }

    NVIC_EnableIRQ(irq);
}

void interrupt_disable(IRQn_Type irq)
{
    if (irq < 0)
    {
        return;
    }

    NVIC_DisableIRQ(irq);
}
