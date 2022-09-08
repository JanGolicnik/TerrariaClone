#version 430 core

out vec4 FragColor;

layout (location = 0) uniform sampler2D spriteSheet;

in vec2 fSpriteCoords;

void main()
{
	FragColor = vec4(1.0f) * texture2D(spriteSheet, fSpriteCoords).r;
}

