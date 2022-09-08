#version 430 core

out vec4 FragColor;

layout (location = 2) uniform float transmit = 0.98;
layout (location = 3) uniform int txrsiz = 500;
layout (location = 4) uniform vec2 t0;
layout (location = 5) uniform sampler2D txrmap;
layout (location = 8) uniform vec2 lights[10];

in smooth vec2 t1;

void main()
{
	int i = 0;
	vec2 t, dt;
	vec4 c0, c1;
	dt = normalize(t1- t0) / float(txrsiz);
	c0 = vec4(0.9f, 0.7f, 0.1f, 1.0f);
	vec4 maptx = texture2D(txrmap, t1);

	FragColor = vec4(0.0f);
	t = t0;

	float dist = distance(t1,t);
	if(dist > 0.5f){
		c0 = vec4(0.0f,0.0f,0.0f,1.0f);
	}else{

		if( dot(t1- t, dt) > 0.0f)
		{
				for(i = 0; i < txrsiz; i++)
				{
						c1 = texture2D(txrmap, t);
						c1.a *= 1.0f;
						c0.rgb *= ( ( c1.a * c1.rgb ) + (1.0f - c1.a) * transmit );
						if( dot( t1 - t, dt) <= 0.0f ){
							break;
							}
						if(c0.r + c0.g + c0.b <= 0.0001f){
							break;
							}
						t += dt;

				}

		}
	}
	c0.rgb *= 1-dist;
	FragColor = 0.8f * c0 + 0.2f * texture2D(txrmap, t1);
	
	//FragColor = c0;
}
