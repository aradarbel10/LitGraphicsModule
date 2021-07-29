#version 330 core

in vec2 texCoords;

out vec4 FragColor;

uniform vec4 color;
uniform sampler2D myTexture;

void main() {
	//FragColor = vec4(color.r, color.g, color.b, 0.5);
	FragColor = texture(myTexture, texCoords);
}