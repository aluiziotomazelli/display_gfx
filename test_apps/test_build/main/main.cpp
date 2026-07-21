#define LOG_LOCAL_LEVEL ESP_LOG_INFO
#include "esp_log.h"

#include "interfaces/i_hal_display.hpp"
#include "interfaces/i_graphics_context.hpp"
#include "framebuffer_graphics_context.hpp"

static const char* TAG = "test_build";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "display_gfx - Build validation app started");
}

