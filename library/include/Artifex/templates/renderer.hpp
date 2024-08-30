/**
 * @brief C++ Renderer Template for Artifex
 *
 * @author SamTheDev
 */

#pragma once

#include <Artifex/core/types/color.hpp>
#include <Artifex/core/types/vec.hpp>
#include <cstdint>

struct DrawInfo {
    vec<2> center, size;
    float rotation;

    vec<4> corner;

    vec<3> color;
    unsigned int texture;

    //        enum {
    //            DYNAMIC = 0,
    //            COLOR,
    //            TEXTURE
    //        } style;

    int style;
};

class axRenderer {
   public:
    // Initialize Renderer + Window
    axRenderer(const char* title, uint32_t width, uint32_t height);
    ~axRenderer();

    virtual void update();
    virtual void clear(color<ColorFormat::RGBA> color);

    // Load Resources
    virtual unsigned int load_shader(const char* vertex, const char* fragment, const char* geometry = nullptr);
    virtual unsigned int load_texture(int width, int height, int channels, const unsigned char* data);

    virtual unsigned int load_mesh(const std::vector<vec<2>>& vertices, const std::vector<unsigned int>& indices);

    // Unload Resources
    void unload_shader(unsigned int shader);
    void unload_texture(unsigned int texture);

    void unload_mesh(unsigned int);

    // Render
    void draw(const DrawInfo& drawInfo);
};