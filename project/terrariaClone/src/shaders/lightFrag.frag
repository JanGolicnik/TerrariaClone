#version 430 core
out vec4 FragColor;

const int numlights = 100; 

struct Light{
    vec2 pos;
    vec3 color;
};

layout(location = 4) uniform Light lightArray[numlights];
layout(location = 2) uniform mat4 uCamtrans;
layout(location = 3) uniform vec2 uRes;

in vec2 t1;

void main()
{
    FragColor = vec4(0.0f,0.0f,0.0f,0.0f);
    vec2 fragpos = ( (gl_FragCoord.xy / (uRes.xy) ) - 0.5f ) * 2;
    vec4 col = vec4(0.0f);
    for(int i = 0; i < numlights; i++){
        vec4 pos = uCamtrans * vec4(lightArray[i].pos, 0, 1);
        float f = (1 - distance(pos.xy, fragpos.xy))/ 2.0f;
        col = max(FragColor, vec4(lightArray[i].color.rgb, pow(clamp(f,0,1),2)));
        //col += vec4(lightArray[i].color.rgb, clamp(f,0,1));
    }
    FragColor = clamp(col, 0.0f, 1.0f);
}