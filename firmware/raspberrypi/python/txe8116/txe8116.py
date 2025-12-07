"""
TXE8116 - 16-bit SPI I/O Expander Driver for Raspberry Pi
"""

import spidev
import RPi.GPIO as GPIO
import time


class TXE8116:
    """Simple driver for TXE8116 16-bit I/O expander"""
    
    # Register addresses
    SCRATCH = 0x00
    DEVICE_ID = 0x01
    INPUT = 0x02
    OUTPUT = 0x03
    DIRECTION = 0x04
    POLARITY = 0x05
    PUSH_PULL = 0x06
    
    # Ports
    PORT0 = 0x00
    PORT1 = 0x01
    
    def __init__(self, bus=0, device=0, reset_pin=None, speed_hz=500000):
        """
        Initialize TXE8116
        
        Args:
            bus: SPI bus number (usually 0)
            device: SPI device (0 = CE0/GPIO8, 1 = CE1/GPIO7)
            reset_pin: BCM GPIO pin for reset (optional, recommended)
            speed_hz: SPI clock speed (default 500kHz)
        """
        self.spi = spidev.SpiDev()
        self.spi.open(bus, device)
        self.spi.max_speed_hz = speed_hz
        self.spi.mode = 0  # CPOL=0, CPHA=0
        
        self.reset_pin = reset_pin
        if reset_pin:
            GPIO.setmode(GPIO.BCM)
            GPIO.setup(reset_pin, GPIO.OUT)
            self.reset()
    
    def reset(self):
        """Hardware reset via GPIO pin"""
        if self.reset_pin:
            GPIO.output(self.reset_pin, GPIO.LOW)
            time.sleep(0.01)
            GPIO.output(self.reset_pin, GPIO.HIGH)
            time.sleep(0.1)
    
    def _transfer(self, register, port, data, multi_port=False):
        """
        Write to register and return previous value
        
        Args:
            register: Register address (0x00-0x1F)
            port: Port number (0 or 1)
            data: Data byte to write
            multi_port: Enable multi-port mode
            
        Returns:
            Previous register value
        """
        cmd_byte = register & 0x1F
        port_byte = (port << 4) & 0x70
        m_bit = 0x01 if multi_port else 0x00
        
        rx = self.spi.xfer2([cmd_byte, port_byte + m_bit, data])
        return rx[2]
    
    def _read(self, register, port):
        """
        Read from register
        
        Args:
            register: Register address (0x00-0x1F)
            port: Port number (0 or 1)
            
        Returns:
            Register value
        """
        cmd_byte = (register & 0x1F) | 0x80  # Set read bit
        port_byte = (port << 4) & 0x70
        
        rx = self.spi.xfer2([cmd_byte, port_byte, 0x00])
        return rx[2]
    
    def read_device_id(self):
        """Read device ID (should return 0x00 for TXE8116)"""
        return self._read(self.DEVICE_ID, self.PORT0)
    
    def configure_port(self, port, direction=0xFF, push_pull=0x00):
        """
        Configure a port
        
        Args:
            port: Port number (0 or 1)
            direction: Direction bits (1=output, 0=input). Default: all outputs
            push_pull: Push-pull mode (0=push-pull, 1=open-drain). Default: all push-pull
        """
        self._transfer(self.DIRECTION, port, direction)
        self._transfer(self.PUSH_PULL, port, push_pull)
    
    def write_port(self, port, value):
        """
        Write to output port
        
        Args:
            port: Port number (0 or 1)
            value: 8-bit value to write (1=HIGH, 0=LOW)
        """
        self._transfer(self.OUTPUT, port, value)
    
    def read_port(self, port):
        """
        Read from input port
        
        Args:
            port: Port number (0 or 1)
            
        Returns:
            8-bit input value
        """
        return self._read(self.INPUT, port)
    
    def read_output(self, port):
        """
        Read current output register value
        
        Args:
            port: Port number (0 or 1)
            
        Returns:
            8-bit output value
        """
        return self._read(self.OUTPUT, port)
    
    def set_pin(self, port, pin):
        """
        Set a single pin HIGH
        
        Args:
            port: Port number (0 or 1)
            pin: Pin number (0-7)
        """
        current = self.read_output(port)
        self.write_port(port, current | (1 << pin))
    
    def clear_pin(self, port, pin):
        """
        Set a single pin LOW
        
        Args:
            port: Port number (0 or 1)
            pin: Pin number (0-7)
        """
        current = self.read_output(port)
        self.write_port(port, current & ~(1 << pin))
    
    def toggle_pin(self, port, pin):
        """
        Toggle a single pin
        
        Args:
            port: Port number (0 or 1)
            pin: Pin number (0-7)
        """
        current = self.read_output(port)
        self.write_port(port, current ^ (1 << pin))
    
    def close(self):
        """Clean up resources"""
        self.spi.close()
        if self.reset_pin:
            GPIO.cleanup()
