#pragma once

#include <GL/stb_image.h>

#include <fstream>
#include <string>

// TODO: Multithread? & Optimize Loading

typedef unsigned int Shader;
typedef unsigned int Texture;
struct Material {};
struct Mesh {};

namespace GL::LOAD {

// Load Shader
Shader shader(std::string path);

// Load Texture
Texture texture(std::string path);

// Load Material
Material material(std::string path);

// Load Mesh
Mesh mesh(std::string path);

} // namespace GL::LOAD

// Load In Resources
namespace load {

/* Load Shader (.VS / .FS) */
Shader shader(std::string path, std::string vertex = "",
              std::string fragment = "") {
    Shader out;

    // TODO: Geometry Shader Support

    if (path != "" && hashing && loaded_shader.count(path) > 0)
        // Already Hashed
        out = loaded_shader[path];
    else {
        std::string vs_raw, fs_raw;
        const char *vc, *fc;

        if ((path == "") && (vertex == "" || fragment == "")) {
            LOG::WARNING("No Shader Sources: " + path);
            return out;
        }

        // Load From File
        if (path != "") {
            // Open Files
            std::ifstream vs_f(PATH_SHADER + path + ".vs");
            std::ifstream fs_f(PATH_SHADER + path + ".fs");

            if (!vs_f.is_open() || !fs_f.is_open()) {
                LOG::WARNING("Can't Open Shader File(s): " + path);
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

            LOG::WARNING("Can't Compile Vertex Shader: " + path + "\n" +
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

            LOG::WARNING("Can't Compile Fragment Shader: " + path + "\n" +
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

            LOG::WARNING("Can't Link Shaders: " + path + "\n" +
                         std::string(infoLog));
            return out;
        }

        loaded_shader[path] = out;
        LOG::DEBUG("Loaded Shader: " + path);
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
        glGenBuffers(1, &out.EBO);

        glBindVertexArray(out.VAO);

        glBindBuffer(GL_ARRAY_BUFFER, out.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(out.vertices), out.vertices,
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, out.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(out.indices), out.indices,
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
        LOG::DEBUG("Loaded Font: " + path);
    }

    return out;
}

// Load Material Library (.mtl)
std::map<std::string, Material> material(std::string path) {
    std::map<std::string, Material> out;

    if (hashing && loaded_material.count(path) > 0)
        // Already Hashed
        out = loaded_material[path];
    else {
        // Open File
        std::ifstream f(PATH_MATERIAL + path);
        if (f.is_open()) {
            // Load From File
            std::string unrecognized = "";
            std::string mtl = "";
            std::string line;
            while (getline(f, line)) {
                line = string::multispace(line);
                std::string index = string::split(line, " ", 0);

                if (line[0] == '#' || index == "" || index == " " ||
                    index == "\n") // Comment
                {
                    // Aviod Warning
                } else if (index == "newmtl") // New Material
                {
                    mtl = string::split(line, " ", 1);
                } else
                    unrecognized += index + " ";
            }

            // Print Invalid Elements
            if (unrecognized.size() > 0)
                LOG::WARNING("Unrecognized Material Element(s): [ " +
                             unrecognized + "]");

            // Finish Up
            f.close();
            loaded_material[path] = out;
        } else {
            // Can't Open File
            LOG::WARNING("Can't Open Material File: " + path);
            return out;
        }
    }

    // Return Loaded
    return out;
}

// TODO: Object Groups
// Load 3D Model (.obj)
Model model(std::string path, float scale = 1.0f) {
    Model out;

    if (hashing && loaded_object.count(path) > 0)
        // Already Hashed
        out = loaded_object[path];
    else {
        // Open File
        std::ifstream f(PATH_OBJECT + path);
        if (f.is_open()) {
            // Load File
            std::string unrecognized = "";

            std::vector<vec3> vertices;
            std::vector<vec2> texcoords;
            std::vector<vec3> normals;

            int o = -1;
            std::string line;
            while (getline(f, line)) {
                line = string::multispace(line);
                std::string index = string::split(line, " ", 0);

                if (line[0] == '#' || index == "" || index == " " ||
                    index == "\n") // Comment / Empty
                {
                    // Avoid Warning
                } else if (index == "o") // New Object
                {
                    o++;
                    out.mesh.push_back(Mesh());
                    out.name[string::split(line, " ", 1)] = o;
                } else if (index == "mtllib") // New Material
                    out.material = material(string::split(line, " ", 1));
                else if (index == "usemtl") // Use Material
                    out.mesh[o].material = string::split(line, " ", 1);
                else if (index == "v") // Vertex Point
                {
                    if (string::count(line, ' ') == 3)
                        vertices.push_back(vec3(string::fsplit(line, " ", 1),
                                                string::fsplit(line, " ", 2),
                                                string::fsplit(line, " ", 3)));
                    else
                        LOG::WARNING("Broken Model Line: " + line);
                } else if (index == "vt") // Vertex Texture
                {
                    if (string::count(line, ' ') >= 2)
                        texcoords.push_back(vec2(string::fsplit(line, " ", 1),
                                                 string::fsplit(line, " ", 2)));
                    else
                        LOG::WARNING("Broken Model Line: " + line);
                } else if (index == "vn") // Vertex Normal
                {
                    if (string::count(line, ' ') >= 3)
                        normals.push_back(vec3(string::fsplit(line, " ", 1),
                                               string::fsplit(line, " ", 2),
                                               string::fsplit(line, " ", 3)));
                    else
                        LOG::WARNING("Broken Model Line: " + line);
                } else if (index == "f") // Face Element
                {
                    int mode = string::count(line, '/');

                    // Validate Line
                    if (mode % 3 == 0) {
                        std::string part[3];
                        part[0] = string::split(line, " ", 1);
                        part[1] = string::split(line, " ", 2);
                        part[2] = string::split(line, " ", 3);

                        // TODO: Optimize Vertex-Loading

                        Vertex p[3];

                        // Get Vertices
                        p[0].position =
                            vertices[string::isplit(part[0], "/", 0) - 1] *
                            scale;
                        p[1].position =
                            vertices[string::isplit(part[1], "/", 0) - 1] *
                            scale;
                        p[2].position =
                            vertices[string::isplit(part[2], "/", 0) - 1] *
                            scale;

                        // Get TexCoords
                        if (mode == 3 || mode == 6) {
                            p[0].texcoord =
                                texcoords[string::isplit(part[0], "/", 1) - 1];
                            p[1].texcoord =
                                texcoords[string::isplit(part[1], "/", 1) - 1];
                            p[2].texcoord =
                                texcoords[string::isplit(part[2], "/", 1) - 1];
                        }

                        // Get Normals
                        if (mode == 6) {
                            p[0].normal =
                                normals[string::isplit(part[0], "/", 2) - 1];
                            p[1].normal =
                                normals[string::isplit(part[1], "/", 2) - 1];
                            p[2].normal =
                                normals[string::isplit(part[2], "/", 2) - 1];
                        }

                        out.mesh[o].vertices.push_back(p[0]);
                        out.mesh[o].vertices.push_back(p[1]);
                        out.mesh[o].vertices.push_back(p[2]);
                    }
                } else
                    unrecognized += index + " ";
            }

            // Print Invalid Elements
            if (unrecognized.size() > 0)
                LOG::WARNING("Unrecognized Model Element(s): [ " +
                             unrecognized + "]");

            // Finish Up
            f.close();
            loaded_object[path] = out;
        } else {
            // Can't Open File
            LOG::WARNING("Can't Open Model File: " + path);
            return out;
        }
    }

    // Load Into VRAM
    glGenVertexArrays(1, &out.VAO);
    glBindVertexArray(out.VAO);

    for (int i = 0; i < (int)out.mesh.size(); i++) {
        glGenBuffers(1, &out.mesh[i].VBO);
        glBindBuffer(GL_ARRAY_BUFFER, out.mesh[i].VBO);

        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(Vertex) * out.mesh[i].vertices.size(),
                     &out.mesh[i].vertices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(i * 3 + 0);
        glVertexAttribPointer(i * 3 + 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *)offsetof(Vertex, position));
        // vertex texture coords
        glEnableVertexAttribArray(i * 3 + 1);
        glVertexAttribPointer(i * 3 + 1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *)offsetof(Vertex, texcoord));
        // vertex normals
        glEnableVertexAttribArray(i * 3 + 2);
        glVertexAttribPointer(i * 3 + 2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *)offsetof(Vertex, normal));
    }

    // Return Loaded
    return out;
}

// TODO: Hash-map Cubemaps !
// TODO: custom Cubemap file-format ?
// Load CubeMap (list of images)
texture cubemap(std::vector<std::string> faces) {
    texture out;

    glGenTextures(1, &out);
    glBindTexture(GL_TEXTURE_CUBE_MAP, out);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        int width, height, nrChannels;
        unsigned char *data = stbi_load((PATH_TEXTURE + faces[i]).c_str(),
                                        &width, &height, &nrChannels, 0);
        if (data) {
            glGenTextures(1, &out);
            glBindTexture(GL_TEXTURE_2D, out);

            uint mode = GL_RGB;
            switch (nrChannels) {
            case 1:
                mode = GL_RED;
                break;

            case 3:
                mode = GL_RGB;
                break;

            case 4:
                mode = GL_RGBA;
                break;

            default:
                LOG::WARNING("CubeMap Has " + itos(nrChannels) + " Channels");
                break;
            }

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, mode, width,
                         height, 0, mode, GL_UNSIGNED_BYTE, data);

            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
                            GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,
                            GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
                            GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
                            GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
                            GL_CLAMP_TO_EDGE);

            stbi_image_free(data);
        } else {
            LOG::WARNING("Failed to load cubemap face: " + faces[i]);
            return out;
        }
    }

    return out;
}
} // namespace load
