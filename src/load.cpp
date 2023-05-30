#include <GL/load.h>

#include <GL/glad.h>
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
        printf("%s: \n%s", err.c_str(), infoLog);
    }

    return success;
}

std::string split(std::string source, uint part) {
    std::string out;
    uint c = 0;
    for (int i = 0; i < source.size(); i++) {
        if (c < part) {
            if (source[i] == ' ' || source[i] == '\t' || source[i] == '\n')
                c++;
        } else {
            // TODO: Fix String Splitting
        }
    }
}

// Load Functions

Shader shader(std::string path) {
    Shader out;
    std::ifstream file(path);

    if (!file.is_open()) {
        printf("No Shader File\n");
        return out;
    }

    std::string vertex, geometry, fragment, compute, unknown;

    std::string line;
    uint shader_type = 0;
    while (getline(file, line)) {
        if (line == "#vertex") {
            shader_type = 1;
        } else if (line == "#geometry") {
            shader_type = 2;
        } else if (line == "#fragment") {
            shader_type = 3;
        } else if (line == "#compute") {
            shader_type = 4;
        } else {
            switch (shader_type) {
            case 1:
                // Vertex Shader
                vertex += line + "\n";
                break;

            case 2:
                // Geometry Shader
                geometry += line + "\n";
                break;

            case 3:
                // Fragment Shader
                fragment += line + "\n";
                break;

            case 4:
                // Compute Shader
                compute += line + "\n";
                break;

            default:
                // Unknown
                unknown += line + "\n";
                break;
            }
        }
    }

    // Get RAW Codes
    const char *vert = vertex.c_str();
    const char *geo = geometry.c_str();
    const char *frag = fragment.c_str();

    bool hasVertex, hasGeometry, hasFragment;

    // Compile & Link Shaders
    uint vs, gs, fs;

    // ---- Compile Vertex Shader
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vert, NULL);
    glCompileShader(vs);

    checkShader(vs, "Vertex Shader Compilation Failed");

    // ---- Compile Geometry Shader
    gs = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(gs, 1, &geo, NULL);
    glCompileShader(gs);

    checkShader(vs, "Geometry Shader Compilation Failed");

    // ---- Compile Fragment Shader
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &frag, NULL);
    glCompileShader(fs);

    checkShader(vs, "Fragment Shader Compilation Failed");

    // ---- Link Shader Programs
    out = glCreateProgram();
    glAttachShader(out, vs);
    glAttachShader(out, gs);
    glAttachShader(out, fs);

    glLinkProgram(out);

    checkShader(out, "Shader Linking Failed", GL_LINK_STATUS);

    // Delete unnecessary shaders
    glDeleteShader(vs);
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
    uint shader_type = 0;
    while (getline(file, line)) {
    }
}

Mesh mesh(std::string path) {
    // TODO
}

} // namespace GL::LOAD