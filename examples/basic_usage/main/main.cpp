// examples/basic_usage/main/main.cpp
#include <cstddef>
#include <cstdint>

#define LOG_LOCAL_LEVEL ESP_LOG_INFO
#include "esp_log.h"
#include "driver/i2c_master.h"
#include "esp_err.h"

#include "hal_display_ssd1306.hpp"
#include "framebuffer_graphics_context.hpp"

static const char* TAG = "basic_usage";

#define I2C_MASTER_SDA_IO 21 ///< GPIO pin for I2C SDA
#define I2C_MASTER_SCL_IO 22 ///< GPIO pin for I2C SCL

/**
 * @brief Initialize I2C driver on the ESP32 chip using new driver.
 */
static esp_err_t init_i2c(i2c_master_bus_handle_t* bus_handle)
{
    i2c_master_bus_config_t bus_config = {};
    bus_config.i2c_port = I2C_NUM_0;
    bus_config.sda_io_num = (gpio_num_t)I2C_MASTER_SDA_IO;
    bus_config.scl_io_num = (gpio_num_t)I2C_MASTER_SCL_IO;
    bus_config.clk_source = I2C_CLK_SRC_DEFAULT;
    bus_config.glitch_ignore_cnt = 7;
    bus_config.flags.enable_internal_pullup = true;

    return i2c_new_master_bus(&bus_config, bus_handle);
}

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Starting basic usage example for display_gfx component");

    // 1. Initialize the I2C master driver
    i2c_master_bus_handle_t i2c_bus = nullptr;
    esp_err_t err = init_i2c(&i2c_bus);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize I2C driver: %s", esp_err_to_name(err));
        return;
    }
    ESP_LOGI(TAG, "I2C driver initialized successfully on SDA:%d SCL:%d", I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO);

    // 2. Configure the SSD1306 display hardware
    Ssd1306Config display_config;
    display_config.i2c_bus = i2c_bus;  // Pass the new I2C bus handle
    display_config.i2c_address = 0x3C; // Standard SSD1306 address
    display_config.width = 128;
    display_config.height = 64;
    display_config.rst_gpio = -1; // No physical reset pin connected

    HalDisplaySsd1306 display(display_config);

    // 3. Initialize physical display HAL
    err = display.init();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize SSD1306 hardware HAL: %s", esp_err_to_name(err));
        return;
    }
    ESP_LOGI(TAG, "SSD1306 hardware HAL initialized successfully");

    // 4. Instantiate the graphics drawing context
    FramebufferGraphicsContext gfx(display);
    gfx.set_rotation(Rotation::ROTATION_180);

    // 5. Draw some patterns
    gfx.clear(0); // Clear to black

    // Draw border rect
    gfx.draw_rect(0, 0, 128, 64, 1);

    // Draw header text
    gfx.draw_string(14, 4, "SMART FARM HUB", 1);

    // Draw a horizontal divider line
    gfx.draw_hline(4, 14, 120, 1);

    // Draw status info
    gfx.draw_string(8, 20, "WiFi: CONNECTED", 1);
    gfx.draw_string(8, 32, "Water Level: 84%", 1);
    gfx.draw_string(8, 44, "Temp: 24.5 C", 1);

    // Draw decoration pattern
    gfx.draw_vline(118, 20, 36, 1);
    gfx.draw_rect(120, 32, 4, 12, 1);

    // 6. Flush drawings to the physical display
    err = gfx.flush();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to flush graphics to display: %s", esp_err_to_name(err));
        return;
    }
    ESP_LOGI(TAG, "Successfully flushed basic usage frame to display");
}
