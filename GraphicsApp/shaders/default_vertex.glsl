#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 col;

out vec4 MyColor;

void main() {
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
	MyColor = vec4(col.x, col.y, col.z, 1.0);
}