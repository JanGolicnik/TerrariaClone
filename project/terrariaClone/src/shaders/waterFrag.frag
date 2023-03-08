#version 430 core

out vec4 FragColor;
layout (location = 5) uniform sampler2D uSpriteSheet;
layout (location = 6) uniform sampler2D uCaustics;
layout (location = 7) uniform int uTime;
layout (location = 8) uniform vec2 uRes;
layout (location = 9) uniform sampler2D uBase;

in vec2 fSpriteCoords;
in vec2 fTexCoords;
in vec3 fLight;
in float fLevel;

void main()
{
	float height = 8 / 900.0f;

	FragColor = vec4(0);

	if(fTexCoords.y > fLevel) return;

	vec2 c = gl_FragCoord.xy/uRes;

	vec4 col = texture(uSpriteSheet, fSpriteCoords + vec2(0, height * (1-fLevel))).rgba * vec4(fLight.rgb,1);
	
	float brightness = texture(uCaustics, c + uTime/5000.0f).r;
	brightness = pow(brightness, 9);
	brightness = 1 + brightness/5;
	col.rgb *= brightness;
	brightness = texture(uCaustics, c*2 + sin(uTime/2500.0f)).r;
	brightness = pow(brightness, 9);
	brightness = 1 + brightness/10;
	col.rgb *= brightness;

	float offset = texture(uCaustics, c*8 + uTime/200.0f).r;
	FragColor = texture(uBase, c + offset / 500);

	FragColor.rgb = col.rgb * col.a + FragColor.rgb * (1-col.a);

}

