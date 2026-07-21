// components/display_gfx/include/interfaces/i_hal_display.hpp
#pragma once

#include <cstddef>
#include <cstdint>

#include "esp_err.h"

/**
 * @brief Abstract interface representing a physical display hardware HAL.
 */
class IHalDisplay {
public:
    virtual ~IHalDisplay() = default;

    /**
     * @brief Initialize the physical display hardware and driver.
     *
     * @return ESP_OK: on success
     * @return esp_err_t: error code on failure
     */
    virtual esp_err_t init() = 0;

    /**
     * @brief Flush a pixel buffer to the display hardware.
     *
     * @param buffer Pointer to the raw frame buffer byte array.
     * @param size Size of the buffer in bytes.
     * @return ESP_OK: on success
     * @return esp_err_t: error code on failure
     */
    virtual esp_err_t flush_buffer(const uint8_t* buffer, size_t size) = 0;

    /**
     * @brief Set display brightness level.
     *
     * @param level Brightness value (0 to 255).
     * @return ESP_OK: on success
     * @return ESP_ERR_NOT_SUPPORTED: if hardware does not support brightness adjustment
     */
    virtual esp_err_t set_brightness(uint8_t level) = 0;

    /**
     * @brief Enable or disable display power (sleep/wake mode).
     *
     * @param enabled True to turn on display, false to enter low-power sleep mode.
     * @return ESP_OK: on success
     * @return esp_err_t: error code on failure
     */
    virtual esp_err_t set_power(bool enabled) = 0;

    /**
     * @brief Query whether the display supports touch input.
     *
     * @return true if touch capability is supported, false otherwise.
     */
    virtual bool supports_touch() const { return false; }

    /**
     * @brief Query whether the display supports backlight control.
     *
     * @return true if backlight control is supported, false otherwise.
     */
    virtual bool supports_backlight() const { return true; }
};
