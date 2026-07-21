// components/display_gfx/host_test/test_display_gfx/main/test_interfaces.cpp

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "interfaces/i_graphics_context.hpp"
#include "interfaces/i_hal_display.hpp"
#include "mock_hal_display.hpp"

using ::testing::Return;
using ::testing::_;

class DummyDefaultDisplay : public IHalDisplay {
public:
    esp_err_t init() override { return ESP_OK; }
    esp_err_t flush_buffer(const uint8_t* buffer, size_t size) override {
        (void)buffer;
        (void)size;
        return ESP_OK;
    }
    esp_err_t set_brightness(uint8_t level) override {
        (void)level;
        return ESP_OK;
    }
    esp_err_t set_power(bool enabled) override {
        (void)enabled;
        return ESP_OK;
    }
};

TEST(DisplayGfxInterfacesTest, MockHalDisplayBehavior) {
    MockHalDisplay mock_display;

    EXPECT_CALL(mock_display, init())
        .WillOnce(Return(ESP_OK));

    EXPECT_CALL(mock_display, flush_buffer(_, 1024))
        .WillOnce(Return(ESP_OK));

    EXPECT_CALL(mock_display, set_brightness(128))
        .WillOnce(Return(ESP_OK));

    EXPECT_CALL(mock_display, set_power(true))
        .WillOnce(Return(ESP_OK));

    EXPECT_CALL(mock_display, supports_touch())
        .WillOnce(Return(false));

    EXPECT_CALL(mock_display, supports_backlight())
        .WillOnce(Return(true));

    EXPECT_EQ(mock_display.init(), ESP_OK);
    uint8_t dummy_buffer[1024] = {0};
    EXPECT_EQ(mock_display.flush_buffer(dummy_buffer, sizeof(dummy_buffer)), ESP_OK);
    EXPECT_EQ(mock_display.set_brightness(128), ESP_OK);
    EXPECT_EQ(mock_display.set_power(true), ESP_OK);
    EXPECT_FALSE(mock_display.supports_touch());
    EXPECT_TRUE(mock_display.supports_backlight());
}

TEST(DisplayGfxInterfacesTest, DefaultInterfaceCapabilities) {
    DummyDefaultDisplay default_display;
    EXPECT_FALSE(default_display.supports_touch());
    EXPECT_TRUE(default_display.supports_backlight());
}
