#version 430 core

out vec4 FragColor;

layout (location = 2) uniform sampler2D uSpriteSheet;

in vec2 fSpriteCoords;

void main()
{
	FragColor = texture(uSpriteSheet, fSpriteCoords);
}

