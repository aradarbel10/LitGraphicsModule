#version 330 core

out vec4 FragColor;

uniform vec4 color;

void main() {
	//FragColor = vec4(0.47f, 0.85f, 0.74f, 1.0f);
	FragColor = color;
}