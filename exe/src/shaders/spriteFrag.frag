#version 430 core

out vec4 FragColor;

layout (location = 1) uniform sampler2D[2] uSpriteSheets;

in vec2 fSpriteCoords;
in vec4 fColor;
in float fSpritesheet;

void main()
{
	if(fSpritesheet == 0){
		FragColor = texture2D(uSpriteSheets[0], fSpriteCoords) * fColor;
	}else{
		FragColor = fColor * texture2D(uSpriteSheets[1], fSpriteCoords);
	}
}

