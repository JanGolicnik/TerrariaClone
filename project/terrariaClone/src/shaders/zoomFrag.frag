#version 430 core
const float PI = 3.1415926535;

out vec4 FragColor;

uniform float bloom_spread = 1;
uniform float bloom_ratio = 0.2;
uniform float bloom_intensity = 1.5;
layout (location = 5) uniform sampler2D txrmap;
layout (location = 2) uniform float zoom;

in vec2 t1;

void main()
{
    vec2 t10 = t1 - 0.5;
    t10 /= zoom;
    t10 += 0.5;

    FragColor = texture(txrmap, t10);
}