#include <Artifex/Artifex.h>
#include <string>

// Public

Artifex::Artifex(std::string name, uint width, uint height)
    : Window(name, width, height) {}

Artifex::~Artifex() {}

bool Artifex::update() { return Window::update(); }

void Artifex::clear(float red, float green, float blue, GLbitfield buffers) {
    glClearColor(red, green, blue, 1.0f);
    glClear(buffers);
}

uint Artifex::shader(std::string vertex, std::string fragment,
                     std::string geometry) {
    // Exit if no Shader Code
    if (fragment == "" || vertex == "") {
        printf("ERROR: No Shader Resource\n");
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

    shaders.push_back(id);
    return shaders.size();
}

// Private

float Artifex::ratio() { return (float)width / (float)height; }