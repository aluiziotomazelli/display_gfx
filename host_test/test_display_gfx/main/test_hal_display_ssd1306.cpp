// components/display_gfx/host_test/test_display_gfx/main/test_hal_display_ssd1306.cpp

#include <gtest/gtest.h>

#include "hal_display_ssd1306.hpp"

TEST(HalDisplaySsd1306Test, HostDummyBehavior) {
    Ssd1306Config config;
    config.width = 128;
    config.height = 64;
    config.i2c_address = 0x3C;
    config.i2c_bus = nullptr;

    HalDisplaySsd1306 display(config);

    // Initial state before init
    uint8_t buffer[1024] = {0};
    EXPECT_EQ(display.flush_buffer(buffer, sizeof(buffer)), ESP_ERR_INVALID_STATE);

    // Init on host
    EXPECT_EQ(display.init(), ESP_OK);

    // Flush after init
    EXPECT_EQ(display.flush_buffer(buffer, sizeof(buffer)), ESP_OK);
    EXPECT_EQ(display.set_power(true), ESP_OK);

    // Capabilities
    EXPECT_FALSE(display.supports_touch());
    EXPECT_FALSE(display.supports_backlight());
}
