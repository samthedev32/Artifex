#shader vertex

#version 300 es

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out struct {
	vec2 TexCoord;
	vec2 localPos;
	vec2 globalPos;

	vec2 center;
	vec2 size;
} frag;

uniform vec2 center;
uniform vec2 size;
uniform float rotation;

uniform vec2 ratio;

void main() {
	vec2 point = vec2(size.x * aPos.x, size.y * aPos.y);

	frag.localPos = point + center;
	vec2 pos;

	pos.x += (cos(rotation) * (point.x) - sin(rotation) * (point.y));
	pos.y += (sin(rotation) * (point.x) + cos(rotation) * (point.y));

	pos *= ratio;
	pos += center;

	frag.globalPos = pos + center;

	frag.center = center;
	frag.size = size;

	frag.TexCoord = aTexCoord;

	gl_Position = vec4(pos.xy, 0.0, 1.0);
}

#shader fragment

#version 300 es
precision mediump float;
out vec4 FragColor;

const float PI = 3.14159265;

in struct {
	vec2 TexCoord;
	vec2 localPos;
	vec2 globalPos;

	vec2 center;
	vec2 size;
} frag;

uniform int look;

uniform sampler2D tex;
uniform vec3 color;

uniform float cutradius;
uniform float corner;

// from https://iquilezles.org/articles/distfunctions
float roundedBoxSDF(vec2 center, vec2 size, float radius) {
    return length(max(abs(center) - size + radius, 0.0)) - radius;
}

void main() {
	float radius = corner * min(frag.size.x, frag.size.y);

	float distance = roundedBoxSDF(frag.localPos - frag.center, frag.size, radius);

	float smoothedAlpha =  1.0 - smoothstep(0.0, 0.002, distance);

	vec3 color = 0.5 + 0.5*cos(5.0 + frag.globalPos.xyx * 5.0 + vec3(0, 2, 4));
	FragColor		= vec4(color, smoothedAlpha);
}