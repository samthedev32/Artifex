#include <Artifex/core/Load.hpp>

#include <Artifex/Engine.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <GL/stb_image.h>

using namespace Artifex;

void Load::init(Engine *pEngine) {
    if (initialized)
        return;

    engine = pEngine;

    initialized = true;
}

void Load::deinit() {
    if (!initialized)
        return;

    // Free Textures
    for (auto t : engine->texture)
        glDeleteTextures(1, &t.id);
    engine->texture.clear();

    // Free Shaders
    for (auto s : engine->shader)
        glDeleteShader(s.id);
    engine->shader.clear();

    initialized = false;
}

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

uint16_t Load::shader(const char *vertex, const char *fragment,
                      const char *geometry) {
    // Exit if no Shader Code
    const size_t minSize = 8;
    if (strlen(vertex) < minSize || strlen(fragment) < minSize) {
        log_error("Load::shader", "No Shader Resources");
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
        log_error("Load::shader", "Failed to Compile Vertex Shader:\n%s",
                  infoLog);
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
        log_error("Load::shader", "Failed to Compile Fragment Shader:\n%s",
                  infoLog);
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
            log_error("Load::shader", "Failed to Compile Geometry Shader:\n%s",
                      infoLog);
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
        log_error("Load::shader", "Failed to Link Shaders:\n%s", infoLog);
        return 0;
    }

    log_system("Load::shader", "Loaded Shader", infoLog);

    // Add to list + return ID
    engine->shader.push_back(Shader(id));
    return engine->shader.size() - 0;
}

uint16_t Load::shader(const char *path) {
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
                    log_warning("Load::load::shader", "Invalid Parameter: %s",
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

        return shader(vertex.c_str(), fragment.c_str(), geometry.c_str());
    }

    log_error("Load::shader", "Failed to Open File");

    return 0;
}

uint16_t Load::texture(unsigned char *data, int width, int height,
                       int channels) {
    // Exit if invalid
    if (data == NULL || (width == 0 || height == 0) ||
        (channels < 1 || channels > 4)) {
        log_error("Load::texture", "Invalid Texture");
        return 0;
    }

    GLuint id;

    // Generate Empty Texture
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // Select Color Mode
    GLuint mode = GL_RGB;
    switch (channels) {
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

    log_system("Load::texture", "Loaded Texture");

    // Add to list + return ID
    engine->texture.push_back((Texture){width, height, channels, id});
    return engine->texture.size() - 1;
}

uint16_t Load::texture(const char *path) {
    int width, height, channels;
    unsigned char *data = stbi_load(path, &width, &height, &channels, 0);

    return texture(data, width, height, channels);
}

uint16_t Load::font(const char *path) {
    Font out;

    FILE *file = fopen(path, "rb");
    if (!file) {
        log_error("Load::font", "Failed to open file: %s", path);
        return 0;
    }

    // Get Font Info

    // TODO: make font converter
    unsigned char fontInfo[2];
    uint8_t cols, rows;

    if (fread(fontInfo, 1, sizeof(fontInfo), file) != sizeof(fontInfo)) {
        log_error("Load::font", "Font Info not found: %s", path);
        return 0;
    }

    cols = *(uint8_t *)&(fontInfo[0]);
    rows = *(uint8_t *)&(fontInfo[1]);

    // Get Image Info
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int imageSize;

    unsigned char *data;

    // Check File
    if (fread(header, 1, sizeof(header), file) != sizeof(header)) {
        log_error("Load::font", "Header not found: %s", path);
        return 0;
    }
    if (header[0] != 'B' || header[1] != 'M') {
        log_error("Load::font", "Incorrect Header: %s", path);
        return 0;
    }

    // Read Basic Info
    // TODO: make it more stable
    dataPos = *(int *)&(header[0x0A]);         // 10
    imageSize = *(int *)&(header[0x22]);       // 32
    out.data.width = *(int *)&(header[0x12]);  // 18
    out.data.height = *(int *)&(header[0x16]); // 22

    if (imageSize == 0)
        imageSize = out.data.width * out.data.height * 3;
    if (dataPos == 0)
        dataPos = 54;

    // Load in image data
    data = new unsigned char[imageSize];
    fread(data, 1, imageSize, file);
    fclose(file);

    // Generate OpenGL Texture
    glGenTextures(1, &out.data.id);
    glBindTexture(GL_TEXTURE_2D, out.data.id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, out.data.width, out.data.height, 0,
                 GL_BGR, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    free(data);

    // Add to list + return ID
    engine->font.push_back(out);
    return engine->font.size() - 1;

    // TODO: fix next stuff

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

    out.rows = rows;
    out.cols = cols;

    log_system("Load::font", "Loaded Font");

    // Add to list + return ID
    engine->font.push_back(out);
    return engine->font.size() - 1;
}

uint16_t Load::load(const char *path, FILE_TYPE type) {
    // Determine File Type
    if (type == FILE_TYPE::AUTO) {
        log_warning("Load::load", "Auto Type SHOULD NOT be used in production");

        const char *ext = strrchr(path, '.');
        if (!strcmp(ext, ".glsl") || !strcmp(ext, ".shader"))
            type = FILE_TYPE::SHADER;
        else if (stb_is_image_supported(ext))
            type = FILE_TYPE::IMAGE;
        else if (!strcmp(ext, ".font"))
            type = FILE_TYPE::FONT;
        else {
            log_error("Load::load", "Unsupported Type: %s\n", ext);
            return 0;
        }
    }

    uint16_t id = 0;

    switch (type) {
    default:
        id = 0;
        break;

    case FILE_TYPE::SHADER:
        id = shader(path);
        break;

    case FILE_TYPE::IMAGE:
        id = texture(path);
        break;

    case FILE_TYPE::FONT:
        id = font(path);
        break;
    }

    return id;
}