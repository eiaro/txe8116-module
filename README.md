# TXE8116 Module

Open-source hardware module for the TXE8116 — a 16-bit SPI I/O expander with two 8-bit ports, configurable push-pull/open-drain outputs, pull resistors, interrupt support, and per-pin glitch filtering.

## Repository Structure

```
txe8116-module/
├── hardware/          # KiCad schematic and PCB (CC BY-SA 4.0)
├── firmware/          # Driver and example code (MIT)
│   ├── esp32/         # ESP-IDF example
│   ├── zephyr/        # Zephyr RTOS examples
│   ├── raspberrypi/   # Python driver and C example
│   ├── arduino/       # Arduino example
│   └── beaglebone/    # BeagleBone C example
└── docs/              # Wiring guides
```

## Firmware

### ESP32 (ESP-IDF)

Uses the [`eiaro/txe81xx`](https://components.espressif.com/components/eiaro/txe81xx) component from the ESP component registry.

```bash
cd firmware/esp32/esp-idf/txe8116_idf
idf.py build flash monitor
```

See [`firmware/esp32/esp-idf/txe8116_idf/README.md`](firmware/esp32/esp-idf/txe8116_idf/README.md).

### Zephyr RTOS

Uses the [`xylon-labs/zephyr-txe81xx`](https://github.com/xylon-labs/zephyr-txe81xx) driver module.

| Example | Description |
|---------|-------------|
| [`firmware/zephyr/basic_gpio`](firmware/zephyr/basic_gpio/README.md) | Polling — LED mirrors button state |
| [`firmware/zephyr/gpio_interrupt`](firmware/zephyr/gpio_interrupt/README.md) | Interrupt-driven — LED toggles on button events |

```bash
west build -b nucleo_l476rg firmware/zephyr/basic_gpio
```

### Raspberry Pi (Python)

```bash
cd firmware/raspberrypi/python
python3 example.py
```

See [`firmware/raspberrypi/python/README.md`](firmware/raspberrypi/python/README.md).

## Hardware

KiCad 8.x project in [`hardware/txe8116-module/`](hardware/txe8116-module/README.md). Fabrication files (Gerbers, BOM, pick & place) are in `hardware/txe8116-module/fabrication/`.

## License

- **Hardware** (`hardware/`): [CC BY-SA 4.0](hardware/LICENSE)
- **Firmware / Software** (`firmware/`): [MIT](firmware/LICENSE)
