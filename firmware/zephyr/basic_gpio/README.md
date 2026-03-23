# TXE8116 Basic GPIO Sample (Zephyr)

Minimal Zephyr sample for the TXE8116 16-bit SPI I/O expander. Reads a button and mirrors the state to an LED in a polling loop.

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

## Build & Flash

```bash
west build -b nucleo_l476rg
west flash
```

## Behaviour

LED follows button state with a 50 ms polling interval.
