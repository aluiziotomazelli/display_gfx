# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).


## [0.1.0] - 2026-07-21

### Added
- Initial release of the `display_gfx` component.
- Dependency-injected monochrome graphics context (`FramebufferGraphicsContext`) supporting primitive shapes (lines, rectangles) and text rendering.
- Modern ESP-IDF `esp_lcd` hardware abstraction layer (`HalDisplaySsd1306`) utilizing `driver/i2c_master.h` driver.
- Configurable I2C clock speed (`i2c_clk_speed_hz`) to support various SSD1306 panels.
- Font engine featuring a 5x7 monospaced pixel font (`font5x7.hpp`).
- Host-based unit tests with Google Test (`host_test/test_display_gfx`).
- `basic_usage` example demonstrating setup with `driver/i2c_master.h` and drawing API.

### Fixed
- Fixed static noise rendering issue on SSD1306 panels by correcting the control phase `dc_bit_offset` configuration to `6`.
- Resolved `E (275) i2c: CONFLICT!` errors on boot by entirely removing the legacy `driver/i2c.h` driver reference and fallback paths.

[0.1.0]: https://github.com/aluiziotomazelli/display_gfx/releases/tag/v0.1.0
