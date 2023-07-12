// The Resource-Loading Library for Artifex
#pragma once

#include <artifex/core/utility.hpp>

// Load In Resources
namespace load {
/* Hash-mapped already loaded resources */
std::map<std::string, texture> loaded_texture;
std::map<std::string, shader> loaded_shader;
std::map<std::string, audio> loaded_audio;
std::map<std::string, font> loaded_font;

// Hash-mapping resources
bool hashing = true;

/* Default Resource Paths */
std::string PATH_TEXTURE = "res/textures/";
std::string PATH_SHADER = "res/shaders/";
std::string PATH_AUDIO = "res/audio/";
std::string PATH_FONT = "res/fonts/";

/* Load Image */
texture image(std::string path) {
    texture out;

    if (hashing && loaded_texture.count(path) > 0)
        // Already Hashed
        out = loaded_texture[path];
    else {
        SDL_Surface *surface = IMG_Load((PATH_TEXTURE + path).c_str());
        if (surface != NULL) {
            SDL_FlipSurface(surface);

            glGenTextures(1, &out);
            glBindTexture(GL_TEXTURE_2D, out);

            int Mode = GL_RGB;
            if (surface->format->BytesPerPixel == 4)
                Mode = GL_RGBA;

            glTexImage2D(GL_TEXTURE_2D, 0, Mode, surface->w, surface->h, 0,
                         Mode, GL_UNSIGNED_BYTE, surface->pixels);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            SDL_FreeSurface(surface);

            loaded_texture[path] = out;
        } else {
            LOG::WARNING("Failed to load image!");
            return out;
        }
    }

    return out;
}

/* Create A Single-Colored RGB Texture (1 by 1) */
texture rgb(vec3 color) {
    texture out;

    // Generate name like: "RGB-255-140-0" for orange
    std::string name = "RGB-" + string::itos(color.r * 255) + "-" +
                       string::itos(color.g * 255) + "-" +
                       string::itos(color.b * 255);

    if (hashing && loaded_texture.count(name) > 0)
        // Already Hashed
        out = loaded_texture[name];
    else {
        unsigned char data[3];

        data[0] = (unsigned char)color.r * 255.0f;
        data[1] = (unsigned char)color.g * 255.0f;
        data[2] = (unsigned char)color.b * 255.0f;

        // Generate OpenGL Texture
        glGenTextures(1, &out);
        glBindTexture(GL_TEXTURE_2D, out);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        loaded_texture[name] = out;
    }

    return out;
}

audio sound(std::string path) {
    audio out;

    if (hashing && loaded_audio.count(path) > 0)
        out = loaded_audio[path];
    else {
        // Fix it, there's a bug inside
        out.source = Mix_LoadWAV((PATH_AUDIO + path).c_str());

        if (out.source == NULL)
            LOG::WARNING("Failed to load sound!");
        else
            loaded_audio[path] = out;
    }

    return out;
}

/* Load Shader (.VS / .FS) */
shader gls(std::string path, std::string vertex = "",
           std::string fragment = "") {
    shader out;

    // TODO: Geometry Shader Support

    if (path != "" && hashing && loaded_shader.count(path) > 0)
        // Already Hashed
        out = loaded_shader[path];
    else {
        std::string vs_raw, fs_raw;
        const char *vc, *fc;

        if ((path == "") && (vertex == "" || fragment == "")) {
            LOG::WARNING("load::gls(" + path + ")", "no shader sources");
            return out;
        }

        // Load From File
        if (path != "") {
            // Open Files
            std::ifstream vs_f(PATH_SHADER + path + ".vs");
            std::ifstream fs_f(PATH_SHADER + path + ".fs");

            if (!vs_f.is_open() || !fs_f.is_open()) {
                LOG::WARNING("load::gls(" + path + ")",
                             "can't open file(s): " + path);
                return out;
            }

            // Copy Files
            std::string line;
            while (getline(vs_f, line))
                vs_raw += line + "\n";
            while (getline(fs_f, line))
                fs_raw += line + "\n";

            vs_f.close();
            fs_f.close();

            vc = vs_raw.c_str();
            fc = fs_raw.c_str();
        } else {
            vc = vertex.c_str();
            fc = fragment.c_str();
        }

        // Compile & Load Shader
        uint vs, fs;

        int success;
        char infoLog[1024];

        // ---- Compile Vertex Shader
        vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vc, NULL);
        glCompileShader(vs);

        // Check if compilation was successfull
        glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vs, 1024, NULL, infoLog);

            LOG::WARNING("load::gls(" + path + ")",
                         "can't compile vertex shader: " +
                             std::string(infoLog));

            return out;
        }

        // ---- Compile Fragment Shader
        fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fc, NULL);
        glCompileShader(fs);

        // Check if compilation was successful
        glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fs, 1024, NULL, infoLog);

            LOG::WARNING("load::gls(" + path + ")",
                         "can't compile fragment shader: " +
                             std::string(infoLog));

            glDeleteShader(vs);
            return out;
        }

        // ---- Create Shader Program
        out.id = glCreateProgram();
        glAttachShader(out.id, vs);
        glAttachShader(out.id, fs);

        glLinkProgram(out.id);

        // Delete unnecessary shaders
        glDeleteShader(vs);
        glDeleteShader(fs);

        // Check if linking was successful
        glGetProgramiv(out.id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(out.id, 1024, NULL, infoLog);

            LOG::WARNING("load::gls(" + path + ")",
                         "can't link shaders: " + std::string(infoLog));
            return out;
        }

        loaded_shader[path] = out;
    }

    return out;
}

/* Load Font (.TTF) */
font ttf(std::string path, int width = 16, int height = 16, int start = 32) {
    font out;

    if (hashing && loaded_font.count(path) > 0)
        // Already Hashed
        out = loaded_font[path];
    else {
        // Load Data (texture)
        std::string old = PATH_TEXTURE;
        PATH_TEXTURE = PATH_FONT;
        out.data = image(path);
        PATH_TEXTURE = old;

        // Load OpenGL
        glGenVertexArrays(1, &out.VAO);
        glGenBuffers(1, &out.VBO);

        glBindVertexArray(out.VAO);

        glBindBuffer(GL_ARRAY_BUFFER, out.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(out.vertices), out.vertices,
                     GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                              (void *)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        out.width = width;
        out.height = height;
        out.start = start;

        loaded_font[path] = out;
    }

    return out;
}
} // namespace load