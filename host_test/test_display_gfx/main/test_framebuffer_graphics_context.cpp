// components/display_gfx/host_test/test_display_gfx/main/test_framebuffer_graphics_context.cpp

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "framebuffer_graphics_context.hpp"
#include "mock_hal_display.hpp"

using ::testing::Return;
using ::testing::_;

class FramebufferGraphicsContextTest : public ::testing::Test {
protected:
    MockHalDisplay mock_display;
};

TEST_F(FramebufferGraphicsContextTest, DefaultInitialization) {
    FramebufferGraphicsContext gfx(mock_display);
    EXPECT_EQ(gfx.get_width(), 128);
    EXPECT_EQ(gfx.get_height(), 64);
    EXPECT_EQ(gfx.get_buffer_size(), 1024);
    EXPECT_EQ(gfx.get_rotation(), Rotation::ROTATION_0);
}

TEST_F(FramebufferGraphicsContextTest, SetAndGetPixel) {
    FramebufferGraphicsContext gfx(mock_display, 128, 64);

    EXPECT_EQ(gfx.get_pixel(10, 20), 0);

    gfx.set_pixel(10, 20, 1);
    EXPECT_EQ(gfx.get_pixel(10, 20), 1);

    gfx.set_pixel(10, 20, 0);
    EXPECT_EQ(gfx.get_pixel(10, 20), 0);

    // Out of bounds tests
    gfx.set_pixel(-1, 0, 1);
    gfx.set_pixel(128, 0, 1);
    gfx.set_pixel(0, 64, 1);

    EXPECT_EQ(gfx.get_pixel(-1, 0), 0);
    EXPECT_EQ(gfx.get_pixel(128, 0), 0);
    EXPECT_EQ(gfx.get_pixel(0, 64), 0);
}

TEST_F(FramebufferGraphicsContextTest, DrawHLineAndVLine) {
    FramebufferGraphicsContext gfx(mock_display, 128, 64);

    gfx.draw_hline(10, 5, 10, 1);
    for (int x = 10; x < 20; ++x) {
        EXPECT_EQ(gfx.get_pixel(x, 5), 1);
    }
    EXPECT_EQ(gfx.get_pixel(9, 5), 0);
    EXPECT_EQ(gfx.get_pixel(20, 5), 0);

    gfx.draw_vline(15, 10, 5, 1);
    for (int y = 10; y < 15; ++y) {
        EXPECT_EQ(gfx.get_pixel(15, y), 1);
    }
    EXPECT_EQ(gfx.get_pixel(15, 9), 0);
    EXPECT_EQ(gfx.get_pixel(15, 15), 0);
}

TEST_F(FramebufferGraphicsContextTest, DrawRectAndFillRect) {
    FramebufferGraphicsContext gfx(mock_display, 128, 64);

    gfx.draw_rect(5, 5, 10, 10, 1);
    EXPECT_EQ(gfx.get_pixel(5, 5), 1);
    EXPECT_EQ(gfx.get_pixel(14, 5), 1);
    EXPECT_EQ(gfx.get_pixel(5, 14), 1);
    EXPECT_EQ(gfx.get_pixel(14, 14), 1);
    EXPECT_EQ(gfx.get_pixel(7, 7), 0); // Interior is empty

    gfx.clear(0);
    gfx.fill_rect(5, 5, 10, 10, 1);
    EXPECT_EQ(gfx.get_pixel(7, 7), 1); // Interior is filled
}

TEST_F(FramebufferGraphicsContextTest, ClearScreen) {
    FramebufferGraphicsContext gfx(mock_display, 128, 64);

    gfx.clear(1);
    EXPECT_EQ(gfx.get_pixel(0, 0), 1);
    EXPECT_EQ(gfx.get_pixel(64, 32), 1);
    EXPECT_EQ(gfx.get_pixel(127, 63), 1);

    gfx.clear(0);
    EXPECT_EQ(gfx.get_pixel(0, 0), 0);
    EXPECT_EQ(gfx.get_pixel(64, 32), 0);
    EXPECT_EQ(gfx.get_pixel(127, 63), 0);
}

TEST_F(FramebufferGraphicsContextTest, FlushDelegatesToHalDisplay) {
    FramebufferGraphicsContext gfx(mock_display, 128, 64);

    EXPECT_CALL(mock_display, flush_buffer(gfx.get_buffer(), gfx.get_buffer_size()))
        .WillOnce(Return(ESP_OK));

    EXPECT_EQ(gfx.flush(), ESP_OK);
}

TEST_F(FramebufferGraphicsContextTest, RotationDimensionsAndPixelMapping) {
    FramebufferGraphicsContext gfx(mock_display, 128, 64);

    gfx.set_rotation(Rotation::ROTATION_90);
    EXPECT_EQ(gfx.get_width(), 64);
    EXPECT_EQ(gfx.get_height(), 128);
    EXPECT_EQ(gfx.get_rotation(), Rotation::ROTATION_90);

    // Set logical pixel (0, 0) under 90 deg rotation
    gfx.set_pixel(0, 0, 1);
    EXPECT_EQ(gfx.get_pixel(0, 0), 1);

    gfx.set_rotation(Rotation::ROTATION_180);
    EXPECT_EQ(gfx.get_width(), 128);
    EXPECT_EQ(gfx.get_height(), 64);

    gfx.set_rotation(Rotation::ROTATION_270);
    EXPECT_EQ(gfx.get_width(), 64);
    EXPECT_EQ(gfx.get_height(), 128);
}

TEST_F(FramebufferGraphicsContextTest, DrawCharAndString) {
    FramebufferGraphicsContext gfx(mock_display, 128, 64);

    gfx.draw_char(0, 0, 'A', 1);
    // 'A' has pixels set in column 0..4
    bool has_pixels = false;
    for (int y = 0; y < 7; ++y) {
        for (int x = 0; x < 5; ++x) {
            if (gfx.get_pixel(x, y) == 1) {
                has_pixels = true;
                break;
            }
        }
    }
    EXPECT_TRUE(has_pixels);

    gfx.clear(0);
    gfx.draw_string(0, 0, "SMART FARM", 1);
    // Check that characters advance and draw pixels
    EXPECT_TRUE(gfx.get_pixel(0, 0) != 0 || gfx.get_pixel(6, 0) != 0 || gfx.get_pixel(12, 0) != 0);
}
