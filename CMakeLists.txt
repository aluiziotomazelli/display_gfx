set(COMPONENT_NAME display_gfx)

if(IDF_TARGET STREQUAL "linux")
    idf_component_register(
        INCLUDE_DIRS 
            "include"
            "include/interfaces"
    )
else()
    idf_component_register(
        INCLUDE_DIRS 
            "include"
            "include/interfaces"
        REQUIRES
            esp_lcd
            driver
    )
endif()

if(IDF_TARGET STREQUAL "linux")
    target_compile_options(${COMPONENT_LIB} INTERFACE --coverage)
    target_link_options(${COMPONENT_LIB} INTERFACE --coverage)
endif()
