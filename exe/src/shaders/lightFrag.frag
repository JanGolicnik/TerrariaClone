#version 430 core

out vec4 FragColor;
layout (location = 9) uniform sampler2D uTex;
layout (location = 10) uniform vec2 uRes;

in vec2 fSpriteCoords;
in vec2 fTexCoords;
in vec3 fLight;
in float fBreaking;

void main()
{
	vec2 c  = gl_FragCoord.xy / uRes;

	vec3 light = clamp(fLight, vec3(0), vec3(1.2));
	FragColor.rgb = texture(uTex, c).rgb * light;
	FragColor.a = 1;
}

