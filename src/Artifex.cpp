#include <Artifex/Artifex.h>
#include <GL/stb_image.h>

#include <stdio.h>
#include <string.h>
#include <string>
#include <time.h>

// Public

Artifex::Artifex(std::string name, uint width, uint height)
    : Window(name, width, height) {}

Artifex::~Artifex() {
    // Free Textures
    if (texture.size() > 0)
        glDeleteTextures(texture.size(), texture.data());
    texture.clear();

    // Free Shaders
    for (uint id : shader)
        glDeleteShader(id);
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
                        fprintf(stderr, "Invalid Shader Parameter: %s\n",
                                parameter);
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
            fprintf(stderr, "Failed to open Shader File: %s\n", path);
    } break;

        // Image
    case AX_TYPE_TEXTURE: {
        int width, height, channels;
        unsigned char *data = stbi_load(path, &width, &height, &channels, 0);

        if (data)
            id = load_texture(data, width, height, channels);
        else
            fprintf(stderr, "Failed to load Image: %s\n", path);
    } break;
    }

    return id;
}

uint16_t Artifex::load_shader(const char *vertex, const char *fragment,
                              const char *geometry) {
    // Exit if no Shader Code
    const size_t minSize = 8;
    if (strlen(vertex) < minSize || strlen(fragment) < minSize) {
        fprintf(stderr, "No Shader Resource(s)\n");
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
        fprintf(stderr, "Failed to compile Vertex Shader:\n%s", infoLog);
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
        fprintf(stderr, "Failed to compile Fragment Shader:\n%s", infoLog);
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
            fprintf(stderr, "Failed to compile Geometry Shader:\n%s", infoLog);
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
        fprintf(stderr, "Failed to link Shaders:\n%s", infoLog);
        return 0;
    }

    fprintf(stdout, "Loaded Shader\n");

    // Add to list + return ID
    shader.push_back(id);
    return shader.size();
}

uint16_t Artifex::load_texture(unsigned char *data, int width, int height,
                               int nrChannels) {
    // Exit if invalid
    if (data == NULL || (width == 0 || height == 0) ||
        (nrChannels < 1 || nrChannels > 4)) {
        fprintf(stderr, "Invalid Texture\n");
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

    fprintf(stdout, "Loaded Texture\n");

    // Add to list + return ID
    texture.push_back(id);
    return texture.size();
}

// ---- Rendering

// TODO

// Private

float Artifex::ratio() { return (float)width / (float)height; }