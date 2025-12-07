#!/usr/bin/env python3
"""
TXE8116 Example - LED Blink
Demonstrates basic usage of the TXE8116 I/O expander
"""

from txe8116 import TXE8116
import time

# Initialize TXE8116
# - SPI bus 0, device 0 (CE0 = GPIO8)
# - Reset pin on GPIO22
io = TXE8116(bus=0, device=0, reset_pin=22)

# Check device ID (should be 0x00 for TXE8116)
device_id = io.read_device_id()
print(f'TXE8116 Device ID: 0x{device_id:02X}')

# Configure Port 0: all outputs, push-pull mode
# direction: 1=output, 0=input
# push_pull: 0=push-pull, 1=open-drain
io.configure_port(port=0, direction=0xFF, push_pull=0x00)

print('\nCycling through patterns on Port 0...')
print('Press Ctrl+C to stop\n')

try:
    while True:
        # All HIGH
        io.write_port(0, 0xFF)
        print('0xFF (all HIGH)')
        time.sleep(1)
        
        # Alternating pattern 1
        io.write_port(0, 0xAA)
        print('0xAA (10101010)')
        time.sleep(1)
        
        # Alternating pattern 2
        io.write_port(0, 0x55)
        print('0x55 (01010101)')
        time.sleep(1)
        
        # All LOW
        io.write_port(0, 0x00)
        print('0x00 (all LOW)')
        time.sleep(1)
        
        print()  # Blank line between cycles
        
except KeyboardInterrupt:
    print('\n\nStopped')
    
finally:
    # Clean up: set all pins LOW
    io.write_port(0, 0x00)
    io.close()
    print('GPIO cleaned up')
