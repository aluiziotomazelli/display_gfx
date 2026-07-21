set(COMPONENT_NAME display_gfx)

if(IDF_TARGET STREQUAL "linux")
    idf_component_register(
        SRCS
            "src/framebuffer_graphics_context.cpp"
            "src/hal_display_ssd1306.cpp"
        INCLUDE_DIRS 
            "include"
            "include/interfaces"
            "include/fonts"
    )
else()
    idf_component_register(
        SRCS
            "src/framebuffer_graphics_context.cpp"
            "src/hal_display_ssd1306.cpp"
        INCLUDE_DIRS 
            "include"
            "include/interfaces"
            "include/fonts"
        REQUIRES
            esp_lcd
            driver
    )
endif()

if(IDF_TARGET STREQUAL "linux")
    target_compile_options(${COMPONENT_LIB} PUBLIC --coverage)
    target_link_options(${COMPONENT_LIB} PUBLIC --coverage)
endif()
