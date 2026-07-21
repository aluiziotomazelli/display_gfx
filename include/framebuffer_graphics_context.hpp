// components/display_gfx/include/framebuffer_graphics_context.hpp
#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "esp_err.h"

#include "i_graphics_context.hpp"
#include "i_hal_display.hpp"

/**
 * @brief Monochrome Framebuffer implementation of IGraphicsContext.
 *
 * Manages an in-RAM 1-bit per pixel framebuffer arranged in SSD1306 page format (vertical 8-pixel bytes).
 */
class FramebufferGraphicsContext : public IGraphicsContext {
public:
    /**
     * @brief Construct a new FramebufferGraphicsContext object.
     *
     * @param display Reference to IHalDisplay for flushing.
     * @param phys_width Physical width of display in pixels (default 128).
     * @param phys_height Physical height of display in pixels (default 64).
     */
    explicit FramebufferGraphicsContext(IHalDisplay& display, uint16_t phys_width = 128, uint16_t phys_height = 64);

    ~FramebufferGraphicsContext() override = default;

    uint16_t get_width() const override;
    uint16_t get_height() const override;

    void set_pixel(int x, int y, uint32_t color) override;
    uint32_t get_pixel(int x, int y) const override;

    void draw_hline(int x, int y, int w, uint32_t color) override;
    void draw_vline(int x, int y, int h, uint32_t color) override;

    void clear(uint32_t color = 0) override;
    esp_err_t flush() override;

    void set_rotation(Rotation r) override;
    Rotation get_rotation() const override;

    void draw_char(int x, int y, char c, uint32_t color) override;
    void draw_string(int x, int y, const char* str, uint32_t color) override;

    /**
     * @brief Get direct access to raw buffer (useful for testing or direct hardware transfers).
     */
    const uint8_t* get_buffer() const { return buffer_.data(); }
    size_t get_buffer_size() const { return buffer_.size(); }

private:
    IHalDisplay& display_;
    uint16_t phys_width_;
    uint16_t phys_height_;
    Rotation rotation_;
    std::vector<uint8_t> buffer_;

    void transform_coords(int x, int y, int& px, int& py) const;
};
