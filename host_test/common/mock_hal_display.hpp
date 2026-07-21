// components/display_gfx/host_test/common/mock_hal_display.hpp
#pragma once

#include <gmock/gmock.h>
#include "interfaces/i_hal_display.hpp"

class MockHalDisplay : public IHalDisplay {
public:
    MOCK_METHOD(esp_err_t, init, (), (override));
    MOCK_METHOD(esp_err_t, flush_buffer, (const uint8_t* buffer, size_t size), (override));
    MOCK_METHOD(esp_err_t, set_brightness, (uint8_t level), (override));
    MOCK_METHOD(esp_err_t, set_power, (bool enabled), (override));
    MOCK_METHOD(bool, supports_touch, (), (const, override));
    MOCK_METHOD(bool, supports_backlight, (), (const, override));
};
