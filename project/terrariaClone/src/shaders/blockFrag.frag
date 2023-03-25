#version 430 core

out vec4 FragColor;
layout (location = 2) uniform float uDarkness;
layout (location = 5) uniform sampler2D uSpriteSheet;
layout (location = 6) uniform sampler2D uBroken1;
layout (location = 7) uniform sampler2D uBroken2;
layout (location = 8) uniform sampler2D uBroken3;
layout (location = 9) uniform sampler2D uTex;
layout (location = 10) uniform vec2 uRes;

in vec2 fSpriteCoords;
in vec2 fTexCoords;
in vec3 fLight;
in float fBreaking;

void main()
{
	vec4 spriteCol = texture(uSpriteSheet, fSpriteCoords) * vec4(fLight * uDarkness, 1.0f);
	if(fBreaking < 0.3333){
		vec4 b = texture(uBroken3, fTexCoords);
		spriteCol.rgb *= 1-b.a;
	}else if(fBreaking< 0.6666){
				vec4 b = texture(uBroken2, fTexCoords);
		spriteCol.rgb *= 1-b.a;
	} else if(fBreaking < 1){
		vec4 b = texture(uBroken1, fTexCoords);
		spriteCol.rgb *= 1-b.a;
	}

	FragColor = vec4((1-spriteCol.a), 0,0,1);
	FragColor = spriteCol;

}

