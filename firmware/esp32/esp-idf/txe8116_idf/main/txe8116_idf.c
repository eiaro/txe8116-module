#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_log.h"

#include "txe81xx.h"

static const char *TAG = "irq_example";

#define TXE_RESET_LOW_MS       5
#define TXE_RESET_HIGH_MS      10
#define BLINK_FAST_MS          100
#define BLINK_SLOW_MS          300
#define DEBUG_LOG_DIVIDER      10

// ---------------- ESP32 wiring ----------------
#define PIN_NUM_MOSI   GPIO_NUM_7
#define PIN_NUM_MISO   GPIO_NUM_2
#define PIN_NUM_CLK    GPIO_NUM_6
#define PIN_NUM_CS     GPIO_NUM_10
#define PIN_NUM_RESET  GPIO_NUM_3
#define PIN_NUM_INT    GPIO_NUM_4   // TXE INT pin -> ESP GPIO4

// ---------------- TXE pins used ----------------
// LED on TXE P0.0, LED has pull-up => active LOW
#define LED_PORT       TXE_PORT0
#define LED_MASK       0x01

// Switch on TXE P1.1, switch closes to 3.3V => use pull-down => pressed = 1
#define SW_PORT        TXE_PORT1
#define SW_MASK        0x02

static volatile uint8_t g_pressed = 0;

static inline bool sw_is_pressed(uint8_t value)
{
    return (value & SW_MASK) != 0;
}

static inline TickType_t blink_delay_ticks(void)
{
    return pdMS_TO_TICKS(g_pressed ? BLINK_FAST_MS : BLINK_SLOW_MS);
}

// Simple read-modify-write helper for an 8-bit TXE register
static void hw_reset_txe(void)
{
    gpio_config_t io = {
        .pin_bit_mask = 1ULL << PIN_NUM_RESET,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_ERROR_CHECK(gpio_config(&io));

    gpio_set_level(PIN_NUM_RESET, 0);
    vTaskDelay(pdMS_TO_TICKS(TXE_RESET_LOW_MS));

    gpio_set_level(PIN_NUM_RESET, 1);
    vTaskDelay(pdMS_TO_TICKS(TXE_RESET_HIGH_MS));
}

static void irq_cb(txe81xx_port_t port, uint8_t flags, uint8_t state, void *ctx)
{
    (void)ctx;

    if (port == SW_PORT && (flags & SW_MASK)) {
        g_pressed = sw_is_pressed(state) ? 1 : 0;
        ESP_EARLY_LOGI(TAG, "IRQ: flags=0x%02X state=0x%02X pressed=%u", flags, state, g_pressed);
    }
}

static void configure_led_and_switch(txe81xx_handle_t io, uint8_t *out_p0)
{
    ESP_ERROR_CHECK(txe81xx_update_pushpull_od(io, LED_PORT, LED_MASK, 0));
    ESP_ERROR_CHECK(txe81xx_update_direction(io, LED_PORT, LED_MASK, 0));

    ESP_ERROR_CHECK(txe81xx_read_outputs(io, LED_PORT, out_p0));
    *out_p0 |= LED_MASK;
    ESP_ERROR_CHECK(txe81xx_write_outputs(io, LED_PORT, *out_p0));

    ESP_ERROR_CHECK(txe81xx_update_direction(io, SW_PORT, 0, SW_MASK));
    ESP_ERROR_CHECK(txe81xx_update_pull_enable_mask(io, SW_PORT, SW_MASK, 0));
    ESP_ERROR_CHECK(txe81xx_update_pull_select_mask(io, SW_PORT, 0, SW_MASK));
    ESP_ERROR_CHECK(txe81xx_update_glitch_filter(io, SW_PORT, SW_MASK, 0));
}

static void configure_txe_interrupts(txe81xx_handle_t io)
{
    ESP_ERROR_CHECK(txe81xx_irq_configure_port(io, SW_PORT, true, 0xFF));
    ESP_ERROR_CHECK(txe81xx_irq_enable_pins(io, SW_PORT, SW_MASK));
    ESP_ERROR_CHECK(txe81xx_irq_clear_pending(io));
}

static void log_periodic_debug(txe81xx_handle_t io)
{
    uint8_t flags = 0;
    uint8_t port_status = 0;
    uint8_t raw = 0;

    ESP_ERROR_CHECK(txe81xx_irq_read_port_status(io, &port_status));
    ESP_ERROR_CHECK(txe81xx_irq_read_flags(io, TXE_PORT1, &flags));
    ESP_ERROR_CHECK(txe81xx_read_inputs(io, SW_PORT, &raw));

    ESP_LOGI(TAG,
             "GPIO4=%d rawP1=0x%02X flagsP1=0x%02X portStat=0x%02X pressed=%u",
             gpio_get_level(PIN_NUM_INT), raw, flags, port_status, g_pressed);
}

void app_main(void)
{
    ESP_LOGI(TAG, "Starting TXE IRQ example");

    // ---------- Reset TXE ----------
    hw_reset_txe();

    // ---------- Init SPI bus ----------
    spi_bus_config_t buscfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO));

    // ---------- Init TXE component ----------
    txe81xx_handle_t io;
    txe81xx_spi_config_t cfg = {
        .host = SPI2_HOST,
        .cs_gpio = PIN_NUM_CS,
        .clock_hz = 100 * 1000,
        .mode = 0,               // datasheet
        .queue_size = 1,
        .use_polling = true,
        .chip = TXE_CHIP_8116,
    };
    ESP_ERROR_CHECK(txe81xx_init_spi(&cfg, &io));

    // ---------- Clear POR / fault ----------
    uint8_t fault = 0;
    ESP_ERROR_CHECK(txe81xx_read_fault_status(io, &fault));
    ESP_LOGI(TAG, "FAULT STATUS = 0x%02X", fault);

    // ---------- Sanity: read device id ----------
    uint8_t id = 0xFF;
    ESP_ERROR_CHECK(txe81xx_read_device_id(io, &id));
    ESP_LOGI(TAG, "Device ID: %u (0x%02X)", id, id);

    uint8_t out_p0 = 0;
    configure_led_and_switch(io, &out_p0);

    configure_txe_interrupts(io);

    ESP_LOGI(TAG, "GPIO4 before IRQ install = %d", gpio_get_level(PIN_NUM_INT));

    // ---------- Install ESP-side IRQ ----------
    txe81xx_irq_config_t irq_cfg = {
        .int_gpio = PIN_NUM_INT,   // GPIO4
        .active_low = true,        // inverted / active-low
        .cb = irq_cb,
        .user_ctx = NULL,
        .task_stack = 3072,
        .task_prio = 10,
        .task_core = -1,
    };
    ESP_ERROR_CHECK(txe81xx_irq_install(io, &irq_cfg));

    // ---------- Initial raw switch read ----------
    uint8_t in_p1 = 0;
    ESP_ERROR_CHECK(txe81xx_read_inputs(io, SW_PORT, &in_p1));
    g_pressed = sw_is_pressed(in_p1) ? 1 : 0;

    ESP_LOGI(TAG, "Initial switch raw P1=0x%02X pressed=%u", in_p1, g_pressed);

    // ---------- Main loop ----------
    while (1) {
        // Blink LED. Faster when button pressed.
        // LED active low:
        //  bit0 = 0 -> ON
        //  bit0 = 1 -> OFF

        // ON
        out_p0 &= (uint8_t)~LED_MASK;
        ESP_ERROR_CHECK(txe81xx_write_outputs(io, LED_PORT, out_p0));
        vTaskDelay(blink_delay_ticks());

        // OFF
        out_p0 |= LED_MASK;
        ESP_ERROR_CHECK(txe81xx_write_outputs(io, LED_PORT, out_p0));
        vTaskDelay(blink_delay_ticks());

        // Optional slow debug
        static int dbg_div = 0;
        dbg_div++;
        if (dbg_div >= DEBUG_LOG_DIVIDER) {
            dbg_div = 0;
            log_periodic_debug(io);
        }
    }
}