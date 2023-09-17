#shader vertex

#version 300 es

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out vec2 FragPos;

out vec2 Center;
out vec2 Size;

uniform vec2 center;
uniform vec2 size;
uniform float rotation;

uniform float ratio;

void main() {
	float rads = radians(-rotation);
	vec2 point = vec2(size.x * aPos.x, size.y * aPos.y);

	vec2 pos = center;

	Size = size;
	Center = center;

	pos.x += (cos(rads) * (point.x) - sin(rads) * (point.y));
	pos.y += (sin(rads) * (point.x) + cos(rads) * (point.y)) * ratio;

	TexCoord = aTexCoord;
	FragPos = vec2(aPos.x, aPos.y);
	gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);
}

#shader fragment

// TODO: rounded corners

#version 300 es
precision mediump float;
out vec4 FragColor;

const float PI = 3.14159265;

in vec2 TexCoord;
in vec2 FragPos;

in vec2 Center;
in vec2 Size;

uniform bool isTextured;
uniform int type;

uniform sampler2D tex;
uniform vec3 color;

uniform float cutradius;
uniform float corner;

// from https://iquilezles.org/articles/distfunctions
float roundedBoxSDF(vec2 center, vec2 size, float radius) {
    return length(max(abs(center) - size + radius, 0.0)) - radius;
}

void main() {
	switch (type) {
		default:
		case 0:	// Rect
			if (isTextured)
				FragColor = texture(tex, TexCoord);
			else
				FragColor = vec4(color.rgb, 1.0);
		break;

		case 1: // Circle
			vec2 val = FragPos;

			float R = 1.0f + 0.5 * corner;
			float R2 = cutradius;
			float dist = sqrt(dot(val, val));

			if (dist >= R || dist <= R2)
				discard;

			float sm = smoothstep(R, R - 0.01, dist);
			float sm2 = smoothstep(R2, R2 + 0.01, dist);
			float alpha = sm * sm2;

			if (isTextured)
				FragColor = texture(tex, TexCoord);
			else
				FragColor = vec4(color.xyz, 1.0);
		break;

		case 2: { // Rounded
			// TODO: replace rect & circle
			float radius = corner * 0.5f;
			
			float distance = roundedBoxSDF(FragPos - Center, Size, radius);
			
			float edgeSoftness = 1.0f;
			float smoothedAlpha =  smoothstep(edgeSoftness * 0.01f, 0.0f ,distance);
			
			vec3 color = 0.5 + 0.5*cos(0.0 + FragPos.xyx * 2.0 + vec3(0, 2, 4));
			FragColor		= mix(vec4(0.0, 0.0, 0.0, 0.0f), vec4(color, smoothedAlpha), smoothedAlpha);
 		} break;
	}
}