#pragma once

#define TXE81XX_MAX_SPI_FREQ SPI_MASTER_FREQ_10M //!< Maximum SPI frequency for TXE81xx devices

//Define the TXE81xx Register Map from the TXE81xx d
#define CMD_SCRATCH_REG (0x00)
#define CMD_DEVICE_ID (0x01)
#define CMD_INPUT_REG (0x02)
#define CMD_OUTPUT_REG (0x03)
#define CMD_CONFIG_REG (0x04)
#define CMD_POL_REG (0x05)
#define CMD_PP_OD_SEL (0x06)
#define CMD_OD_CONF (0x07)
#define CMD_PU_PD_EN (0x08)
#define CMD_PU_PD_SEL (0x09)
#define CMD_BUS_HOLDER (0x0A)
#define CMD_SMART_INT (0x0B)
#define CMD_INT_MASK (0x0C)
#define CMD_GLITCH_FILTER_EN (0x0D)
#define CMD_INT_FLAG_STATUS (0x0E)
#define CMD_INT_PORT_STATUS (0x0F)
#define CMD_FAIL_SAFE_EN_REG_1 (0x12)
#define CMD_FAIL_SAFE_EN_REG_2 (0x13)
#define CMD_FAIL_SAFE_DIR_CONFIG1 (0x14)
#define CMD_FAIL_SAFE_DIR_CONFIG2 (0x15)
#define CMD_FAIL_SAFE_OUT1 (0x16)
#define CMD_FAIL_SAFE_OUT2 (0x17)
#define CMD_FAIL_SAFE_REDUN (0x18)
#define CMD_FAIL_SAFE_FAULT (0x19)
#define CMD_SOFTWARE_RESET (0x1A)
//define each port
//TXE8116 - Port 0 / Port 1
//TXE8124 - Port 0 / Port 1 / Port 2
#define port0 (0x00)
#define port1 (0x01)
#define port2 (0x02)

typedef struct txe81xx_dev {
    spi_device_interface_config_t spi_cfg;
    spi_device_handle_t spi_dev;
    bool use_software_cs; //!< Use software CS control instead of hardware.
    //!< Whether to use a software Chip Select (CS) line instead of the hardware
    //!< one. This is useful when multiple TXE81xx chips are sharing the same CS
    //!< line on the SPI bus.
    gpio_port_t cs_gpio;      //!< GPIO port number for software CS control.

    bool is_txe8124; //!< Set to true if the device is TXE8124, false for TXE8116.
} txe81xx_dev_t;

/**
 * @brief Initialize the TXE81xx device descriptor.
 */
esp_err_t txe81xx_init_desc(txe81xx_dev_t *dev, spi_host_device_t host, uint32_t clock_speed, gpio_num_t cs_gpio);

/**
 * @brief Free the TXE81xx device descriptor.
 */
esp_err_t txe81xx_free_desc(txe81xx_dev_t *dev);


esp_err_t txe81xx_transmit(txe81xx_dev_t *dev, spi_transaction_t *t);

esp_err_t txe81xx_read_register(txe81xx_dev_t *dev, uint8_t reg_addr, uint16_t *data);