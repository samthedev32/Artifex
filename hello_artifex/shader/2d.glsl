// Basic 2D Shader

#shader vertex
#version 300 es

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out vec2 FragPos;

uniform vec2 center;
uniform vec2 size;
uniform float rotation;

uniform vec2 ratio;

void main() {
    float rads = radians(-rotation);
    vec2 point = vec2(size.x * aPos.x, size.y * aPos.y);

    float x = cos(rads) * (point.x) - sin(rads) * (point.y) + center.x;
    float y = sin(rads) * (point.x) + cos(rads) * (point.y) + center.y;

    x *= ratio.x;
    y *= ratio.y;

    TexCoord = aTexCoord;
    FragPos = vec2(aPos.x, aPos.y);
    gl_Position = vec4(x, y, 0.0, 1.0);
}

#shader fragment
#version 300 es
precision mediump float;
out vec4 FragColor;

in vec2 TexCoord;
in vec2 FragPos;

uniform int type;

uniform sampler2D tex;
uniform vec3 color;

uniform float cutradius;

void main() {
	switch(type) {
		case 0:
			// Color
			FragColor = vec4(color.rgb, 1.0);
			break;

		case 1:
			// Texture
			FragColor = texture(tex, TexCoord);
			break;

		case 2:
		// Text
			FragColor = vec4(color.rgb, length(texture(tex, TexCoord).rgb));
			break;

		case 3:
			// Circle
			vec2 val = FragPos;

			float R = 1.0f;
			float R2 = cutradius;
			float dist = sqrt(dot(val, val));

			if(dist >= R || dist <= R2)
				discard;

			float sm = smoothstep(R, R - 0.01, dist);
			float sm2 = smoothstep(R2, R2 + 0.01, dist);
			float alpha = sm * sm2;

			FragColor = vec4(color.xyz, 1.0);
			break;

		default:
			FragColor = vec4(color.xyz, 0.0);
			break;
	}
}