# TXE8116 Python Driver

Python library for the TXE8116 16-bit SPI I/O expander on Raspberry Pi.

## Installation

```bash
# Install dependencies
sudo apt-get install python3-spidev python3-rpi.gpio

# Enable SPI
sudo raspi-config
# Interface Options → SPI → Enable
```

## Hardware Setup

### SPI Connections
- **VCC** → 3.3V
- **GND** → GND
- **SCLK** → GPIO11 (SCLK)
- **MOSI** → GPIO10 (MOSI)
- **MISO** → GPIO9 (MISO)
- **CS** → GPIO8 (CE0)
- **RESET** → GPIO22 (optional, recommended)

## Usage

```python
from txe8116 import TXE8116

# Initialize
io = TXE8116(bus=0, device=0, reset_pin=22)

# Configure Port 0 as all outputs
io.configure_port(port=0, direction=0xFF, push_pull=0x00)

# Write patterns
io.write_port(port=0, value=0xFF)  # All HIGH
io.write_port(port=0, value=0x00)  # All LOW

# Individual pin control
io.set_pin(port=0, pin=0)    # P0.0 HIGH
io.clear_pin(port=0, pin=0)  # P0.0 LOW
io.toggle_pin(port=0, pin=0) # Toggle P0.0

# Read inputs (set direction=0x00 first)
value = io.read_port(port=0)

# Clean up
io.close()
```

## Example

Run the example:
```bash
python3 example.py
```

## Notes

- **Direction**: `1` = output, `0` = input
- **Push-pull**: `0` = push-pull (default), `1` = open-drain
- **Ports**: Port 0 and Port 1, 8 pins each (16 total)
