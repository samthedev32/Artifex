// Main Artifex Library
#pragma once

#include <artifex/window.h>
#include <artifex/mixer.h>

class Artifex : public Window, public Mixer
{
private:
    long past, now; //< The Previous and Current Update's Time

    shader s;      //< The Default 2D Rendering Shader
    uint VAO, VBO; //< The Rendering Vertex-Array-Object and Vertex-Buffer-Object
    font chars;    //< A Built-In Font

    std::map<std::string, int> input; //< Temporary Variables for Buttons and Interactables

    // Initalize Artifex
    void init();

public:
    float deltaTime;

    // Constructors & Destructor

    Artifex(std::string name, int w, int h);
    Artifex();
    ~Artifex();

    bool update();
};

// Init
void Artifex::init()
{
    // TODO: Set Up 2D Renderer, Mixer, etc
}

// Constructors & Destructor

Artifex::Artifex(std::string name, int w, int h) : Window(name, false, w, h), Mixer()
{
    // Init 2D Renderer
    s = load::gls("2d");
    s.use();

    float vertices[] = {
        // positions      // texture coords
        -1.0f, 1.0f, 0.0f, 1.0f,  // 0 top right
        -1.0f, -1.0f, 0.0f, 0.0f, // 1 bottom right
        1.0f, -1.0f, 1.0f, 0.0f,  // 3 top left

        -1.0f, 1.0f, 0.0f, 1.0f, // 1 bottom right
        1.0f, -1.0f, 1.0f, 0.0f, // 2 bottom left
        1.0f, 1.0f, 1.0f, 1.0f,  // 3 top left
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    chars = load::ttf("DroidSansMono.ttf");

    // Config OpenGL
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    // glCullFace(GL_FRONT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    now = time();
}

Artifex::Artifex() : Artifex("Window", 720, 480)
{ // Init 2D Renderer
}

Artifex::~Artifex()
{
    glDeleteShader(s.id);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

bool Artifex::update()
{
    // Update Time
    past = now;
    now = time();
    deltaTime = (now - past) / 1000.0f;

    return Window::update(color.r, color.g, color.b);
}