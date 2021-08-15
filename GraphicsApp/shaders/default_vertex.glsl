#version 330 core

layout (location = 0) in vec2 aPos;

uniform vec2 wsize;

uniform vec2 tpos;
uniform vec2 tscale;
uniform float trot;

uniform vec2 tex_tpos;
uniform vec2 tex_tscale;
uniform float tex_trot;

out vec2 texCoords;

void main() {
	vec2 transformed = mat2(tscale.x, 0, 0, tscale.y)
						* mat2(cos(trot), sin(trot), -sin(trot), cos(trot))
						* aPos
						+ tpos;

	gl_Position = vec4(transformed.x / wsize.x, transformed.y / wsize.y, 0.0, 1.0);

	texCoords = mat2(tex_tscale.x, 0, 0, tex_tscale.y)
				* mat2(cos(tex_trot), sin(tex_trot), -sin(tex_trot), cos(tex_trot))
				* aPos
				+ tex_tpos;
}