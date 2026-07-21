// components/display_gfx/include/interfaces/i_graphics_context.hpp
#pragma once

#include <cstdint>

#include "esp_err.h"

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
     * @brief Placeholder for drawing a rectangle outline.
     */
    virtual void draw_rect(int x, int y, int w, int h, uint32_t color) {
        draw_hline(x, y, w, color);
        draw_hline(x, y + h - 1, w, color);
        draw_vline(x, y, h, color);
        draw_vline(x + w - 1, y, h, color);
    }

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
