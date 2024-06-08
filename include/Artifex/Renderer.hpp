#pragma once

#include <Artifex/Window.hpp>
#include <Artifex/AssetManager.hpp>

namespace Artifex {
    // Visual Properties
    // (for default rendering)
    struct Visual {
        // Rotation of the object
        float rotation = 0;

        // Amount to round the 4 corners (0: none, 1: max)
        // Top-Left, Top-Right, Bottom-Left, Bottom-Right
        vec<4> corner = {};

        // TODO: other config, like:
        // dynamic, color & textured tint
        // Shadowed corners
        // ...
    };

    class Renderer {
    public:
        Renderer(Window &window, AssetManager &asset); // NOLINT(*-explicit-constructor)
        ~Renderer();

        // Clear Screen
        void clear(vec<4> color = {});

        // Update Renderer
        void update(const vec<2, int> &size, float ratio);

        // TODO bon rendering api

        // ---- Custom Rendering
        // TODO

        // ---- Default Rendering (using default shader & mesh

        // Render Image
        void image(vec<2> center, vec<2> size, UUID tex, const Visual &v = {}) const;

        // Render Color
        void color(const vec<2> &center, const vec<2> &size, const vec<3> &color, const Visual &v = {}) const;

        // TODO: render target support

        // Check for OpenGL Errors
        int checkErrors();

    private:
        Window &m_window;
        AssetManager &m_asset;

        // Default Resources (for UI)
        struct {
            UUID shader; // Default 2D Shader
            UUID mesh; // Default Rectangle Mesh

            // Shader Uniform Locations
            struct {
                int v_center, v_size, v_ratio;
                int v_rotation;
                int f_look, f_corner, f_time, f_color, f_tex;
            } uni;
        } m_default{};

        vec<2, int> m_size{};
        float m_ratio{};

        // TODO shader uniform functions
    };
}
