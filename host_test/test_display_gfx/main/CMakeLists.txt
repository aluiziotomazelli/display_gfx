# host_test/test_display_gfx/main/CMakeLists.txt

idf_component_register(
    SRCS 
        "main.cpp"
        "test_interfaces.cpp"
        "test_framebuffer_graphics_context.cpp"
        "test_hal_display_ssd1306.cpp"
    
    INCLUDE_DIRS
        "."
        "../../common"
        
    REQUIRES 
        display_gfx
        gtest
        WHOLE_ARCHIVE
)

if(IDF_TARGET STREQUAL "linux")
    target_compile_options(${COMPONENT_LIB} PUBLIC --coverage)
    target_link_options(${COMPONENT_LIB} PUBLIC --coverage)
endif()