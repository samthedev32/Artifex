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

uniform struct {
	vec2 center;
	vec2 size;
	float rotation;

	vec2 ratio;
} uni;

uniform vec2 center;
uniform vec2 size;
uniform float rotation;

uniform vec2 ratio;

void main() {
	vec2 point = vec2(size.x * aPos.x, size.y * aPos.y);

	frag.localPos = point + center;
	vec2 pos = center;

	pos.x += (sin(rotation) * (point.x) - cos(rotation) * (point.y)) * ratio.x;
	pos.y += (cos(rotation) * (point.x) + sin(rotation) * (point.y)) * ratio.y;

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

in struct {
	vec2 TexCoord;

	vec2 localPos;
	vec2 globalPos;

	vec2 center;
	vec2 size;
} frag;

uniform struct {
	int look;

	sampler2D tex;
	vec3 color;

	float cutradius;
	float corner;

	float time;
} funi;

// from https://iquilezles.org/articles/distfunctions
float roundedBoxSDF(vec2 center, vec2 size, float radius) {
    return length(max(abs(center) - size + radius, 0.0)) - radius;
}

void main() {
	float radius = clamp(funi.corner, 0.0, 1.0) * min(frag.size.x, frag.size.y);

	float distance = roundedBoxSDF(frag.localPos - frag.center, frag.size, radius);

	float smoothedAlpha =  1.0 - smoothstep(0.0, 0.002, distance);

	vec3 color;
	switch (funi.look) {
		default:
		case 0:
			// TODO: customizability
			color = 0.5 + 0.5*cos(funi.time + frag.localPos.xyx * 5.0 + vec3(0, 2, 4));
			break;
		
		case 1:
			// Custom Color
			color = funi.color;
			break;

		case 2:
			// Texture
			color = texture(funi.tex, frag.TexCoord).xyz;
			break;
	}
	 
	FragColor = vec4(color, smoothedAlpha);
}