# Embedded Control Platform

A bare-metal firmware project for the **B-L4S5I-IOT01A** board, built around the **STM32L4S5VITx** microcontroller.

## Architecture

flowchart TD
    A["Application<br/>Commands and LED demos"]
    B["Middleware<br/>Logging, errors and configuration"]
    C["Drivers<br/>GPIO, UART, timer, PWM and interrupts"]
    D["Hardware<br/>STM32L4S5VITx"]

    A --> B
    B --> C
    C --> D
    A -. "Direct peripheral access through driver APIs" .-> C

**In simple terms:** The application decides what the project should do. Middleware provides shared services such as logging and error reporting. Drivers control the microcontroller's peripherals. The hardware is where those operations happen.

## Features

- GPIO input/output control
- UART serial commands
- Millisecond timekeeping using TIM2
- PWM output using TIM15 Channel 1
- LED fading and SOS demonstrations
- Timestamped logging over UART
- Error reporting with source location
- Simple key/value configuration stored in RAM

## Hardware and tools

| Item | Details |
|---|---|
| Board | B-L4S5I-IOT01A |
| Microcontroller | STM32L4S5VITx |
| IDE | STM32CubeIDE |
| Language | Embedded C |
| Serial settings | 115200 baud, 8 data bits, no parity, 1 stop bit |

The current source configures USART1 on PB6/PB7 and TIM15 Channel 1 on PB14. Verify the pin mappings and clock settings against the board documentation when testing on hardware.

## Commands

Enter a command in the serial terminal, then press Enter.

| Command | Function |
|---|---|
| `HELP` | Show available commands |
| `LED ON` | Turn the LED output on |
| `LED OFF` | Turn the LED output off |
| `LED TOGGLE` | Toggle the LED output |
| `FADE` | Start the PWM fading demo |
| `SOS` | Play the SOS LED pattern |
| `UPTIME` | Show elapsed time in seconds |

## Project folders

```text
Embedded_Control_Platform/
├── Inc/       # Header files
├── Src/       # C source files
├── Startup/   # MCU startup code
├── *.ld       # Linker scripts
└── README.md
```

## Build

1. Open or import the project in STM32CubeIDE.
2. Confirm the selected MCU is `STM32L4S5VITx`.
3. Select **Project → Build Project**.
4. Check the build console for errors.

The project has built successfully in the development environment. Verify peripheral operation on the physical board before treating hardware behaviour as validated.

## Hardware test checklist

1. Connect the board to the computer.
2. Open a serial terminal at **115200 baud, 8 data bits, no parity, 1 stop bit**.
3. Reset the board and check for the startup message.
4. Test `HELP`, `LED ON`, `LED OFF`, `LED TOGGLE`, `UPTIME`, `FADE`, and `SOS`.
5. Check that log messages appear in the terminal.

## Notes

- This is a bare-metal project; it does not use an RTOS.
- Configuration values are stored in RAM and are lost after reset.
- Timer callbacks should be short and non-blocking.
- Logging and UART transmission are blocking operations; do not call them from interrupt handlers.

## Author

**Sushant Sharma**
