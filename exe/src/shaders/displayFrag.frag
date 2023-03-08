#version 430 core
const float PI = 3.1415926535;

out vec4 FragColor;

layout (location = 5) uniform sampler2D txrmap;

in vec2 t1;

void main()
{
    FragColor = texture(txrmap, t1);
}