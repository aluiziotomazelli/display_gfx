// components/display_gfx/include/hal_display_ssd1306.hpp
#pragma once

#include <cstddef>
#include <cstdint>

#include "esp_err.h"

#if __has_include("esp_lcd_panel_io.h")
#define HAL_DISPLAY_SSD1306_HAS_ESP_LCD 1
#include "driver/i2c_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"
#else
#define HAL_DISPLAY_SSD1306_HAS_ESP_LCD 0
#endif

#include "i_hal_display.hpp"

/**
 * @brief Configuration parameters for SSD1306 OLED display.
 */
struct Ssd1306Config {
    void* i2c_bus = nullptr;    ///< The I2C master bus handle (i2c_master_bus_handle_t)
    uint8_t i2c_address = 0x3C; ///< The I2C address of the SSD1306 (typically 0x3C or 0x3D)
    uint16_t width = 128;       ///< Width of the display in pixels (e.g. 128)
    uint16_t height = 64;       ///< Display height in pixels
    int rst_gpio = -1;          ///< Reset GPIO pin (-1 if unused)
    uint32_t i2c_clk_speed_hz = 400000; ///< I2C clock speed in Hz (default 400kHz)
};

/**
 * @brief ESP-IDF HAL implementation for SSD1306 OLED display using esp_lcd component.
 */
class HalDisplaySsd1306 : public IHalDisplay {
public:
    /**
     * @brief Construct a new HalDisplaySsd1306 object.
     *
     * @param config Hardware configuration parameters.
     */
    explicit HalDisplaySsd1306(const Ssd1306Config& config = Ssd1306Config());

    ~HalDisplaySsd1306() override;

    esp_err_t init() override;
    esp_err_t flush_buffer(const uint8_t* buffer, size_t size) override;
    esp_err_t set_brightness(uint8_t level) override;
    esp_err_t set_power(bool enabled) override;

    bool supports_touch() const override { return false; }
    bool supports_backlight() const override { return false; }

private:
    Ssd1306Config config_;
#if HAL_DISPLAY_SSD1306_HAS_ESP_LCD
    esp_lcd_panel_io_handle_t io_handle_{nullptr};
    esp_lcd_panel_handle_t panel_handle_{nullptr};
#endif
    bool initialized_{false};
};
