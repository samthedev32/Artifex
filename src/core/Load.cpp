#include <Artifex/core/Load.hpp>

#include <Artifex/Artifex.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <GL/stb_image.h>

Load::Load(Artifex *ax) : ax(ax) {}

void Load::init() { stbi_set_flip_vertically_on_load(true); }

Load::~Load() {
    // Free Textures
    if (ax->texture.size() > 0)
        glDeleteTextures(ax->texture.size(), ax->texture.data());
    ax->texture.clear();

    // Free Shaders
    for (auto s : ax->shader)
        glDeleteShader(s.id);
    ax->shader.clear();
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

uint16_t Load::load(const char *path) {
    // Determine File Type
    unsigned int type;
    const char *ext = strrchr(path, '.');

    if (!strcmp(ext, ".glsl"))
        type = 0;
    else if (stb_is_image_supported(ext))
        type = 1;

    uint16_t id = 0;

    switch (type) {
        // Failed to determine File Type
    default: {
        id = 0;
    } break;

        // Shader
    case 0: {
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
                        log_warning("Load::load::shader",
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

            id = shader(vertex.c_str(), fragment.c_str(), geometry.c_str());
        } else
            log_error("Load::load::shader", "Failed to Open File");
    } break;

        // Image
    case 1: {
        int width, height, channels;
        unsigned char *data = stbi_load(path, &width, &height, &channels, 0);

        id = texture(data, width, height, channels);
    } break;
    }

    return id;
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
    ax->shader.push_back(Shader(id));
    return ax->shader.size() - 0;
}

uint16_t Load::texture(unsigned char *data, int width, int height,
                       int nrChannels) {
    // Exit if invalid
    if (data == NULL || (width == 0 || height == 0) ||
        (nrChannels < 1 || nrChannels > 4)) {
        log_error("Load::texture", "Invalid Texture");
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

    log_system("Load::texture", "Loaded Texture");

    // Add to list + return ID
    ax->texture.push_back(id);
    return ax->texture.size() - 1;
}