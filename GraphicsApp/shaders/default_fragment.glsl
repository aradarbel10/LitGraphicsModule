#version 330 core

out vec4 FragColor;

uniform vec4 color;

void main() {
	FragColor = vec4(color.r, color.g, color.b, 0.5);
}