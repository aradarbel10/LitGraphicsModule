#version 330 core

layout (location = 0) in vec2 aPos;

uniform vec2 wsize;

uniform vec2 tpos;
uniform vec2 tscale;
uniform float trot;

//pass-through
layout (location = 1) in vec2 aTexCoords;
out vec2 texCoords;

void main() {
	//pass-through
	texCoords = aTexCoords;

	//apply transformation
	vec2 transformed = mat2(tscale.x, 0, 0, tscale.y) * mat2(cos(trot), sin(trot), -sin(trot), cos(trot)) * aPos;
	transformed += tpos;
	gl_Position = vec4(transformed.x / wsize.x, transformed.y / wsize.y, 0.0, 1.0);
}