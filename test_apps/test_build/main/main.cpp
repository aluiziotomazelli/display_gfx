#define LOG_LOCAL_LEVEL ESP_LOG_INFO
#include "esp_log.h"

#include "interfaces/i_hal_display.hpp"
#include "interfaces/i_graphics_context.hpp"
#include "framebuffer_graphics_context.hpp"
#include "hal_display_ssd1306.hpp"

static const char* TAG = "test_build";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "display_gfx - Build validation app started");

    Ssd1306Config config;
    HalDisplaySsd1306 display(config);
    FramebufferGraphicsContext gfx(display);

    gfx.clear(0);
    gfx.draw_string(0, 0, "SMART FARM HUB", 1);
    ESP_LOGI(TAG, "Graphics context initialized cleanly with width %u, height %u", gfx.get_width(), gfx.get_height());
}

