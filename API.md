# Display Gfx — API Reference

Complete API documentation for the `display_gfx` component. This reference covers the interfaces, concrete classes, data types, and drawing methods.

---

## Table of Contents

- [Core Interfaces](#core-interfaces)
  - [`IHalDisplay`](#ihaldisplay)
  - [`IGraphicsContext`](#igraphicscontext)
- [Hardware Abstraction Layer (HAL)](#hardware-abstraction-layer-hal)
  - [`Ssd1306Config`](#ssd1306config)
  - [`HalDisplaySsd1306`](#haldisplayssd1306)
- [Graphics Context](#graphics-context)
  - [`FramebufferGraphicsContext`](#framebuffergraphicscontext)
- [Drawing API](#drawing-api)
  - [`set_pixel()`](#set_pixel)
  - [`draw_hline() / draw_vline()`](#draw_hline--draw_vline)
  - [`draw_rect()`](#draw_rect)
  - [`draw_char() / draw_string()`](#draw_char--draw_string)
  - [`clear()`](#clear)
  - [`flush()`](#flush)

---

## Core Interfaces

### `IHalDisplay`

Abstract interface representing the physical display hardware wrapper.

```cpp
class IHalDisplay {
public:
    virtual ~IHalDisplay() = default;
    virtual esp_err_t init() = 0;
    virtual esp_err_t flush_buffer(const uint8_t* buffer, size_t size) = 0;
    virtual esp_err_t set_brightness(uint8_t level) = 0;
    virtual esp_err_t set_power(bool enabled) = 0;
    virtual bool supports_touch() const = 0;
    virtual bool supports_backlight() const = 0;
};
```

---

### `IGraphicsContext`

Abstract interface defining the drawing API contract.

```cpp
class IGraphicsContext {
public:
    enum class Rotation {
        ROTATION_0,
        ROTATION_90,
        ROTATION_180,
        ROTATION_270
    };

    virtual ~IGraphicsContext() = default;
    virtual uint16_t get_width() const = 0;
    virtual uint16_t get_height() const = 0;
    virtual void set_pixel(int x, int y, uint32_t color) = 0;
    virtual uint32_t get_pixel(int x, int y) const = 0;
    virtual void draw_hline(int x, int y, int w, uint32_t color) = 0;
    virtual void draw_vline(int x, int y, int h, uint32_t color) = 0;
    virtual void clear(uint32_t color = 0) = 0;
    virtual esp_err_t flush() = 0;
    virtual void set_rotation(Rotation r) = 0;
    virtual Rotation get_rotation() const = 0;
    virtual void draw_char(int x, int y, char c, uint32_t color) = 0;
    virtual void draw_string(int x, int y, const char* str, uint32_t color) = 0;
};
```

---

## Hardware Abstraction Layer (HAL)

### `Ssd1306Config`

Configuration parameters for initializing the physical SSD1306 OLED display.

```cpp
struct Ssd1306Config {
    void* i2c_bus = nullptr;            ///< Pointer to i2c_master_bus_handle_t (new driver)
    uint8_t i2c_address = 0x3C;         ///< SSD1306 device I2C address (typically 0x3C or 0x3D)
    uint16_t width = 128;               ///< Display width in pixels
    uint16_t height = 64;               ///< Display height in pixels
    int rst_gpio = -1;                  ///< Reset GPIO pin (-1 if unused)
    uint32_t i2c_clk_speed_hz = 400000; ///< I2C clock speed in Hz (default 400kHz)
};
```

---

### `HalDisplaySsd1306`

ESP-IDF HAL wrapper for the SSD1306 OLED panel using the modern `esp_lcd` subsystem.

```cpp
class HalDisplaySsd1306 : public IHalDisplay {
public:
    explicit HalDisplaySsd1306(const Ssd1306Config& config);
    esp_err_t init() override;
    esp_err_t flush_buffer(const uint8_t* buffer, size_t size) override;
    esp_err_t set_brightness(uint8_t level) override;
    esp_err_t set_power(bool enabled) override;
    bool supports_touch() const override { return false; }
    bool supports_backlight() const override { return false; }
};
```

---

## Graphics Context

### `FramebufferGraphicsContext`

Concrete implementation of `IGraphicsContext` that stores pixels in a local RAM buffer and uses page-based arrangement mapping to SSD1306 memory.

```cpp
class FramebufferGraphicsContext : public IGraphicsContext {
public:
    explicit FramebufferGraphicsContext(IHalDisplay& display, uint16_t phys_width = 128, uint16_t phys_height = 64);
    // ... implements all IGraphicsContext draw calls
};
```

---

## Drawing API

### `set_pixel()`

Sets the color of a specific coordinate `(x, y)` in the framebuffer.

```cpp
void set_pixel(int x, int y, uint32_t color) override;
```
*   `color = 0`: Pixel off (Black)
*   `color = 1`: Pixel on (White)

### `draw_hline() / draw_vline()`

Draws horizontal or vertical lines from a starting point `(x, y)` with a given width/height.

```cpp
void draw_hline(int x, int y, int w, uint32_t color) override;
void draw_vline(int x, int y, int h, uint32_t color) override;
```

### `draw_rect()`

Draws a rectangle starting at `(x, y)` spanning width `w` and height `h`.

```cpp
void draw_rect(int x, int y, int w, int h, uint32_t color);
```

### `draw_char() / draw_string()`

Renders characters or null-terminated strings using the built-in 5x7 font.

```cpp
void draw_char(int x, int y, char c, uint32_t color) override;
void draw_string(int x, int y, const char* str, uint32_t color) override;
```

### `clear()`

Fills the entire framebuffer with a given color.

```cpp
void clear(uint32_t color = 0) override;
```

### `flush()`

Flushes the local RAM framebuffer to the physical OLED panel via the hardware HAL.

```cpp
esp_err_t flush() override;
```
