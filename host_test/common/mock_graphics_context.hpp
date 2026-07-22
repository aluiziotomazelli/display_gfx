#pragma once

#include "i_graphics_context.hpp"
#include <gmock/gmock.h>

class MockGraphicsContext : public IGraphicsContext {
public:
    MOCK_METHOD(uint16_t, get_width, (), (const, override));
    MOCK_METHOD(uint16_t, get_height, (), (const, override));
    MOCK_METHOD(void, set_pixel, (int x, int y, uint32_t color), (override));
    MOCK_METHOD(uint32_t, get_pixel, (int x, int y), (const, override));
    MOCK_METHOD(void, draw_hline, (int x, int y, int w, uint32_t color), (override));
    MOCK_METHOD(void, draw_vline, (int x, int y, int h, uint32_t color), (override));
    MOCK_METHOD(void, draw_rect, (int x, int y, int w, int h, uint32_t color), (override));
    MOCK_METHOD(void, clear, (uint32_t color), (override));
    MOCK_METHOD(esp_err_t, flush, (), (override));
    MOCK_METHOD(void, set_rotation, (Rotation r), (override));
    MOCK_METHOD(Rotation, get_rotation, (), (const, override));
    MOCK_METHOD(void, draw_char, (int x, int y, char c, uint32_t color), (override));
    MOCK_METHOD(void, draw_string, (int x, int y, const char* str, uint32_t color), (override));
};
