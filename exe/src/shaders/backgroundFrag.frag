#version 430 core

out vec4 FragColor;

struct bg{
	sampler2D tex;
	vec2 res;
	vec3 offset;
	bool repeatY;
};

layout (location = 0)  uniform vec3  uColor;
layout (location = 1)  uniform float uOpacity;
layout (location = 2)  uniform vec2 uRes;
layout (location = 3)  uniform float uYOffset;

layout (location = 4) uniform bg[7] uBgs;

in vec2 t1;

void main()
{	
	vec4 col = vec4(0);

	vec2 neki;

	for(int i = 0; i < 7; i++){
		if(uBgs[i].res == vec2(0)) break;
		vec2 ratio = uRes/uBgs[i].res;
		vec2 c = t1 * ratio + uBgs[i].offset.xy;
		c.y += uYOffset * ratio.y - ratio.y/2;
		c /= uBgs[i].offset.z;

		c.x = c.x - int(c.x);
		vec4 texCol = texture(uBgs[i].tex, c); 
		col.rgb *= (1-texCol.a);
		col += texCol;
	}
	
	FragColor = col;
	FragColor.rgb *= uColor;
	FragColor = clamp(FragColor, 0,1) * uOpacity;
}
