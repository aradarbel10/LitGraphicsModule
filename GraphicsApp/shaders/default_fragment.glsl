#version 330 core

in vec2 texCoords;

out vec4 FragColor;

uniform vec4 color;
uniform sampler2D myTexture;
uniform bool has_texture;

void main() {
	FragColor = mix(texture(myTexture, texCoords), color, has_texture ? 0.5 : 1);
}