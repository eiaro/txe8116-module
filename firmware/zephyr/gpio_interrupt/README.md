# TXE8116 GPIO Interrupt Sample (Zephyr)

Zephyr sample demonstrating interrupt-driven GPIO with the TXE8116 16-bit SPI I/O expander. The LED toggles on each button press and release.

## Requirements

- Zephyr RTOS v3.6 or later
- [xylon-labs/zephyr-txe81xx](https://github.com/xylon-labs/zephyr-txe81xx) driver module

## Hardware Setup

Board overlay provided for **STM32 Nucleo-L476RG** (`boards/nucleo_l476rg.overlay`).

| Signal | Pin   |
|--------|-------|
| SPI1 CS | PA4  |
| RESET   | PC6  |
| INT     | PC7  |

| TXE Pin | Function         |
|---------|-----------------|
| Pin 0   | LED (active low) |
| Pin 8   | Switch (active high, pull-down) |

A hardware glitch filter is enabled on the switch pin via `glitch-filter-pins` in the overlay.

## Build & Flash

```bash
west build -b nucleo_l476rg
west flash
```

## Behaviour

- Button press → LED toggles, logs `Switch: PRESSED`
- Button release → LED toggles, logs `Switch: RELEASED`
- Main thread sleeps indefinitely; all activity is interrupt-driven
