// Artifex Library
#pragma once

#include <artifex/core/window.h>
#include <artifex/core/mixer.h>

// Function Prototypes
bool setup();
bool loop();
void cleanup() __attribute__((weak));

// TODO: Render Thread (?)

// Artifex - A Light-Weight, Cross-Platform 2D Game Engine
class Artifex : public Window, public Mixer
{
private:
    long past, now; //< The Previous Update's Time

    uint VAO, VBO; //< The Rendering Vertex-Array-Object and Vertex-Buffer-Object
    font tfont;

    std::map<std::string, int> input; //< Temporary Variables for Buttons and Interactables

    // Apply 2D Renderer
    void apply()
    {
        // Change FrameBuffer
        int fb;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fb);
        if (fb != 0)
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Disable 3D Depth Testing
        glDisable(GL_DEPTH_TEST);

        // Change ViewPort
        int vp[4];
        glGetIntegerv(GL_VIEWPORT, vp);
        if (vp[2] != size[0] || vp[3] != size[1])
            glViewport(0, 0, size[0], size[1]);

        // Use Shader
        s.use();
        s.set("tex", 0);
    }

    // Get Screen Ratio Multiplyer
    vec2 ratio()
    {
        if (dres)
            return vec2(1.0f, (float)size[0] / (float)size[1]);
        return vec2(1.0f, 1.0f);
    }

public:
    long uptime;
    float deltaTime;

    shader s;

    bool dres = true;

    // Constructors & Destructor

    Artifex(std::string name, bool fullscreen = true, int w = 0, int h = 0);
    Artifex(std::string name, int w, int h);
    ~Artifex();

    bool update(vec3 color = vec3());

    // Basic 2D Rendering

    void circle(vec2 center, float radius, float cutradius, vec3 color);                      //< Draw Circle
    void rect(vec2 center, vec2 size, vec3 color, float scale = 1.0f);                        //< Draw Colored Rectangle
    void rect(vec2 center, vec2 size, texture tex, float scale = 1.0f);                       //< Draw Textured Rectangle
    void rect(vec2 center, vec2 size, texture *texs, uint n, uint speed, float scale = 1.0f); //< Draw Animated Rectangle
    void text(vec2 center, float size, std::string text, vec3 color, float height = 1.5f);

    // User Inputs

    bool button(vec2 center, vec2 size, std::string name, texture a = load::rgb({0, 1}), texture b = load::rgb(1));
    bool toggle(vec2 center, vec2 size, std::string name, texture a = load::rgb({0, 1}), texture b = load::rgb(1));
    bool touch(vec2 center, vec2 size, texture a = load::rgb({0, 1}), texture b = load::rgb(1));
    vec2 joystick(vec2 center = vec2(), std::string name = "joystick", float radius = 0.2f, float nob_radius = 0.04f, vec3 color = vec3(1.0f, 0.4f, 0.0f), vec3 nob_color = vec3(1.0f, 1.0f, 1.0f));
};

// Constructors & Destructor

Artifex::Artifex(std::string name, bool fullscreen, int w, int h) : Window(name, fullscreen, w, h), Mixer()
{
    // Init 2D Renderer
    s = load::gls("2d");

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

    // Config OpenGL
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Init Text Renderer
    tfont = load::ttf("ubuntu.png");

    // Set Up DeltaTime
    uptime = 0;
    now = time();
}

Artifex::Artifex(std::string name, int w, int h) : Artifex(name, false, w, h)
{
}

Artifex::~Artifex()
{
    glDeleteShader(s.id);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

bool Artifex::update(vec3 color)
{
    // Update Time
    past = now;
    now = time();
    deltaTime = (now - past) / 1000.0f;

    uptime += (now - past);

    return Window::update(color.r, color.g, color.b);
}

/*  ----  Basic 2D Rendering  ----  */

/* Draw Circle */
void Artifex::circle(vec2 center, float radius, float cutradius, vec3 color)
{
    apply();

    s.set("type", 3);
    s.set("cutradius", cutradius);
    s.set("color", color);

    s.set("center", center);
    s.set("size", vec2(radius, radius));
    s.set("ratio", ratio());

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
}

/* Draw Color */
void Artifex::rect(vec2 center, vec2 size, vec3 color, float scale)
{
    apply();

    s.set("type", 0);
    s.set("color", color);

    s.set("center", center);
    s.set("size", vec2(size / 2) * scale);
    s.set("ratio", ratio());

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

/* Draw Texture */
void Artifex::rect(vec2 center, vec2 size, texture tex, float scale)
{
    apply();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    s.set("type", 1);

    s.set("center", center);
    s.set("size", size / 2.0f * scale);
    s.set("ratio", ratio());

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

/* Draw Animation */
void Artifex::rect(vec2 center, vec2 size, texture *texs, uint n, uint speed, float scale)
{
    rect(center, size, texs[now / speed % n], scale);
}

// Render Text
void Artifex::text(vec2 center, float width, std::string text, vec3 color, float height)
{
    apply();

    s.set("type", 2);
    s.set("color", color);
    s.set("ratio", ratio());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tfont.data);

    float step = width / text.size();
    float left = center.x - step * text.size() / 2;

    // iterate through all characters
    for (int i = 0; i < (int)text.size(); i++)
    {
        char c = text[i] - tfont.start;

        // Set Position
        s.set("center", vec2(left + i * step, center.y));
        s.set("size", vec2(step, step * height));

        vec2 start = {(c % tfont.width) / (float)tfont.width, (c / tfont.height + 1) / (float)tfont.height};
        vec2 csize = {1.0f / (float)tfont.width, 1.0f / (float)tfont.height};

        tfont.vertices[2] = start.x;
        tfont.vertices[3] = 1 - start.y + csize.y;
        tfont.vertices[6] = start.x;
        tfont.vertices[7] = 1 - start.y;
        tfont.vertices[10] = start.x + csize.x;
        tfont.vertices[11] = 1 - start.y;

        tfont.vertices[14] = start.x;
        tfont.vertices[15] = 1 - start.y + csize.y;
        tfont.vertices[18] = start.x + csize.x;
        tfont.vertices[19] = 1 - start.y;
        tfont.vertices[22] = start.x + csize.x;
        tfont.vertices[23] = 1 - start.y + csize.y;

        glBindBuffer(GL_ARRAY_BUFFER, tfont.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(tfont.vertices), tfont.vertices, GL_STATIC_DRAW);

        glBindVertexArray(tfont.VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

/*  ----  User Inputs  ----  */

/* Draw A Button */
bool Artifex::button(vec2 center, vec2 size, std::string name, texture a, texture b)
{
    if (math::collide(cursor, center, size * ratio()))
    {
        if (input[name] && key("mouse") == false)
        {
            input[name] = false;
            return true;
        }
        input[name] = key("mouse");
    }
    else
        input[name] = false;

    rect(center, size, input[name] ? b : a);
    return false;
}

bool Artifex::toggle(vec2 center, vec2 size, std::string name, texture a, texture b)
{
    if (math::collide(cursor, center, size * ratio()))
    {
        if (key("mouse") == true)
        {
            if (input[name] > 0)
                input[name] = 2;

            if (input[name] <= 0)
                input[name] = -2;
        }
        else
        {
            if (input[name] == 2)
                input[name] = -1;

            if (input[name] == -2)
                input[name] = 1;
        }
    }
    else
    {
        if (input[name] > 0)
            input[name] = 1;
        if (input[name] <= 0)
            input[name] = -1;
    }

    rect(center, size, input[name] > 0 ? b : a);

    return false;
}

bool Artifex::touch(vec2 center, vec2 size, texture a, texture b)
{
    if (key("mouse") && math::collide(cursor, center, size * ratio()))
    {
        rect(center, size, b);
        return true;
    }

    rect(center, size, a);
    return false;
}

vec2 Artifex::joystick(vec2 center, std::string name, float radius, float nob_radius, vec3 color, vec3 nob_color)
{
    circle(center, radius, 0.9f, color);

    vec2 pos = center;

    if (key("mouse"))
    {
        if (cursor.distance(center) <= radius && input[name] == false)
            input[name] = true;

        if (input[name])
            pos = cursor;
    }
    else
        input[name] = false;

    // Works - Don't touch it!
    vec2 direction = (pos - center).normalize() * radius;
    float distance = math::clamp(pos.distance(center) / radius, -1.0f, 1.0f);

    circle(center + direction * distance * ratio(), nob_radius, 0.0f, nob_color);

    vec2 a = pos - center;
    a.x = math::map(a.x, -radius, radius, -1, 1);
    a.y = math::map(a.y, -radius, radius, -1, 1);

    return math::clamp(a, vec2(-1.0f, -1), vec2(1.0f, 1.0f));
}

// Web Loop
#ifdef __browser
void emloop()
{
    loop();
}
#endif

// Main Function
int main()
{
    // run setup, exit on setup-error
    if (!setup())
        LOG::ERROR("Setup Failed!");

#ifdef __browser
    // Run Web Main-Loop
    emscripten_set_main_loop(emloop, 0, 1);
#else
    // Run Local Main-Loop
    while (loop())
        ;
#endif

    // Call optional "cleanup" function
    if (cleanup)
        cleanup();
    return 0;
}