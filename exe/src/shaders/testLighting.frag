#version 430 core

out vec4 FragColor;

layout (location = 2) uniform float transmit = 0.98;
layout (location = 4) uniform vec2 t0;
layout (location = 5) uniform sampler2D txrmap;
layout (location = 7) uniform sampler2D fb;
layout (location = 8) uniform vec2 lights[10];

in smooth vec2 t1;
in vec2 fPos;

void main()
{
    
    int resX = 128, resY = 100;

	vec2 FC = vec2(gl_FragCoord.x / resX, gl_FragCoord.y / resY);
	vec2 f = gl_FragCoord.xy;
    float t = texture2D(txrmap, t1*8).a;
	
    float col;
	col = texelFetch(fb, ivec2(FC.x * resX, FC.y * resY), 0).r;

    float surr = 0.0f;
    surr = texelFetch(fb, ivec2(FC.x * resX + 1, FC.y * resY), 0).r; 
    surr += texelFetch(fb, ivec2(FC.x * resX - 1, FC.y * resY), 0).r; 
    surr += texelFetch(fb, ivec2(FC.x * resX, FC.y * resY + 1), 0).r; 
    surr += texelFetch(fb, ivec2(FC.x * resX, FC.y * resY - 1), 0).r; 

    col = (surr)/4;
    col = clamp(col, 0.0f, 1.0f);

    float dist = 0.0f;
	//set lights
	for(int i = 0; i < 10; i++){
        if(distance(FC, lights[i])<=0.01f){
            col = 1.0f;
        }
	}

    col*= 1-t;

	FragColor = vec4(vec3(col,0.0f,0.0f), 1.0f);
}
