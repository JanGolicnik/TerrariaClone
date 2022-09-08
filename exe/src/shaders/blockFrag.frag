#version 430 core

out vec4 FragColor;
layout (location = 2) uniform float uDarkness;
layout (location = 5) uniform sampler2D uSpriteSheet;
layout (location = 6) uniform sampler2D uBroken1;
layout (location = 7) uniform sampler2D uBroken2;
layout (location = 8) uniform sampler2D uBroken3;

in vec2 fSpriteCoords;
in vec2 fTexCoords;
in float fLight;
in float fBreaking;

void main()
{
	FragColor = texture(uSpriteSheet, fSpriteCoords) * vec4(vec3(fLight * uDarkness), 1.0f);
	if(fBreaking == 1){
			vec4 b = texture(uBroken3, fTexCoords);
		FragColor.rgb *= 1-b.a;
	}else if(fBreaking == 2){
				vec4 b = texture(uBroken2, fTexCoords);
		FragColor.rgb *= 1-b.a;
	} else if(fBreaking == 3){
		vec4 b = texture(uBroken1, fTexCoords);
		FragColor.rgb *= 1-b.a;
	}
}

