# host_test/test_display_gfx/main/CMakeLists.txt

idf_component_register(
    SRCS 
        "main.cpp"
        "test_interfaces.cpp"
    
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