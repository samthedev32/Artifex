#pragma once

const char* res_vertex =
    "#version 300 es\n"

    "layout(location = 0) in vec2 aPos;\n"
    "layout(location = 1) in vec2 aTexCoord;\n"

    "out struct {\n"
    "    vec2 TexCoord;// Texture Coordinates\n"
    "    vec2 localPos;// Local Pos (in rect)\n"
    "    vec2 globalPos;// Global Pos (on screen)\n"
    "    vec2 center;// Center\n"
    "    vec2 size;// Size\n"
    "} io;\n"

    "uniform struct {\n"
    "    vec2 center;\n"
    "    vec2 size;\n"
    "    float rotation;\n"
    "} v;\n"

    "void main() {\n"
    "    vec2 point = v.size * aPos;\n"

    "    io.localPos = aPos;\n"
    "    io.globalPos = point + v.center;\n"

    "    vec2 pos = v.center;\n"
    "    pos.x += cos(v.rotation) * (point.x) - sin(v.rotation) * (point.y);\n"
    "    pos.y += (sin(v.rotation) * (point.x) + cos(v.rotation) * (point.y));\n"

    "    io.center = v.center;\n"
    "    io.size = v.size;\n"

    "    io.TexCoord = aTexCoord;\n"

    "    gl_Position = vec4(pos.xy, 0.0, 1.0);\n"
    "}";

const char* res_fragment =
    "#version 300 es\n"

    "precision mediump float;\n"
    "out vec4 FragColor;\n"

    "in struct {\n"
    "    vec2 TexCoord;\n"
    "    vec2 localPos;\n"
    "    vec2 globalPos;\n"
    "    vec2 center;\n"
    "    vec2 size;\n"
    "} io;\n"

    "uniform struct {\n"
    "    int look;\n"
    "    sampler2D tex;\n"
    "    vec3 color;\n"
    "    vec4 corner;// topleft, topright, bottomleft, bottomright\n"
    "    float time;\n"
    "} f;\n"

    "// from https://iquilezles.org/articles/distfunctions\n"
    "float roundedBoxSDF(vec2 center, vec2 size, float radius) {\n"
    "    return length(max(abs(center) - size + radius, 0.0)) - radius;\n"
    "}\n"

    "void main() {\n"
    "    float corner = 0.0;\n"
    "    if (io.localPos.x < 0.0) {\n"
    "        if (io.localPos.y > 0.0)\n"
    "        corner = f.corner.x;\n"
    "        if (io.localPos.y < 0.0)\n"
    "        corner = f.corner.z;\n"
    "    } else if (io.localPos.x > 0.0) {\n"
    "        if (io.localPos.y > 0.0)\n"
    "        corner = f.corner.y;\n"
    "        if (io.localPos.y < 0.0)\n"
    "        corner = f.corner.w;\n"
    "    }\n"

    "    float radius = clamp(corner, 0.0, 1.0) * min(io.size.x, io.size.y);\n"

    "    float distance =\n"
    "    roundedBoxSDF(io.globalPos - io.center, io.size, radius);\n"

    "    float smoothedAlpha = 1.0 - smoothstep(0.0, 0.003, distance);\n"

    "    vec4 color;\n"
    "    switch (f.look) {\n"
    "        default :\n"
    "        case 0:\n"
    "        // TODO: customizability\n"
    "        color = vec4(0.5 + 0.5 * cos(f.time + io.globalPos.xyx + vec3(0.0, 2.0, 4.0)), 1);\n"
    "        break;\n"

    "        case 1:\n"
    "        // Custom Color\n"
    "        color = vec4(f.color, 1);\n"
    "        break;\n"

    "        case 2:\n"
    "        // Texture\n"
    "        color = texture(f.tex, io.TexCoord);\n"
    "        break;\n"

    "        // case 3:\n"
    "        // // Text\n"
    "        // color = vec4(texture(f.tex, io.TexCoord).r, 0, 0, 1);\n"
    "        // break;\n"
    "    }\n"

    "    FragColor = vec4(color.rgb, color.a * smoothedAlpha);\n"
    "}";