#include "txe8116.h"

static esp_err_t txe81xx_transmit(txe81xx_dev_t *dev, spi_transaction_t *t)
{
    esp_err_t ret;
    spi_device_handle_t handle = dev->spi_handle;

    if (dev->use_software_cs) {
        // Manually control CS line
        gpio_set_level(dev->cs_gpio, 0); // Assert CS
    }

    ret = spi_device_transmit(handle, t);

    if (dev->use_software_cs) {
        gpio_set_level(dev->cs_gpio, 1); // Deassert CS
    }

    return ret;    
}