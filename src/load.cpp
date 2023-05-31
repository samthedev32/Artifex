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

uint count(char *s, char d) {
    uint c = 0;
    while (*s) {
        if (*s == d)
            c++;
        s++;
    }
    return c;
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
        char op[8];

        sscanf(line.c_str(), "%7s", op);

        if (!strcmp(op, "newmtl")) {
            // New Material
        } else if (!strcmp(op, "Ka")) {
            // Ambient Color
        } else if (!strcmp(op, "Kd")) {
            // Diffuse Color
        } else if (!strcmp(op, "Ks")) {
            // Specular Color
        } else if (!strcmp(op, "Ns")) {
            // Specular Exponent
        } else if (!strcmp(op, "Ni")) {
            // Optical Density
        } else if (!strcmp(op, "d")) {
            // Density - Transparencity
        } else if (!strcmp(op, "Tr")) {
            // Transparencity
        } else if (!strcmp(op, "Tf")) {
            // Transmission Filter
        } else if (!strcmp(op, "illum")) {
            // Illumination Mode
        }
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
        char index[8];
        char data[129];

        sscanf(line.c_str(), "%7s %128s", index, data);

        if (!strcmp(index, "v")) {
            // Vertex (x, y, z, [w])
            float x, y, z, w = 1.0f;
            sscanf(data, "%f %f %f %f", &x, &y, &z, &w);
        } else if (!strcmp(index, "vt")) {
            // Vertex Texture (u, v, [w])
            float u, v = 0.0f, w = 0.0f;
            sscanf(data, "%f %f %f", &u, &v, &w);
        } else if (!strcmp(index, "vn")) {
            // Vertex Normal (x, y, z)
            float x, y, z;
            sscanf(data, "%f %f %f", &x, &y, &z);
        } else if (!strcmp(index, "vp")) {
            // Parameter Space Vertices (u, [v, w])
            float u, v = 0.0f, w = 0.0f;
            sscanf(data, "%f %f %f", &u, &v, &w);
        } else if (!strcmp(index, "f")) {
            // Face (v[/vt/vn] v[/vt/vn] v[/vt/vn] ...)
            float v[3], t[3], n[3];
            if (sscanf(data, "%f/%f/%f %f/%f/%f %f/%f/%f", &v[0], &t[0], &n[0],
                       &v[1], &t[1], &n[1], &v[2], &t[2], &n[2]) != 9)
                if (sscanf(data, "%f//%f %f//%f %f//%f", &v[0], &n[0], &v[1],
                           &n[1], &v[2], &n[2]) != 6)
                    if (sscanf(data, "%f/%f %f/%f %f/%f", &v[0], &t[0], &v[1],
                               &t[1], &v[2], &t[2]) != 6)
                        if (sscanf(data, "%f %f %f", &v[0], &v[1], &v[2]) != 3)
                            printf("Failed to read Face Line\n");

        } else if (!strcmp(index, "l")) {
            // Line Element (v1 v2 v3 ...)
            // TODO
        }
    }

    return out;
}

} // namespace GL::LOAD