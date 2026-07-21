// components/display_gfx/src/framebuffer_graphics_context.cpp
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>

#define LOG_LOCAL_LEVEL ESP_LOG_INFO
#include "esp_log.h"

#include "fonts/font5x7.hpp"
#include "i_graphics_context.hpp"
#include "i_hal_display.hpp"
#include "framebuffer_graphics_context.hpp"

static const char *TAG = "FramebufferGraphicsContext";

FramebufferGraphicsContext::FramebufferGraphicsContext(IHalDisplay& display, uint16_t phys_width, uint16_t phys_height)
    : display_(display),
      phys_width_(phys_width),
      phys_height_(phys_height),
      rotation_(Rotation::ROTATION_0),
      buffer_((static_cast<size_t>(phys_width) * phys_height) / 8, 0) {
    ESP_LOGI(TAG, "Framebuffer initialized: %ux%u (%zu bytes)", phys_width_, phys_height_, buffer_.size());
}

uint16_t FramebufferGraphicsContext::get_width() const {
    if (rotation_ == Rotation::ROTATION_90 || rotation_ == Rotation::ROTATION_270) {
        return phys_height_;
    }
    return phys_width_;
}

uint16_t FramebufferGraphicsContext::get_height() const {
    if (rotation_ == Rotation::ROTATION_90 || rotation_ == Rotation::ROTATION_270) {
        return phys_width_;
    }
    return phys_height_;
}

void FramebufferGraphicsContext::transform_coords(int x, int y, int& px, int& py) const {
    switch (rotation_) {
        case Rotation::ROTATION_0:
            px = x;
            py = y;
            break;
        case Rotation::ROTATION_90:
            px = phys_width_ - 1 - y;
            py = x;
            break;
        case Rotation::ROTATION_180:
            px = phys_width_ - 1 - x;
            py = phys_height_ - 1 - y;
            break;
        case Rotation::ROTATION_270:
            px = y;
            py = phys_height_ - 1 - x;
            break;
    }
}

void FramebufferGraphicsContext::set_pixel(int x, int y, uint32_t color) {
    if (x < 0 || x >= get_width() || y < 0 || y >= get_height()) {
        return;
    }
    int px = 0;
    int py = 0;
    transform_coords(x, y, px, py);

    if (px < 0 || px >= phys_width_ || py < 0 || py >= phys_height_) {
        return;
    }

    size_t byte_idx = static_cast<size_t>((py / 8) * phys_width_ + px);
    uint8_t bit_mask = static_cast<uint8_t>(1 << (py % 8));

    if (color != 0) {
        buffer_[byte_idx] |= bit_mask;
    } else {
        buffer_[byte_idx] &= static_cast<uint8_t>(~bit_mask);
    }
}

uint32_t FramebufferGraphicsContext::get_pixel(int x, int y) const {
    if (x < 0 || x >= get_width() || y < 0 || y >= get_height()) {
        return 0;
    }
    int px = 0;
    int py = 0;
    transform_coords(x, y, px, py);

    if (px < 0 || px >= phys_width_ || py < 0 || py >= phys_height_) {
        return 0;
    }

    size_t byte_idx = static_cast<size_t>((py / 8) * phys_width_ + px);
    uint8_t bit_mask = static_cast<uint8_t>(1 << (py % 8));

    return (buffer_[byte_idx] & bit_mask) ? 1 : 0;
}

void FramebufferGraphicsContext::draw_hline(int x, int y, int w, uint32_t color) {
    for (int i = 0; i < w; ++i) {
        set_pixel(x + i, y, color);
    }
}

void FramebufferGraphicsContext::draw_vline(int x, int y, int h, uint32_t color) {
    for (int i = 0; i < h; ++i) {
        set_pixel(x, y + i, color);
    }
}

void FramebufferGraphicsContext::clear(uint32_t color) {
    uint8_t val = (color != 0) ? 0xFF : 0x00;
    std::fill(buffer_.begin(), buffer_.end(), val);
}

esp_err_t FramebufferGraphicsContext::flush() {
    return display_.flush_buffer(buffer_.data(), buffer_.size());
}

void FramebufferGraphicsContext::set_rotation(Rotation r) {
    rotation_ = r;
}

Rotation FramebufferGraphicsContext::get_rotation() const {
    return rotation_;
}

void FramebufferGraphicsContext::draw_char(int x, int y, char c, uint32_t color) {
    if (c < 32 || c > 126) {
        c = '?';
    }
    size_t font_idx = static_cast<size_t>(c - 32);
    for (int col = 0; col < 5; ++col) {
        uint8_t line = font5x7[font_idx][col];
        for (int row = 0; row < 7; ++row) {
            if ((line >> row) & 1) {
                set_pixel(x + col, y + row, color);
            }
        }
    }
}

void FramebufferGraphicsContext::draw_string(int x, int y, const char* str, uint32_t color) {
    if (!str) return;
    int current_x = x;
    while (*str) {
        draw_char(current_x, y, *str, color);
        current_x += 6; // 5 pixels width + 1 pixel gap
        str++;
    }
}
