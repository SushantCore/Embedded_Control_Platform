# Embedded Control Platform

A bare-metal firmware project for the **B-L4S5I-IOT01A Discovery kit**,
built around the **STM32L4S5VITx microcontroller**.

The project demonstrates how to build a small, layered embedded system
in C without relying on HAL peripheral-control functions. It uses STM32
CMSIS device definitions to access peripheral registers directly, with
separate drivers, shared services, and an application layer.

## What the project does

The firmware accepts commands over a serial terminal connected to the
board. You can control an LED, run a brightness-fading effect, play an
SOS Morse-code pattern, and check system uptime.

  -----------------------------------------------------------------------
  Command                             What it does
  ----------------------------------- -----------------------------------
  `HELP`                              Lists the available commands.

  `LED ON`                            Turns the controlled LED on.

  `LED OFF`                           Turns the controlled LED off.

  `LED TOGGLE`                        Changes the LED to the opposite
                                      state.

  `FADE`                              Starts a repeating LED brightness
                                      fade using PWM.

  `SOS`                               Flashes the SOS Morse-code pattern
                                      without blocking the main
                                      application loop.

  `UPTIME`                            Prints the elapsed time in seconds.
  -----------------------------------------------------------------------

Enter a command in the serial terminal and press **Enter**. Commands are
handled through UART receive interrupts, while the application loop
performs the requested action.

## Main features

-   **Register-based GPIO driver** --- configures pins and supports
    input, output, alternate-function, and analog modes, plus read,
    write, and toggle operations.
-   **UART communication** --- sends and receives text over USART1, with
    polling support and interrupt-driven command reception.
-   **Timer service** --- uses TIM2 to maintain a millisecond timebase,
    provide delays, and support callbacks.
-   **PWM control** --- uses TIM15 Channel 1 to control LED brightness.
-   **Interrupt driver** --- provides small functions to enable and
    disable NVIC interrupts.
-   **Logging** --- sends timestamped `INFO`, `WARN`, and `ERROR`
    messages through UART.
-   **Error handling** --- defines error codes and reports the source
    file and line where an error is raised; includes a HardFault handler
    for debugger inspection.
-   **Configuration storage** --- stores simple key-value settings in
    RAM.
-   **Layered design** --- separates application behaviour, shared
    services, peripheral drivers, and hardware register access.

## Architecture

The project is split into four layers. Each layer has a clear
responsibility, making the code easier to understand, test, and extend.

``` mermaid
flowchart TB
    APP["Application layer<br/>app.c / app.h<br/>Commands, LED control, fade, SOS, uptime"]
    MW["Middleware layer<br/>logging.c · error_handler.c · config.c"]
    DRV["Driver layer<br/>GPIO · UART · Timer · PWM · Interrupt"]
    HW["Hardware and CMSIS register definitions<br/>STM32L4S5VITx peripherals"]

    APP --> MW
    APP --> DRV
    MW --> DRV
    DRV --> HW
```

### Folder structure

``` text
Embedded_Control_Platform/
├── Inc/          # Public headers and driver interfaces
├── Src/          # Application, middleware, and driver source files
├── Startup/      # MCU startup and interrupt vector code
├── .project      # STM32CubeIDE project settings
├── .cproject     # C/C++ build settings
├── STM32L4S5VITX_FLASH.ld
└── STM32L4S5VITX_RAM.ld
```

STM32CubeIDE may also create a `Debug/` directory containing generated
build files and output. These files are build products rather than the
main source code.

## Hardware and software

### Hardware

-   **Board:** B-L4S5I-IOT01A Discovery kit
-   **Microcontroller:** STM32L4S5VITx
-   **Core:** Arm Cortex-M4
-   **LED/PWM output used by this project:** PB14, configured for TIM15
    Channel 1 alternate function
-   **UART pins configured by the firmware:** USART1 TX on PB6 and RX on
    PB7
-   **System timebase:** TIM2

Pin routing and the terminal connection should be checked against the
board documentation and the actual lab setup before testing.

### Software

-   STM32CubeIDE
-   STM32CubeL4 firmware package and CMSIS device headers
-   ARM GNU Toolchain supplied with or configured for STM32CubeIDE
-   A serial terminal application, such as Tera Term, PuTTY, or an
    equivalent terminal

## Build the firmware

1.  Install STM32CubeIDE.
2.  Install the STM32CubeL4 firmware package version used by the
    project, or update the project's include and library paths to match
    your installed package.
3.  Download or clone this repository.
4.  In STM32CubeIDE, select **File → Import → Existing Projects into
    Workspace**.
5.  Choose **Select root directory**, browse to the project folder, and
    select the project.
6.  Finish the import.
7.  Select the `Embedded_Control_Platform` project in Project Explorer.
8.  Choose **Project → Build Project**.
9.  Check the Console output for build errors.

The project targets the STM32L4S5VITx. Do not change the MCU or linker
script to a different STM32 family without updating the startup code,
device headers, build settings, and memory layout.

## Flash the firmware

1.  Connect the Discovery kit to your computer using the board's
    supported ST-LINK USB connection.
2.  In STM32CubeIDE, select the project and choose **Run → Debug** (or
    use the project's configured STM32 launch configuration).
3.  If prompted, allow STM32CubeIDE to create or select a debug
    configuration.
4.  Wait for programming to finish.
5.  Resume execution or reset the target so that `main()` runs.

The exact USB connector and debug configuration depend on the board
revision and local setup; follow the board user manual if the target is
not detected.

## Use the serial command interface

1.  Connect to the UART interface exposed by the board's ST-LINK virtual
    COM port or the serial adapter used in your setup.
2.  Open a serial terminal.
3.  Configure the terminal for **115200 baud, 8 data bits, no parity, 1
    stop bit (115200 8N1)**.
4.  Select the correct COM port.
5.  Reset or restart the board. The firmware prints a startup message.
6.  Type `HELP` and press Enter to display the commands.

Use one command at a time and wait for the response before sending the
next command.

## How the software is organized

### Application (`app.c`, `app.h`)

The application connects the drivers and services. It receives terminal
commands, chooses the requested action, and updates the LED or reports
uptime. The SOS and fade demonstrations use timer-based updates rather
than long blocking delays.

### Drivers

-   `gpio_driver.c` --- GPIO mode, read, write, and toggle operations.
-   `uart_driver.c` --- USART1 setup and byte/string transmission and
    reception.
-   `timer_driver.c` --- TIM2 timebase, delays, and callback support.
-   `pwm_driver.c` --- TIM15 PWM setup and duty-cycle control.
-   `interrupt_driver.c` --- NVIC interrupt enable and disable
    functions.

The drivers access MCU peripheral registers through CMSIS device
definitions. They do not use HAL functions to perform peripheral
operations.

### Middleware

-   `logging.c` formats timestamped messages and sends them over UART.
-   `error_handler.c` maps error codes to readable messages and reports
    their source location.
-   `config.c` stores small key-value settings in RAM.

Configuration values are **not retained across reset or power loss**,
because the current implementation uses RAM rather than non-volatile
storage.

## Design notes

-   The timer timebase assumes the peripheral clock configuration used
    by the project. If the clock tree is changed, recalculate the timer
    and UART settings.
-   UART logging is blocking: sending a log message waits for UART
    transmission to progress.
-   The timer callback, if used, should be short and should not perform
    blocking UART output.
-   The command interface uses a fixed-size buffer to keep memory use
    predictable.
-   Peripheral pin mappings and LED polarity depend on the board wiring
    and must match the selected hardware configuration.

## License

Add a license file if you intend to allow others to reuse or modify this
project. Until a license is included, do not assume that the repository
grants permission to reuse the code.
