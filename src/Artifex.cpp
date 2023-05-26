#include <Artifex/Artifex.h>
#include <string>

// Public

Artifex::Artifex(std::string name, uint width, uint height)
    : Window(name, width, height) {}

Artifex::~Artifex() {
    // Free Textures
    if (textures.size() > 0)
        glDeleteTextures(textures.size(), textures.data());
    textures.clear();

    // Free Shaders
    for (uint id : shaders)
        glDeleteShader(id);
    shaders.clear();
}

bool Artifex::update() { return Window::update(); }

void Artifex::clear(float red, float green, float blue, GLbitfield buffers) {
    glClearColor(red, green, blue, 1.0f);
    glClear(buffers);
}

uint Artifex::shader(std::string vertex, std::string fragment,
                     std::string geometry) {
    // Exit if no Shader Code
    if (fragment == "" || vertex == "") {
        printf("ERROR: No Shader Resource (%s)\n",
               fragment == "" && vertex == ""
                   ? "neither"
                   : (vertex == "" ? "vertex" : "fragment"));

        return 0;
    }

    // Get Raw Codes
    const char *vert_s = vertex.c_str();
    const char *frag_s = fragment.c_str();
    const char *geo_s = geometry == "" ? nullptr : geometry.c_str();

    uint vert, frag, geo, id;
    bool isGeo = geo_s != nullptr;

    int success;
    char infoLog[1024];

    // ---- Compile Vertex Shader
    vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vert_s, NULL);
    glCompileShader(vert);

    glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vert, 1024, NULL, infoLog);
        printf("ERROR: Failed to compile Vertex Shader:\n%s", infoLog);
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
        printf("ERROR: Failed to compile Fragment Shader:\n%s", infoLog);
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
            printf("ERROR: Failed to compile Geometry Shader:\n%s", infoLog);
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
        printf("ERROR: Failed to link Shaders:\n%s", infoLog);
        return 0;
    }

    // Add to list + return ID
    shaders.push_back(id);
    return shaders.size();
}

uint Artifex::texture(unsigned char *data, uint width, uint height,
                      uint nrChannels) {
    // Exit if invalid
    if (data == nullptr || width == 0 || height == 0 || nrChannels == 0 ||
        nrChannels > 4) {
        printf("ERROR: Invalid Texture\n");
        return 0;
    }

    uint id;

    // Generate Empty Texture
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // Select Color Mode
    uint mode = GL_RGB;
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

    // Add to list + return ID
    textures.push_back(id);
    return textures.size();
}

// Private

float Artifex::ratio() { return (float)width / (float)height; }