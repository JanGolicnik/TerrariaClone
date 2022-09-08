#version 430 core

out vec4 FragColor;

layout (location = 0) uniform sampler2D spriteSheet;

in vec2 fSpriteCoords;

void main()
{
	FragColor = texture2D(spriteSheet, fSpriteCoords);
	//FragColor = vec4(fSpriteCoords, 0.0f, 1.0f);
	//FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
}

