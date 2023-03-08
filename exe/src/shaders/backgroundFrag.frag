#version 430 core

out vec4 FragColor;

layout (location = 2)   uniform sampler2D b11;
layout (location = 3)   uniform sampler2D b12;
layout (location = 4)   uniform sampler2D b13;
layout (location = 5)   uniform vec2 bsize1;
layout (location = 6)   uniform sampler2D b21;
layout (location = 7)   uniform sampler2D b22;
layout (location = 8)   uniform sampler2D b23;
layout (location = 9)   uniform vec2 bsize2;

layout (location = 10)  uniform vec3 color;
layout (location = 11)  uniform float offset;
layout (location = 12)  uniform float factor;

in vec2 t1;

void main()
{	
	float uvX1 = t1.x * (1000.0f/bsize1.x);
	float uvY = t1.y;
	float uvX2 = t1.x * (1000.0f/bsize2.x);

	vec4 tex11 = texture(b11, vec2(uvX1 + offset * 4, uvY));
	vec4 tex12 = texture(b12, vec2(uvX1 + offset * 2, uvY));
	vec4 tex13 = texture(b13, vec2(uvX1 + offset, uvY));

	vec4 tex21 = texture(b21, vec2(uvX2 + offset  * 4, uvY));
	vec4 tex22 = texture(b22, vec2(uvX2 + offset * 2, uvY));
	vec4 tex23 = texture(b23, vec2(uvX2 + offset, uvY));

	vec4 first = vec4(0);
	vec4 second = vec4(0);

	first = tex13;
	first.rgb *= (1-tex12.a);
	first += tex12 * tex12.a;
	first.rgb *= (1-tex11.a);
	first += tex11 * tex11.a;

	second = tex23;
	second.rgb *= (1-tex22.a);
	second += tex22 * tex22.a;
	second.rgb *= (1-tex21.a);
	second += tex21 * tex21.a;

	float fac = clamp(factor, 0, 1);
	FragColor = first * (1-fac) + second * fac;
	FragColor.rgb *= color;
	FragColor = clamp(FragColor, 0,1);
}
