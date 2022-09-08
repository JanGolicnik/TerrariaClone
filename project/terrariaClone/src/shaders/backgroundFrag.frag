#version 430 core

out vec4 FragColor;

layout (location = 5) uniform sampler2D b1;
layout (location = 6) uniform float fog;
layout (location = 7) uniform vec3 color;
layout (location = 8) uniform float offset;

in vec2 t1;

void main()
{	
	float uvX = (t1.x + offset) * (1000.0f/1500);
	float uvY = t1.y;

	vec4 t = texture(b1, vec2(uvX, uvY));

	FragColor = vec4( ((t.rgb * (1-fog)) + fog) * color, t.a);
}
