#include "mathutil/log.h"
#include <Artifex/Artifex.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <GL/stb_image.h>

#include <stdio.h>
#include <string.h>
#include <string>
#include <time.h>

// Pre-Definitions
extern const char *default_shader_code[2];

// Public

Artifex::Artifex(std::string name, uint width, uint height)
    : Window(name, width, height) {
    stbi_set_flip_vertically_on_load(true);

    // Load Default Rect
    float vertices[] = {
        // positions      // texture coords
        -1.0f, 1.0f,  0.0f, 1.0f, // top right
        -1.0f, -1.0f, 0.0f, 0.0f, // bottom right
        1.0f,  -1.0f, 1.0f, 0.0f, // bottom left
        1.0f,  1.0f,  1.0f, 1.0f  // top left
    };

    uint indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Config OpenGL
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);

    // Load 2D Shader
    load_shader(default_shader_code[0], default_shader_code[1]);

    // Load Texture
    unsigned char data[3] = {255, 0, 0};
    load_texture(data, 1, 1, 3);

    past = now = time();
}

Artifex::~Artifex() {
    // Free Textures
    if (texture.size() > 0)
        glDeleteTextures(texture.size(), texture.data());
    texture.clear();

    // Free Shaders
    for (auto s : shader)
        glDeleteShader(s.id);
    shader.clear();

    // Delete Buffers
    GLuint buffers[] = {VAO, VBO};
    glDeleteBuffers(2, buffers);
}

bool Artifex::update(float r, float g, float b) {
    // Update Screen
    bool running = Window::update();

    // Clear Screen
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Update GL
    glViewport(0, 0, width, height);

    // Timing
    past = now;
    now = time();
    deltaTime = now - past;

    return running;
}

float Artifex::time() {
    struct timespec res;
    clock_gettime(CLOCK_MONOTONIC, &res);
    return (1000.0f * res.tv_sec + (double)res.tv_nsec / 1e6) / 1000.0f;
}

// ---- Resource Loading

bool stb_is_image_supported(const char *ext) {
    static const char *supported[] = {
        ".png", ".jpg", ".jpeg", ".bmp", ".gif", // TODO: Add more if needed
    };

    if (ext != NULL)
        // Check if the extension is in the list of supported extensions
        for (size_t i = 0; i < sizeof(supported) / sizeof(supported[0]); ++i)
            if (!strcmp(ext, supported[i]))
                return true;

    return false;
}

uint16_t Artifex::load(const char *path, uint8_t type) {
    // Determine File Type
    if (type == AX_TYPE_AUTO) {
        const char *ext = strrchr(path, '.');

        if (!strcmp(ext, ".glsl"))
            type = AX_TYPE_SHADER;
        else if (stb_is_image_supported(ext))
            type = AX_TYPE_TEXTURE;
    }

    uint16_t id = 0;

    switch (type) {
        // Failed to determine File Type
    default: {
        id = 0;
    } break;

        // Shader
    case AX_TYPE_SHADER: {
        FILE *f = fopen(path, "r");

        if (f) {
            uint8_t current = 0;
            std::string vertex, fragment, geometry;

            char line[256];
            while (fgets(line, sizeof(line), f)) {
                char index[10], parameter[10];
                sscanf(line, "%9s %9s", index, parameter);

                if (!strcmp(index, "#shader")) {
                    if (!strcmp(parameter, "vertex"))
                        current = 1;
                    else if (!strcmp(parameter, "fragment"))
                        current = 2;
                    else if (!strcmp(parameter, "geometry"))
                        current = 3;
                    else
                        log_warning("Artifex::load::shader",
                                    "Invalid Parameter: %s", parameter);
                } else {
                    switch (current) {
                    default:
                        break;

                    case 1:
                        vertex += line;
                        break;

                    case 2:
                        fragment += line;
                        break;

                    case 3:
                        geometry += line;
                        break;
                    }
                }
            }

            fclose(f);

            id =
                load_shader(vertex.c_str(), fragment.c_str(), geometry.c_str());
        } else
            log_error("Artifex::load::shader", "Failed to Open File");
    } break;

        // Image
    case AX_TYPE_TEXTURE: {
        int width, height, channels;
        unsigned char *data = stbi_load(path, &width, &height, &channels, 0);

        id = load_texture(data, width, height, channels);
    } break;
    }

    return id;
}

uint16_t Artifex::load_shader(const char *vertex, const char *fragment,
                              const char *geometry) {
    // Exit if no Shader Code
    const size_t minSize = 8;
    if (strlen(vertex) < minSize || strlen(fragment) < minSize) {
        log_error("Artifex::load_shader", "No Shader Resources");
        return 0;
    }

    // Get Raw Codes
    const char *vert_s = vertex;
    const char *frag_s = fragment;
    const char *geo_s = NULL;

    if (strlen(geometry) >= minSize)
        geo_s = geometry;

    GLuint vert, frag, geo, id;
    bool isGeo = geo_s != NULL;

    int success;
    char infoLog[1024];

    // ---- Compile Vertex Shader
    vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vert_s, NULL);
    glCompileShader(vert);

    glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vert, 1024, NULL, infoLog);
        log_error("Artifex::load_shader",
                  "Failed to Compile Vertex Shader:\n%s", infoLog);
        return 0;
    }

    // ---- Compile Fragment Shader
    frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &frag_s, NULL);
    glCompileShader(frag);

    glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(frag, 1024, NULL, infoLog);
        glDeleteShader(vert);
        log_error("Artifex::load_shader",
                  "Failed to Compile Fragment Shader:\n%s", infoLog);
        return 0;
    }

    // ---- Compile Geometry Shader (if present)
    if (isGeo) {
        geo = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(geo, 1, &geo_s, NULL);
        glCompileShader(geo);

        glGetShaderiv(geo, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(geo, 1024, NULL, infoLog);
            log_error("Artifex::load_shader",
                      "Failed to Compile Geometry Shader:\n%s", infoLog);
            isGeo = false;
        }
    }

    // ---- Create Shader Program
    id = glCreateProgram();
    glAttachShader(id, vert);
    glAttachShader(id, frag);
    if (isGeo)
        glAttachShader(id, geo);

    glLinkProgram(id);

    // Delete unnecessary shaders
    glDeleteShader(vert);
    glDeleteShader(frag);
    if (isGeo)
        glDeleteShader(geo);

    // Check if linking was successful
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 1024, NULL, infoLog);
        log_error("Artifex::load_shader", "Failed to Link Shaders:\n%s",
                  infoLog);
        return 0;
    }

    log_system("Artifex::load_shader", "Loaded Shader", infoLog);

    // Add to list + return ID
    shader.push_back(Shader(id));
    return shader.size() - 0;
}

uint16_t Artifex::load_texture(unsigned char *data, int width, int height,
                               int nrChannels) {
    // Exit if invalid
    if (data == NULL || (width == 0 || height == 0) ||
        (nrChannels < 1 || nrChannels > 4)) {
        log_error("Artifex::load_texture", "Invalid Texture");
        return 0;
    }

    GLuint id;

    // Generate Empty Texture
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // Select Color Mode
    GLuint mode = GL_RGB;
    switch (nrChannels) {
    case 1:
        mode = GL_RED;
        break;

    case 2:
        mode = GL_RG;
        break;

    default:
    case 3:
        mode = GL_RGB;
        break;

    case 4:
        mode = GL_RGBA;
        break;
    }

    // Create Texture
    glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode,
                 GL_UNSIGNED_BYTE, data);

    // Generate MipMap & Set Parameters
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    log_system("Artifex::load_texture", "Loaded Texture");

    // Add to list + return ID
    texture.push_back(id);
    return texture.size() - 1;
}

// ---- Rendering

void Artifex::rect(vec2 center, vec2 size, vec3 color, float rotation) {
    shader[0].use();

    shader[0].set("type", 0);
    shader[0].set("color", color);

    shader[0].set("center", center);
    shader[0].set("size", vec2(size / 2.0f));
    shader[0].set("ratio", ratio());

    shader[0].set("rotation", rotation);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Artifex::rect(vec2 center, vec2 size, uint16_t tex, float rotation) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[tex]);

    shader[0].use();

    shader[0].set("type", 1);

    shader[0].set("center", center);
    shader[0].set("size", vec2(size / 2.0f));
    shader[0].set("ratio", ratio());

    shader[0].set("rotation", rotation);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

// Draw Circle (Colored)
void Artifex::circle(vec2 center, float diameter, vec3 color) {}

// Draw Circle (Textured)
void Artifex::circle(vec2 center, float diameter, uint16_t tex, vec2 offset) {}

// Private

float Artifex::ratio() { return (float)width / (float)height; }

// Definitions
const char *default_shader_code[] = {
    // Vertex Shader
    "#version 300 es\n"

    "layout(location = 0) in vec2 aPos;\n"
    "layout(location = 1) in vec2 aTexCoord;\n"

    "out vec2 TexCoord;\n"
    "out vec2 FragPos;\n"

    "uniform vec2 center;\n"
    "uniform vec2 size;\n"
    "uniform float rotation;\n"

    "uniform float ratio;\n"

    "void main() {\n"
    "float rads = radians(-rotation);\n"
    "vec2 point = vec2(size.x * aPos.x, size.y * aPos.y);\n"

    "float x = cos(rads) * (point.x) - sin(rads) * (point.y) + center.x;\n"
    "float y = sin(rads) * (point.x) + cos(rads) * (point.y) + center.y;\n"

    "y *= ratio;\n"

    "TexCoord = aTexCoord;\n"
    "FragPos = vec2(aPos.x, aPos.y);\n"
    "gl_Position = vec4(x, y, 0.0, 1.0);\n"
    "}",

    // Fragment Shader
    "#version 300 es\n"
    "precision mediump float;\n"
    "out vec4 FragColor;\n"

    "in vec2 TexCoord;\n"
    "in vec2 FragPos;\n"

    "uniform int isTextured;\n"
    "uniform int shape;\n"

    "uniform int type;\n"

    "uniform sampler2D tex;\n"
    "uniform vec3 color;\n"

    "uniform float cutradius;\n"

    "void main() {\n"
    "    switch (type) {\n"

    // Color
    "    default:\n"
    "    case 0:\n"
    "        FragColor = vec4(color.rgb, 1.0);\n"
    "        break;\n"

    // Texture
    "    case 1:\n"
    "        FragColor = texture(tex, TexCoord);\n"
    "        break;\n"

    // Text
    "    case 2:\n"
    "        FragColor = vec4(color.rgb, length(texture(tex, TexCoord).rgb));\n"
    "        break;\n"

    // Circle
    "    case 3:\n"
    "        vec2 val = FragPos;\n"

    "       float R = 1.0f;\n"
    "       float R2 = cutradius;\n"
    "       float dist = sqrt(dot(val, val));\n"

    "       if (dist >= R || dist <= R2)\n"
    "             discard;\n"

    "       float sm = smoothstep(R, R - 0.01, dist);\n"
    "       float sm2 = smoothstep(R2, R2 + 0.01, dist);\n"
    "       float alpha = sm * sm2;\n"

    "       FragColor = vec4(color.xyz, 1.0);\n"
    "       break;\n"
    "   }\n"
    "}"};