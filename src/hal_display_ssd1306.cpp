// components/display_gfx/src/hal_display_ssd1306.cpp
#include <cstddef>
#include <cstdint>

#define LOG_LOCAL_LEVEL ESP_LOG_INFO
#include "esp_log.h"

#include "i_hal_display.hpp"
#include "hal_display_ssd1306.hpp"

static const char *TAG = "HalDisplaySsd1306";

HalDisplaySsd1306::HalDisplaySsd1306(const Ssd1306Config& config)
    : config_(config) {
}

HalDisplaySsd1306::~HalDisplaySsd1306() {
#if HAL_DISPLAY_SSD1306_HAS_ESP_LCD
    if (panel_handle_) {
        esp_lcd_panel_del(panel_handle_);
    }
    if (io_handle_) {
        esp_lcd_panel_io_del(io_handle_);
    }
#endif
}

esp_err_t HalDisplaySsd1306::init() {
#if !HAL_DISPLAY_SSD1306_HAS_ESP_LCD
    ESP_LOGI(TAG, "HalDisplaySsd1306 dummy init on host");
    initialized_ = true;
    return ESP_OK;
#else
    ESP_LOGI(TAG, "Initializing SSD1306 OLED display (%ux%u) on I2C port %d (addr 0x%02X)",
             config_.width, config_.height, config_.i2c_port, config_.i2c_address);

    esp_lcd_panel_io_i2c_config_t io_config = {};
    io_config.dev_addr = config_.i2c_address;
    io_config.scl_speed_hz = 400000;
    io_config.control_phase_bytes = 1;
    io_config.lcd_cmd_bits = 8;
    io_config.lcd_param_bits = 8;
    io_config.dc_bit_offset = 0;

    esp_err_t ret = esp_lcd_new_panel_io_i2c(static_cast<i2c_port_t>(config_.i2c_port), &io_config, &io_handle_);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create panel IO I2C: %s", esp_err_to_name(ret));
        return ret;
    }

    esp_lcd_panel_dev_config_t panel_config = {};
    panel_config.reset_gpio_num = config_.rst_gpio;
    panel_config.bits_per_pixel = 1;

    esp_lcd_panel_ssd1306_config_t ssd1306_config = {
        .height = static_cast<uint8_t>(config_.height)
    };
    panel_config.vendor_config = &ssd1306_config;

    ret = esp_lcd_new_panel_ssd1306(io_handle_, &panel_config, &panel_handle_);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create SSD1306 panel: %s", esp_err_to_name(ret));
        return ret;
    }

    ret = esp_lcd_panel_reset(panel_handle_);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to reset SSD1306 panel: %s", esp_err_to_name(ret));
        return ret;
    }

    ret = esp_lcd_panel_init(panel_handle_);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to init SSD1306 panel: %s", esp_err_to_name(ret));
        return ret;
    }

    ret = esp_lcd_panel_disp_on_off(panel_handle_, true);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to turn on SSD1306 display: %s", esp_err_to_name(ret));
        return ret;
    }

    initialized_ = true;
    ESP_LOGI(TAG, "SSD1306 display successfully initialized");
    return ESP_OK;
#endif
}

esp_err_t HalDisplaySsd1306::flush_buffer(const uint8_t* buffer, size_t size) {
    if (!initialized_) {
        ESP_LOGE(TAG, "Display not initialized prior to flush_buffer");
        return ESP_ERR_INVALID_STATE;
    }
    if (!buffer || size == 0) {
        return ESP_ERR_INVALID_ARG;
    }

#if !HAL_DISPLAY_SSD1306_HAS_ESP_LCD
    (void)buffer;
    (void)size;
    return ESP_OK;
#else
    return esp_lcd_panel_draw_bitmap(panel_handle_, 0, 0, config_.width, config_.height, buffer);
#endif
}

esp_err_t HalDisplaySsd1306::set_brightness(uint8_t level) {
    (void)level;
    return ESP_ERR_NOT_SUPPORTED;
}

esp_err_t HalDisplaySsd1306::set_power(bool enabled) {
    if (!initialized_) {
        return ESP_ERR_INVALID_STATE;
    }
#if !HAL_DISPLAY_SSD1306_HAS_ESP_LCD
    (void)enabled;
    return ESP_OK;
#else
    return esp_lcd_panel_disp_on_off(panel_handle_, enabled);
#endif
}
