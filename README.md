# Display Gfx

[![ESP-IDF Build](https://github.com/aluiziotomazelli/display_gfx/actions/workflows/build.yml/badge.svg)](https://github.com/aluiziotomazelli/display_gfx/actions/workflows/build.yml)
[![Host Tests](https://github.com/aluiziotomazelli/display_gfx/actions/workflows/host_test.yml/badge.svg)](https://github.com/aluiziotomazelli/display_gfx/actions/workflows/host_test.yml)
[![Coverage](https://img.shields.io/badge/coverage-report-blue)](https://aluiziotomazelli.github.io/display_gfx/index.html)

A lightweight, dependency-injected monochrome graphics library for SSD1306 OLED displays on ESP-IDF. Decoupled from Espressif peripherals via interfaces, this component is 100% testable on host Linux environments.

## Overview

`display_gfx` provides a decoupled graphics engine structure:
*   **Domain-Oriented HAL**: Hardware operations are abstracted in `IHalDisplay` (with the default `HalDisplaySsd1306` implementation).
*   **Monochrome Framebuffer**: `FramebufferGraphicsContext` acts as a RAM canvas, mapping coordinates to SSD1306's page layout.
*   **Built-in Font**: Includes a standard 5x7 monospaced pixel font for text rendering.

## Architecture

```
┌─────────────────────────────────────────────────────────┐
│              FramebufferGraphicsContext                 │
│                 (IGraphicsContext)                      │
└─────────────────────────────────────────────────────────┘
                            │
                            ▼
┌─────────────────────────────────────────────────────────┐
│                 HalDisplaySsd1306                       │
│                  (IHalDisplay)                          │
└─────────────────────────────────────────────────────────┘
                            │
                            ▼
               ┌────────────────────────┐
               │    esp_lcd driver      │
               │   (i2c_master.h API)   │
               └────────────────────────┘
```

## Key Features

- **No Legacy I2C Dependencies**: Exclusively compiled with ESP-IDF v5.x's modern `driver/i2c_master.h` driver, preventing resource conflicts with other components.
- **Hardware Abstraction Layer**: Easy mocking for automated tests without real hardware dependencies.
- **Low Memory Overhead**: Local frame buffer packed as 1-bit per pixel mapping.
- **Text & Primitive Shapes**: Rectangles, vertical/horizontal lines, single characters, and string drawing out of the box.

## Requirements

- **Framework**: ESP-IDF v5.1+
- **Language**: C++20 / C++17
- **Target Hardware**: SSD1306 128x64 or 128x32 OLED panel
- **Host Testing**: Google Test and Google Mock

## Quick Start Guide

### 1. Installation

Add the component to your ESP-IDF project's components directory:
```bash
git submodule add https://github.com/aluiziotomazelli/display_gfx components/display_gfx
```

### 2. Initialization and Usage

Initialize the modern I2C master driver and pass the bus handle to the SSD1306 HAL configuration:

```cpp
#include "driver/i2c_master.h"
#include "hal_display_ssd1306.hpp"
#include "framebuffer_graphics_context.hpp"

extern "C" void app_main(void)
{
    // 1. Initialize the modern I2C master driver
    i2c_master_bus_handle_t i2c_bus = nullptr;
    i2c_master_bus_config_t bus_config = {};
    bus_config.i2c_port = I2C_NUM_0;
    bus_config.sda_io_num = GPIO_NUM_21;
    bus_config.scl_io_num = GPIO_NUM_22;
    bus_config.clk_source = I2C_CLK_SRC_DEFAULT;
    bus_config.glitch_ignore_cnt = 7;
    bus_config.flags.enable_internal_pullup = true;
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &i2c_bus));

    // 2. Configure SSD1306 Display
    Ssd1306Config display_config;
    display_config.i2c_bus = i2c_bus;
    display_config.i2c_address = 0x3C;
    display_config.width = 128;
    display_config.height = 64;
    display_config.rst_gpio = -1;

    HalDisplaySsd1306 display(display_config);
    ESP_ERROR_CHECK(display.init());

    // 3. Initialize Graphics Context
    FramebufferGraphicsContext gfx(display);
    gfx.clear(0);

    // 4. Draw Layout (Note: layout coordinates can map naturally)
    gfx.draw_rect(0, 0, 128, 64, 1);
    gfx.draw_string(14, 4, "SMART FARM HUB", 1);
    gfx.draw_hline(4, 14, 120, 1);

    // 5. Render
    ESP_ERROR_CHECK(gfx.flush());
}
```

## Project Structure

```
display_gfx/
├── include/
│   ├── interfaces/              # Pure abstract interfaces
│   │   ├── i_hal_display.hpp
│   │   └── i_graphics_context.hpp
│   ├── fonts/                   # Built-in monospaced fonts
│   │   └── font5x7.hpp
│   ├── hal_display_ssd1306.hpp  # SSD1306 hardware HAL
│   └── framebuffer_graphics_context.hpp # Core drawing implementation
├── src/
│   ├── hal_display_ssd1306.cpp
│   └── framebuffer_graphics_context.cpp
├── host_test/                   # GTest project running on Linux
│   └── test_display_gfx/
├── examples/
│   └── basic_usage/             # Standalone target integration test app
├── API.md                       # Comprehensive API Reference
├── CHANGELOG.md                 # Project version release logs
└── idf_component.yml            # ESP-IDF component registry manifest
```

## Testing

### Host-Based Unit Tests

Build and run tests on Linux:
```bash
cd host_test/test_display_gfx
. ~/dev/esp/esp-idf/export.sh
idf.py build
./build/test_display_gfx.elf
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
