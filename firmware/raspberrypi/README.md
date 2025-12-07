# TXE8116 - Raspberry Pi

Simple guide to get started with TXE8116 on Raspberry Pi.

## Quick Start

### 1. Enable SPI

```bash
sudo raspi-config
```
- Navigate to: **Interface Options → SPI → Enable**
- Reboot: `sudo reboot`

### 2. Verify SPI

After reboot:
```bash
ls /dev/spidev*
```
You should see `/dev/spidev0.0` and `/dev/spidev0.1`

### 3. Wiring

Connect TXE8116 to Raspberry Pi:

| TXE8116 Pin | RPi Pin | BCM GPIO | Function |
|-------------|---------|----------|----------|
| VCC | Pin 1 | - | 3.3V Power |
| GND | Pin 6 | - | Ground |
| CS | Pin 24 | GPIO 8 | SPI CE0 |
| SCK | Pin 23 | GPIO 11 | SPI SCLK |
| SDI (MOSI) | Pin 19 | GPIO 10 | SPI MOSI |
| SDO (MISO) | Pin 21 | GPIO 9 | SPI MISO |
| INT | Pin 11 | GPIO 17 | Interrupt (optional*) |
| RESET | Pin 15 | GPIO 22 | Reset (optional*) |

*Note: INT and RESET pins are optional but current hardware revision requires external pull-up resistors if not used. See [#8](https://github.com/eiaro/txe8116-module/issues/8) and [#9](https://github.com/eiaro/txe8116-module/issues/9) for details.

### 4. Test with Python

Install dependencies:
```bash
pip3 install spidev
```

Run example:
```bash
cd python/examples
python3 txe8116_spidev_example.py
```

## Languages & Frameworks

- **[Python](python/)** - High-level examples with spidev
- **[C](c/)** - Direct SPI access with `/dev/spidev`

## Troubleshooting

**SPI not enabled:**
```bash
# Check if SPI is enabled
lsmod | grep spi
# Should show: spi_bcm2835

# Alternative: add to /boot/config.txt
dtparam=spi=on
```

**Permission denied:**
```bash
# Add user to spi group
sudo usermod -a -G spi $USER
# Logout and login again
```

**Device not responding:**
- Check wiring (especially GND connection)
- Verify VCC is 3.3V (not 5V!)
- Measure SPI clock frequency with oscilloscope
- Check TXE8116 datasheet for timing requirements

## Resources

- [Raspberry Pi SPI Documentation](https://www.raspberrypi.com/documentation/computers/raspberry-pi.html#serial-peripheral-interface-spi)
- [TXE8116 Datasheet](../../docs/datasheet/)
- [Wiring Diagram](../../docs/wiring_rpi.md)
