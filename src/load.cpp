#include <GL/load.h>

#include <GL/glad.h>
#include <cstring>
#include <fstream>
#include <random>

namespace GL::LOAD {

// Utility

bool checkShader(Shader s, std::string err, uint type = GL_COMPILE_STATUS) {
    int success;
    char infoLog[1024];

    glGetShaderiv(s, type, &success);
    if (!success) {
        glGetShaderInfoLog(s, 1024, NULL, infoLog);
        printf("%s: \n%s\n", err.c_str(), infoLog);
    }

    return success;
}

// Load Functions

Shader shader(std::string path) {
    Shader out;
    std::ifstream file(path);

    if (!file.is_open()) {
        printf("No Shader File\n");
        return out;
    }

    std::string vertex, geometry, fragment;

    std::string line;
    uint shader_type = 0;
    while (getline(file, line)) {
        char buf[10];
        sscanf(line.c_str(), "%9s", buf);

        if (!strcmp(buf, "#vertex"))
            shader_type = 1;
        else if (!strcmp(buf, "#geometry"))
            shader_type = 2;
        else if (!strcmp(buf, "#fragment"))
            shader_type = 3;
        else
            switch (shader_type) {
            case 1:
                vertex += line + "\n";
                break;

            case 2:
                geometry += line + "\n";
                break;

            case 3:
                fragment += line + "\n";
                break;

            default:
                break;
            }
    }

    // Get RAW Codes
    const char *vert = vertex.c_str();
    const char *geo = geometry.c_str();
    const char *frag = fragment.c_str();

    const uint minSize = 16;

    bool hasVertex = vertex.size() > minSize;
    bool hasGeometry = geometry.size() > minSize;
    bool hasFragment = fragment.size() > minSize;

    // Compile & Link Shaders
    uint vs, gs, fs;

    // ---- Compile Vertex Shader
    if (hasVertex) {
        vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vert, NULL);
        glCompileShader(vs);

        hasVertex = checkShader(vs, "Vertex Shader Compilation Failed");
    }

    // ---- Compile Geometry Shader
    if (hasGeometry) {
        gs = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gs, 1, &geo, NULL);
        glCompileShader(gs);

        hasGeometry = checkShader(gs, "Geometry Shader Compilation Failed");
    }

    // ---- Compile Fragment Shader
    if (hasFragment) {
        fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &frag, NULL);
        glCompileShader(fs);

        hasFragment = checkShader(fs, "Fragment Shader Compilation Failed");
    }

    if (!hasVertex || !hasFragment) {
        printf("Not enough Shaders\n");
        return out;
    }

    // ---- Link Shader Programs
    out = glCreateProgram();
    glAttachShader(out, vs);
    if (hasGeometry)
        glAttachShader(out, gs);
    glAttachShader(out, fs);

    glLinkProgram(out);

    checkShader(out, "Shader Linking Failed", GL_LINK_STATUS);

    // Delete unnecessary shaders
    glDeleteShader(vs);
    if (hasGeometry)
        glDeleteShader(gs);
    glDeleteShader(fs);

    return out;
}

Texture texture(std::string path) {
    int width, height, nrChannels;
    unsigned char *data =
        stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        printf("No Image Data\n");
        return 0;
    }

    Texture id;

    // Generate Empty Texture
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // Select Color Mode
    uint mode;
    switch (nrChannels) {
    default:
    case 0:
    case 3:
        mode = GL_RGB;
        break;

    case 1:
        mode = GL_RED;
        break;

    case 2:
        mode = GL_RG;
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

    // Free RAW Data
    stbi_image_free(data);

    return id;
}

Material material(std::string path) {
    Material out;
    std::ifstream file(path);

    if (!file.is_open()) {
        printf("No Material File\n");
        return out;
    }

    std::string line;
    while (getline(file, line)) {
        char op[32];

        sscanf(line.c_str(), "%s", op);

        switch (op[0]) {}
    }

    return out;
}

Mesh mesh(std::string path) {
    Mesh out;
    std::ifstream file(path);

    if (!file.is_open()) {
        printf("No Mesh File\n");
        return out;
    }

    std::string line;
    while (getline(file, line)) {
        char c[9];
        sscanf(line.c_str(), "%8s", (char *)&c);

        std::string index(c);

        switch (c[0]) {
        default:
        case '#':
            // Comment
            break;

        case 'v': {
            int x, y, z, w;
        } break;
        }
    }

    return out;
}

} // namespace GL::LOAD