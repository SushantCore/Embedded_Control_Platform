/*
 * app.c
 *
 *  Created on: 14-Oct-2026
 *      Author: Sushant Sharma
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "app.h"
#include "gpio_driver.h"
#include "timer_driver.h"
#include "uart_driver.h"
#include "pwm_driver.h"
#include "logging.h"
#include "error_handler.h"
#include "config.h"

#define LED_PORT GPIOB
#define LED_PIN  14U

#define COMMAND_BUFFER_SIZE 32U

static char command_buffer[COMMAND_BUFFER_SIZE];
static volatile uint32_t command_index = 0U;
static volatile uint8_t command_ready = 0U;
static volatile uint8_t discard_command = 0U;

static uint8_t pwm_mode = 0U;
static uint32_t last_fade_update = 0U;
static int16_t fade_duty = 0;
static int8_t fade_direction = 1;

#define SOS_UNIT_MS 200U

typedef struct
{
    uint8_t led_on;
    uint16_t duration_ms;
} sos_step_t;

static const sos_step_t sos_pattern[] =
{
    {1U, 200U}, {0U, 200U},
    {1U, 200U}, {0U, 200U},
    {1U, 200U}, {0U, 600U},

    {1U, 600U}, {0U, 200U},
    {1U, 600U}, {0U, 200U},
    {1U, 600U}, {0U, 600U},

    {1U, 200U}, {0U, 200U},
    {1U, 200U}, {0U, 200U},
    {1U, 200U}, {0U, 1400U}
};

#define SOS_STEP_COUNT \
    (sizeof(sos_pattern) / sizeof(sos_pattern[0]))

static uint8_t sos_active = 0U;
static uint32_t sos_step_start = 0U;
static uint32_t sos_step_index = 0U;

static void uart_rx_handler(uint8_t data)
{
    if (command_ready != 0U)
    {
        return;
    }

    if (data == '\r' || data == '\n')
    {
        if (discard_command != 0U)
        {
            discard_command = 0U;
            command_index = 0U;
            return;
        }

        if (command_index > 0U)
        {
            command_buffer[command_index] = '\0';
            command_ready = 1U;
            command_index = 0U;
        }

        return;
    }

    if (discard_command != 0U)
    {
        return;
    }

    if (command_index < (COMMAND_BUFFER_SIZE - 1U))
    {
        command_buffer[command_index++] = (char)data;
    }
    else
    {
        command_index = 0U;
        discard_command = 1U;
    }
}

static void update_led_mode(int32_t mode)
{
    if (config_set("led_mode", mode) != CONFIG_OK)
    {
        ERROR_HANDLE(ERROR_CONFIGURATION_FAILURE);
    }
}

static void start_sos(void)
{
    pwm_stop(1U);
    pwm_mode = 0U;

    gpio_init(LED_PORT, LED_PIN, GPIO_MODE_OUTPUT);

    sos_step_index = 0U;
    sos_step_start = timer_get_ms();
    sos_active = 1U;

    gpio_write(
        LED_PORT,
        LED_PIN,
        sos_pattern[0].led_on ? GPIO_HIGH : GPIO_LOW
    );

    LOG_INFO("SOS demonstration started");
}

static void update_sos(void)
{
    uint32_t now;

    if (sos_active == 0U)
    {
        return;
    }

    now = timer_get_ms();

    if ((uint32_t)(now - sos_step_start) <
        sos_pattern[sos_step_index].duration_ms)
    {
        return;
    }

    sos_step_index++;

    if (sos_step_index >= SOS_STEP_COUNT)
    {
        sos_active = 0U;
        gpio_write(LED_PORT, LED_PIN, GPIO_LOW);
        LOG_INFO("SOS demonstration completed");
        return;
    }

    sos_step_start = now;

    gpio_write(
        LED_PORT,
        LED_PIN,
        sos_pattern[sos_step_index].led_on
            ? GPIO_HIGH
            : GPIO_LOW
    );
}

static void process_command(const char *command)
{
    if (strcmp(command, "LED ON") == 0)
    {
    	sos_active = 0U;

        pwm_stop(1U);
        pwm_mode = 0U;

        gpio_init(LED_PORT, LED_PIN, GPIO_MODE_OUTPUT);
        gpio_write(LED_PORT, LED_PIN, GPIO_HIGH);

        update_led_mode(0);
        LOG_INFO("LED switched on");
    }
    else if (strcmp(command, "LED OFF") == 0)
    {
    	sos_active = 0U;

        pwm_stop(1U);
        pwm_mode = 0U;

        gpio_init(LED_PORT, LED_PIN, GPIO_MODE_OUTPUT);
        gpio_write(LED_PORT, LED_PIN, GPIO_LOW);

        update_led_mode(0);
        LOG_INFO("LED switched off");
    }
    else if (strcmp(command, "LED TOGGLE") == 0)
    {
    	sos_active = 0U;

        pwm_stop(1U);
        pwm_mode = 0U;

        gpio_init(LED_PORT, LED_PIN, GPIO_MODE_OUTPUT);
        gpio_toggle(LED_PORT, LED_PIN);

        update_led_mode(0);
        LOG_INFO("LED toggled");
    }
    else if (strcmp(command, "FADE") == 0)
    {
        /*
         * Reconfigure PB14 for TIM15 PWM.
         * This restores alternate-function mode after GPIO commands.
         */
    	sos_active = 0U;

        pwm_init(1000U);
        pwm_set_duty(1U, 0U);
        pwm_start(1U);
        last_fade_update = timer_get_ms();
        fade_duty = 0;
        fade_direction = 1;
        pwm_mode = 1U;
        update_led_mode(1);

        LOG_INFO("PWM fading enabled");
    }
    else if (strcmp(command, "SOS") == 0)
    {
        start_sos();
    }
    else if (strcmp(command, "UPTIME") == 0)
    {
        char output[48];

        snprintf(output, sizeof(output),
                 "Uptime: %lu seconds\r\n",
                 (unsigned long)(timer_get_ms() / 1000U));

        uart_send_string(output);
    }
    else if (strcmp(command, "HELP") == 0)
    {
        uart_send_string(
            "Commands:\r\n"
            "LED ON\r\n"
            "LED OFF\r\n"
            "LED TOGGLE\r\n"
            "FADE\r\n"
        	"SOS\r\n"
            "UPTIME\r\n"
            "HELP\r\n");
    }
    else
    {
        LOG_WARN("Unknown command");
        uart_send_string("Unknown command. Type HELP.\r\n");
    }
}
void app_init(void)
{
	gpio_init(LED_PORT, LED_PIN, GPIO_MODE_OUTPUT);
	gpio_write(LED_PORT, LED_PIN, GPIO_LOW);

	uart_init(115200U);

	config_init();

	if (config_set("led_mode", 0) != CONFIG_OK)
	{
	    ERROR_HANDLE(ERROR_CONFIGURATION_FAILURE);
	}

    pwm_init(1000U);
    pwm_set_duty(1U, 0U);

    logging_init();

    uart_send_string("\r\nEmbedded Control Platform\r\n");
    uart_send_string("Type HELP for commands.\r\n");

    uart_enable_rx_interrupt(uart_rx_handler);
}

void app_run(void)
{
    char local_command[COMMAND_BUFFER_SIZE];

    if (command_ready == 0U)
    {
        update_sos();

        if ((sos_active == 0U) && (pwm_mode != 0U))
        {
            uint32_t now = timer_get_ms();

            if ((uint32_t)(now - last_fade_update) >= 10U)
            {
                last_fade_update = now;

                fade_duty += fade_direction * 2;

                if (fade_duty >= 100)
                {
                    fade_duty = 100;
                    fade_direction = -1;
                }
                else if (fade_duty <= 0)
                {
                    fade_duty = 0;
                    fade_direction = 1;
                }

                pwm_set_duty(1U, (uint8_t)fade_duty);
            }
        }

        return;
    }

    NVIC_DisableIRQ(USART1_IRQn);

    if (command_ready != 0U)
    {
        memcpy(local_command, command_buffer, sizeof(local_command));
        command_ready = 0U;
    }
    else
    {
        NVIC_EnableIRQ(USART1_IRQn);
        return;
    }

    NVIC_EnableIRQ(USART1_IRQn);

    process_command(local_command);
}
