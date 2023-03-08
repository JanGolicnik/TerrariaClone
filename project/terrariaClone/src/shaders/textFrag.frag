#version 430 core

out vec4 FragColor;

layout (location = 0) uniform sampler2D spriteSheet;

in vec2 fSpriteCoords;
in vec3 fColor;

const float offset = 0.004;

const vec2 offsets[8] = {
	vec2(-offset,  offset),  vec2(0,  offset), vec2(offset, offset),
	vec2(-offset, 0),						   vec2(offset, 0),
	vec2(-offset, -offset),  vec2(0, -offset), vec2(offset, -offset),
};

void main()
{
	vec4 mainColor = vec4(fColor, 1.0f) * texture2D(spriteSheet, fSpriteCoords).r;
	float border = 0;
	for(int i = 0; i < 8; i++){
		vec2 offsetCoords = fSpriteCoords + offsets[i];
		border += texture2D(spriteSheet, offsetCoords).r;
	}
	border = clamp(border, 0,1);
	FragColor.a = border;
	FragColor.rgb += mainColor.rgb;
}