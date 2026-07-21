// components/display_gfx/include/interfaces/i_graphics_context.hpp
#pragma once

#include <cstdint>

#include "esp_err.h"

enum class Rotation {
    ROTATION_0 = 0,
    ROTATION_90,
    ROTATION_180,
    ROTATION_270
};

/**
 * @brief Abstract interface representing a graphics drawing context.
 */
class IGraphicsContext {
public:
    virtual ~IGraphicsContext() = default;

    /**
     * @brief Get logical width of the drawing area.
     *
     * @return Width in pixels.
     */
    virtual uint16_t get_width() const = 0;

    /**
     * @brief Get logical height of the drawing area.
     *
     * @return Height in pixels.
     */
    virtual uint16_t get_height() const = 0;

    /**
     * @brief Draw or modify a single pixel in the framebuffer.
     *
     * @param x X coordinate.
     * @param y Y coordinate.
     * @param color Color value (1 or 0 for monochrome).
     */
    virtual void set_pixel(int x, int y, uint32_t color) = 0;

    /**
     * @brief Get pixel value at specified logical coordinates.
     *
     * @param x X coordinate.
     * @param y Y coordinate.
     * @return Color value (1 or 0 for monochrome, 0 if out of bounds).
     */
    virtual uint32_t get_pixel(int x, int y) const = 0;

    /**
     * @brief Draw a horizontal line.
     *
     * @param x Starting X coordinate.
     * @param y Y coordinate.
     * @param w Line width.
     * @param color Color value.
     */
    virtual void draw_hline(int x, int y, int w, uint32_t color) = 0;

    /**
     * @brief Draw a vertical line.
     *
     * @param x X coordinate.
     * @param y Starting Y coordinate.
     * @param h Line height.
     * @param color Color value.
     */
    virtual void draw_vline(int x, int y, int h, uint32_t color) = 0;

    /**
     * @brief Clear the entire graphics context with a given color.
     *
     * @param color Background fill color (default 0).
     */
    virtual void clear(uint32_t color = 0) = 0;

    /**
     * @brief Flush the current framebuffer to the underlying display device.
     *
     * @return ESP_OK: on success
     * @return esp_err_t: error code on failure
     */
    virtual esp_err_t flush() = 0;

    /**
     * @brief Set display rotation.
     *
     * @param r Rotation mode.
     */
    virtual void set_rotation(Rotation r) = 0;

    /**
     * @brief Get current display rotation.
     *
     * @return Current rotation.
     */
    virtual Rotation get_rotation() const = 0;

    /**
     * @brief Draw a rectangle outline.
     */
    virtual void draw_rect(int x, int y, int w, int h, uint32_t color) {
        if (w <= 0 || h <= 0) return;
        draw_hline(x, y, w, color);
        draw_hline(x, y + h - 1, w, color);
        draw_vline(x, y, h, color);
        draw_vline(x + w - 1, y, h, color);
    }

    /**
     * @brief Fill a solid rectangle.
     */
    virtual void fill_rect(int x, int y, int w, int h, uint32_t color) {
        if (w <= 0 || h <= 0) return;
        for (int i = 0; i < h; ++i) {
            draw_hline(x, y + i, w, color);
        }
    }

    /**
     * @brief Draw a single character using built-in 5x7 font.
     */
    virtual void draw_char(int x, int y, char c, uint32_t color) = 0;

    /**
     * @brief Draw a null-terminated string using built-in 5x7 font.
     */
    virtual void draw_string(int x, int y, const char* str, uint32_t color) = 0;

    /**
     * @brief Placeholder for drawing a circle outline.
     */
    virtual void draw_circle(int x0, int y0, int r, uint32_t color) {
        (void)x0;
        (void)y0;
        (void)r;
        (void)color;
    }
};

