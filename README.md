# STM32 Temperature Control

A real-time temperature control system built with an **STM32F407VGT6 evaluation board**. The system supports both **heating and cooling**, with temperature feedback from an analog NTC sensor and fan control through an L298N motor driver.

The project was developed using **CLion, CMake, and STM32CubeMX**, with **STM32 HAL, FreeRTOS, and the CMSIS-RTOS v2 API**.

## Features

- STM32F407VGT6 based
- Analog NTC temperature sensing
- ADC + DMA sensor acquisition
- Moving-average filtering
- Automatic temperature control
- Fan cooling
- Heater control
- PWM fan speed control
- L298N motor driver
- Fan/heater mutual exclusion
- Character LCD
- Three physical buttons
- RGB LED status indication
- UART command-line interface
- UART RX/TX using DMA
- FreeRTOS multitasking
- CMSIS-RTOS v2 API
- RTOS semaphore and software timer usage
- Independent watchdog
- CMake-based build system

---

## Hardware

| Component | Purpose |
|---|---|
| STM32F407VGT6 Evaluation Board | Main controller |
| NTC sensor | Temperature measurement |
| L298N | Fan motor driver |
| DC Fan | Cooling |
| Heater | Heating |
| Character LCD | Local display |
| 3 Buttons | User input |
| RGB LED | Status indication |

### STM32F407VGT6

The project uses the STM32F407VGTx MCU with a **168 MHz system clock**.

Main peripherals used:

- ADC1
- TIM5
- USART1
- DMA2
- IWDG
- GPIO
- FreeRTOS

---

## Temperature Sensor

An analog NTC sensor is connected to:

**PA5 / ADC1_IN5**

ADC configuration:

- 12-bit resolution
- 0–4095 ADC range
- DMA enabled
- Circular DMA
- 3-cycle sampling time

Five ADC samples are averaged to reduce measurement noise.

The current implementation converts the ADC reading into a voltage-scaled value using a 3.3 V reference.

> The current firmware does not yet implement a complete NTC resistance-to-temperature calculation such as the Beta or Steinhart-Hart equation. This can be added later depending on the NTC and resistor values used.

---

## Fan Control

The cooling fan is controlled through an **L298N**.

### Connections

| Function | STM32 Pin |
|---|---|
| Fan PWM | PA0 / TIM5_CH1 |
| Fan Direction A | PA1 |
| Fan Direction B | PA2 |

Fan speed is controlled using PWM and represented from **0–99**.

The automatic controller calculates the fan speed from the difference between the current and target temperatures.

A minimum fan command of approximately **31%** is used once cooling is required, helping the motor reliably start.

Fan speed changes are also smoothed using an exponential moving average with:

```text
EMA_ALPHA = 0.2
```

The fan control task runs approximately every **200 ms**.

---

## Heater Control

The system supports automatic heating as well as cooling.

The heater uses a **1°C hysteresis** around the target temperature to prevent rapid ON/OFF switching.

For example, with a target of 25°C:

- Below 24°C → heater ON
- 24–26°C → maintain current state
- Above 26°C → heater OFF

In manual mode, the current implementation keeps the heater OFF.

The heater state is also represented using the RGB LED's green output.

---

## Fan / Heater Safety

The firmware prevents the fan and heater from being actively controlled at the same time.

When the heater is active, the fan controller forces the fan toward zero speed.

This provides a basic mutual-exclusion mechanism between heating and cooling.

---

## LCD

A character LCD is connected through a parallel GPIO interface.

| LCD Signal | Pin |
|---|---|
| EN | PD7 |
| RW | PD10 |
| RS | PD11 |
| D4 | PD15 |
| D5 | PD14 |
| D6 | PD13 |
| D7 | PD12 |

The LCD functionality is separated into its own application module and RTOS task.

---

## Buttons

Three buttons are configured using external interrupts:

| Button | Pin |
|---|---|
| BTN1 | PE4 |
| BTN2 | PE5 |
| BTN3 | PE6 |

They are used for local system interaction, including mode and target-temperature control.

Button handling also uses an RTOS semaphore and debounce timer.

---

# UART CLI

The project includes a simple UART command-line interface for controlling and monitoring the system.

UART:

**USART1**

| Signal | Pin |
|---|---|
| TX | PA9 |
| RX | PA10 |

UART reception uses:

```c
HAL_UARTEx_ReceiveToIdle_DMA()
```

so commands can be received through DMA without blocking the main application.

The CLI uses a **256-byte DMA buffer**.

## Commands

### Automatic mode

```text
$MODE=AUTO
```

Enables automatic temperature control.

### Manual mode

```text
$MODE=MANUAL
```

Switches to manual mode.

### Set target temperature

```text
$SET_TEMP25
```

Sets the target temperature to 25.

### Get system status

```text
$STATUS
```

Prints the current system information.

Example:

```text
Log : STATUS:
MODE = AUTO
Target temp: 25
current temp: 23
fan speed : 0
STATE : Warning
HEATER ON
```

Invalid commands are reported as:

```text
Log : INVALID COMMAND
```

---

# RTOS

The project uses **FreeRTOS through the CMSIS-RTOS v2 API**.

Application tasks are separated by responsibility:

| Task | Responsibility |
|---|---|
| Sensor | ADC acquisition and filtering |
| Fan | Fan speed control |
| Heater | Heater control |
| LCD | Display updates |
| CLI | UART command processing |
| Status | System state and watchdog |
| Default | Default RTOS task |

The application uses CMSIS-RTOS functions such as:

```c
osThreadNew()
osDelay()
osSemaphoreNew()
osTimerNew()
```

### Approximate task stack sizes

| Task | Stack |
|---|---:|
| Sensor | 2048 |
| LCD | 2048 |
| CLI | 4096 |
| Fan | 1024 |
| Heater | 1024 |
| Status | 2048 |

---

# DMA

DMA is used for the main high-frequency peripheral transfers.

### ADC

ADC1 uses:

**DMA2 Stream 0**

for continuous ADC data transfer.

### UART RX

USART1 RX uses:

**DMA2 Stream 2**

with receive-to-idle detection.

### UART TX

USART1 TX uses:

**DMA2 Stream 7**

for transmission.

---

# Watchdog

The STM32 independent watchdog (**IWDG**) is used to detect a stalled system.

The status task periodically refreshes the watchdog:

```c
HAL_IWDG_Refresh(&hiwdg);
```

If the firmware stops reaching the watchdog refresh point, the MCU can reset.

---

# System State

The application maintains a shared system status containing:

- Current temperature
- Target temperature
- Operating mode
- Fan speed
- Heater state
- System state

The system can operate in:

```text
AUTO
MANUAL
```

and reports states such as:

```text
Init
Normal
Warning
Error
```

---

# Pin Mapping

| Function | Pin |
|---|---|
| Fan PWM | PA0 |
| Fan Direction A | PA1 |
| Fan Direction B | PA2 |
| NTC | PA5 |
| UART TX | PA9 |
| UART RX | PA10 |
| Board LED | PC13 |
| LCD EN | PD7 |
| LCD RW | PD10 |
| LCD RS | PD11 |
| LCD D4 | PD15 |
| LCD D5 | PD14 |
| LCD D6 | PD13 |
| LCD D7 | PD12 |
| Button 1 | PE4 |
| Button 2 | PE5 |
| Button 3 | PE6 |
| RGB Red | PE11 |
| RGB Green | PE13 |
| RGB Blue | PE14 |
| SWDIO | PA13 |
| SWCLK | PA14 |

---

# Project Structure

The application code is separated from the CubeMX-generated peripheral code.

```text
Core/
├── App/
│   ├── Include/
│   └── Source/
│       ├── cli.c
│       ├── fan.c
│       ├── heater.c
│       ├── lcd.c
│       ├── sensor.c
│       └── status.c
│
├── Library/
│   ├── ALCD.c
│   ├── ALCD.h
│   ├── DMAbuffer.c
│   └── DMAbuffer.h
│
├── Inc/
└── Src/

Drivers/
├── CMSIS/
└── STM32F4xx_HAL_Driver/

Middlewares/
└── Third_Party/
    └── FreeRTOS/

cmake/
└── stm32cubemx/

CMakeLists.txt
CMakePresets.json
STM32TempControl.ioc
STM32F407XX_FLASH.ld
startup_stm32f407xx.s
```

---

# Software Stack

- **IDE:** CLion
- **Configuration:** STM32CubeMX
- **Build system:** CMake
- **Compiler:** ARM GCC
- **MCU framework:** STM32 HAL
- **RTOS:** FreeRTOS
- **RTOS API:** CMSIS-RTOS v2
- **Language:** C11

The CubeMX project is stored in:

```text
STM32TempControl.ioc
```

---

# Clock Configuration

The MCU is configured for a **168 MHz system clock**.

The project uses a 25 MHz HSE with PLL configuration.

| Clock | Frequency |
|---|---:|
| SYSCLK | 168 MHz |
| AHB | 168 MHz |
| APB1 | 42 MHz |
| APB2 | 84 MHz |

---

# Build

Clone the repository:

```bash
git clone https://github.com/P34SANT/STM32_temperature_control.git
cd STM32_temperature_control
```

Open the project in **CLion**.

The project uses the root:

```text
CMakeLists.txt
```

as its build configuration.

The `.ioc` file can be opened in STM32CubeMX if the hardware configuration needs to be modified or regenerated.

---

# Debugging

The board can be programmed and debugged through SWD.

```text
PA13 → SWDIO
PA14 → SWCLK
```

An ST-Link compatible debugger can be used for flashing and debugging.

---

# Current Limitations

The project is functional as an embedded temperature-control demonstration, but there are areas that can be improved.

### NTC conversion

The current sensor implementation does not yet calculate actual temperature from NTC resistance. The sensor circuit's actual NTC parameters should be used to implement an accurate conversion.

### Heater output

The firmware's current heater output is represented through a GPIO/RGB indication. A real high-power heater requires an appropriate driver stage such as a MOSFET, relay, or SSR.

### Control algorithm

Fan control is currently proportional rather than PID-based. PID control could provide tighter temperature regulation.

### Fault handling

Additional protection could be added for:

- disconnected sensor
- shorted sensor
- over-temperature
- fan failure
- heater failure
- invalid ADC values

---

# Possible Improvements

- Proper NTC temperature conversion
- Sensor calibration
- PID temperature control
- More robust CLI parsing
- CLI help command
- Persistent configuration in flash
- Sensor fault detection
- Over-temperature protection
- Fan failure detection
- More structured RTOS communication using queues/mutexes
- Dedicated heater driver
- More detailed LCD interface

---

# Development Notes

This project was built to explore the combination of **STM32 peripherals, DMA, FreeRTOS, CMSIS-RTOS, and real-time control** in a single application.

The main goal was not just to read a temperature sensor, but to build a modular embedded system with:

- multiple concurrent tasks
- hardware abstraction
- actuator control
- user interfaces
- communication
- real-time scheduling
- basic safety logic

---

# License

No project license is currently specified.

If the project is intended for public reuse, a license can be added to the repository.

---

# Author

**P34SANT**

[GitHub Repository](https://github.com/P34SANT/STM32_temperature_control)
