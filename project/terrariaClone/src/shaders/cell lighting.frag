



#version 430 core

out vec4 FragColor;

layout (location = 2) uniform float transmit = 0.98;
layout (location = 4) uniform vec2 t0;
layout (location = 5) uniform sampler2D txrmap;
layout (location = 7) uniform sampler2D fb;
layout (location = 8) uniform vec2 lights[10];

in smooth vec2 t1;
in vec2 fPos;

const float w  = 1.0f;
const float h  = 1.0f;

const vec2 offset[9] = {
    vec2(-w, -h), vec2(0.0f, -h), vec2(w, -h),
    vec2(-w, 0.0f), vec2(0.0f, 0.0f), vec2(w, 0.0f),
    vec2(-w, h), vec2(0.0f, h), vec2(w, h)
};

const float kernel[9] = {
    0.0,
    0.25,
    0.0,

    0.25,
    -1.0,
    0.25,

    0.0,
    0.25,
    0.0
};

void main()
{
	vec2 FC = vec2(gl_FragCoord.x / 128.0, gl_FragCoord.y / 100.0);
	vec2 f = gl_FragCoord.xy;
    float t = texture2D(txrmap, t1*8).a;
	
    vec4 col;
	col = texelFetch(fb, ivec2(FC.x * 128, FC.y * 100), 0);

	vec3 lap;
    for(int i = 0; i < 9; i++){
        vec3 tmp = texelFetch(fb, ivec2(f + offset[i]), 0).rgb;
        lap += vec3(tmp * kernel[i]);
    }
	col += vec4(lap,1.0f) * 0.9f;
    col = clamp(col, 0.0f, 1.0f);
    col *= 0.99;

	//set lights
	for(int i = 0; i < 10; i++){
		if(distance(FC, lights[i]) <= 0.01f){
			col = vec4(1.0f);
		}	
	}

    col *= 1-t;

	FragColor = vec4(clamp(col, 0.0f, 1.0f).rgb, 1.0f);
}
