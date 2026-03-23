# TXE8116 ESP-IDF Example

ESP-IDF example for the TXE8116 16-bit SPI I/O expander using the [`eiaro/txe81xx`](https://components.espressif.com/components/eiaro/txe81xx) component from the ESP component registry.

Demonstrates interrupt-driven I/O: a button on TXE Port 1 controls the blink rate of an LED on TXE Port 0.

## Requirements

- ESP-IDF v5.0 or later
- ESP32-C6 (or adjust pin definitions in `main/txe8116_idf.c`)

## Hardware Setup

| Signal | ESP32 GPIO |
|--------|-----------|
| MOSI   | GPIO7     |
| MISO   | GPIO2     |
| CLK    | GPIO6     |
| CS     | GPIO10    |
| RESET  | GPIO3     |
| INT    | GPIO4     |

| TXE Pin | Function         |
|---------|-----------------|
| P0.0    | LED (active low) |
| P1.1    | Switch (active high, pull-down) |

## Build & Flash

```bash
idf.py build
idf.py flash monitor
```

The component manager will fetch `eiaro/txe81xx` automatically on first build.

## Behaviour

- LED blinks slowly (300 ms) when button is released
- LED blinks fast (100 ms) when button is held
- Button state is read via TXE interrupt — no polling
