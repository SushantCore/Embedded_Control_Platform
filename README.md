# Embedded Control Platform

A bare-metal firmware platform for the **B-L4S5I-IOT01A development board**, built around the **STM32L4S5VITx microcontroller**.

This project demonstrates a layered approach to embedded C: an application uses reusable drivers and middleware, while the drivers control microcontroller peripherals through registers.

> **Build status:** The source has built successfully in STM32CubeIDE. Complete hardware validation should be performed on the target board before describing every feature as physically tested.

## Architecture

![Embedded Control Platform software architecture](images/architecture.svg)

**How to read the diagram:** The application calls middleware APIs for logging, configuration, and error reporting. It also calls driver APIs directly for GPIO, UART, timer, and PWM operations. Drivers access peripheral registers. CMSIS device headers provide register structures and interrupt declarations; they are not a separate runtime hardware layer.

## Features

- **GPIO:** Configure pins and read, write, or toggle their logic state.
- **UART command interface:** Receive commands and send responses over USART1.
- **Timer:** Use TIM2 interrupts to maintain a millisecond counter.
- **PWM:** Use TIM15 Channel 1 to generate a PWM signal for LED fading.
- **SOS demonstration:** Generate a timed SOS LED pattern without blocking the main loop.
- **Logging:** Print timestamped `INFO`, `WARN`, and `ERROR` messages over UART.
- **Configuration:** Store simple key/value settings in RAM.
- **Error handling:** Report readable error messages with source-file and line information; provide a HardFault handler for debugger inspection.

## Hardware and software

| Item | Details |
|---|---|
| Development board | B-L4S5I-IOT01A |
| Microcontroller | STM32L4S5VITx |
| CPU core | Arm Cortex-M4 |
| IDE | STM32CubeIDE |
| Language | Embedded C |
| Peripheral access | Direct register access using CMSIS device definitions |
| UART format | 115200 baud, 8 data bits, no parity, 1 stop bit (8-N-1) |
| Timer | TIM2 millisecond timebase |
| PWM | TIM15 Channel 1; output configured on PB14 in the current source |

**Pin and clock note:** The current source configures USART1 on PB6/PB7 and TIM15_CH1 on PB14. Confirm these alternate-function mappings and the actual clock tree against the board documentation and MCU reference manual before hardware testing. UART and PWM clock constants assume a 4 MHz peripheral/timer clock.

## UART commands

Send a command followed by Enter in a serial terminal.

| Command | Action |
|---|---|
| `HELP` | Print the command list |
| `LED ON` | Turn the configured LED output on |
| `LED OFF` | Turn the configured LED output off |
| `LED TOGGLE` | Toggle the LED output |
| `FADE` | Start the PWM fading demonstration |
| `SOS` | Play the SOS LED pattern |
| `UPTIME` | Print elapsed time in seconds |

The command parser uses interrupt-driven UART reception. GPIO LED control and PWM use PB14 in the current implementation, so switching between them reconfigures the pin.

## Project structure

```text
Embedded_Control_Platform/
├── Inc/
│   ├── app.h
│   ├── config.h
│   ├── error_handler.h
│   ├── gpio_driver.h
│   ├── interrupt_driver.h
│   ├── logging.h
│   ├── pwm_driver.h
│   ├── timer_driver.h
│   └── uart_driver.h
├── Src/
│   ├── app.c
│   ├── config.c
│   ├── error_handler.c
│   ├── gpio_driver.c
│   ├── interrupt_driver.c
│   ├── logging.c
│   ├── main.c
│   ├── pwm_driver.c
│   ├── timer_driver.c
│   ├── uart_driver.c
│   ├── system_stm32l4xx.c
│   ├── syscalls.c
│   └── sysmem.c
├── Startup/
│   └── startup assembly file
├── STM32L4S5VITX_FLASH.ld
├── STM32L4S5VITX_RAM.ld
├── .project
├── .cproject
├── .gitignore
└── README.md
```

STM32CubeIDE may add generated support files. Keep files required to build and debug the project. Avoid committing temporary build output unless you have a specific reason to distribute it.

## Software layers

### 1. Application — `app.c` / `app.h`

Processes user commands and coordinates features without putting all peripheral-register operations in the application code.

### 2. Middleware

- **Logging (`logging.c`):** Formats messages with a timestamp, severity, and source location.
- **Error handling (`error_handler.c`):** Converts error codes into readable messages and reports them through logging. The HardFault handler stops execution for debugger inspection.
- **Configuration (`config.c`):** Provides key/value storage in RAM. Values are lost when the board resets.

### 3. Drivers

- **GPIO:** Pin mode and digital input/output operations.
- **UART:** USART1 initialization, polling transmit/receive, and interrupt-driven receive callbacks.
- **Timer:** TIM2-based millisecond counter, delay helper, and optional callback.
- **PWM:** TIM15 Channel 1 output and duty-cycle control.
- **Interrupt:** NVIC enable/disable wrappers.

### 4. Microcontroller hardware and CMSIS definitions

The STM32L4S5VITx peripheral hardware performs the GPIO, serial, timer, PWM, and interrupt functions. CMSIS device headers define the register structures, addresses, and interrupt names used by the drivers.

## Build in STM32CubeIDE

1. Install **STM32CubeIDE**.
2. Import or open the existing `Embedded_Control_Platform` project folder.
3. Confirm the target device is **STM32L4S5VITx**.
4. Select **Project → Build Project**.
5. Check the Console for build errors. A successful build should finish with zero errors.

The project has built successfully in the current development environment. Hardware-dependent behaviour should be checked on the physical board.

## Hardware test checklist

When the board is available:

1. Connect the board over USB and identify the correct serial/virtual COM port.
2. Open a serial terminal at **115200 baud, 8 data bits, no parity, 1 stop bit (115200 8-N-1)**.
3. Reset the board and confirm the startup banner appears.
4. Send `HELP` and confirm the command list appears.
5. Test `LED ON`, `LED OFF`, and `LED TOGGLE`.
6. Test `UPTIME` and confirm the reported time increases.
7. Test `FADE` and observe the PWM output/LED.
8. Test `SOS` and confirm the timed pattern completes.
9. Confirm timestamped log messages appear and normal commands continue working after the demonstrations.
10. Record any pin, clock, or terminal configuration changes discovered during testing.

## Design notes and limitations

- This is a **bare-metal** project: the application loop and interrupt handlers run without an RTOS.
- The timer delay helper is a busy-wait and should not be used for long waits in latency-sensitive code.
- The timer callback runs inside the TIM2 interrupt handler; callbacks must be short and non-blocking.
- UART transmission and logging use polling/blocking operations. Do not call them from an interrupt handler.
- Configuration values are stored in RAM only and reset to defaults on restart.
- `gpio_init(..., GPIO_MODE_ALTERNATE)` selects alternate-function mode; the required alternate-function number must be configured separately by the peripheral-specific driver.
- UART and PWM clock constants assume a 4 MHz peripheral clock. If the clock tree changes, update the constants and verify baud rate and PWM frequency.
- Physical pin behaviour depends on the board schematic, alternate-function mapping, and circuitry connected to the pin.

## Author

**Sushant Sharma**  
Embedded firmware portfolio project

---

This project is intended for learning and portfolio demonstration. Verify board-specific pin mappings, clock configuration, and peripheral behaviour on the target hardware before using it in a real product.
