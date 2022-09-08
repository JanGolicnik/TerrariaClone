#version 430 core
const float PI = 3.1415926535;

out vec4 FragColor;

uniform float bloom_spread = 1;
uniform float bloom_ratio = 0.2;
uniform float bloom_intensity = 1.5;
layout (location = 5) uniform sampler2D txrmap;
in vec2 t1;

void main()
{
	ivec2 size = textureSize(txrmap, 0);
    float uv_x = t1.x * size.x;
    float uv_y = t1.y * size.y;

    vec4 sum = vec4(0.0);
    for (int n = 0; n < 9; ++n) {
        uv_y = (t1.y * size.y) + (bloom_spread * float(n - 4));
        vec4 h_sum = vec4(0.0);
        h_sum += texelFetch(txrmap, ivec2(uv_x - (4.0 * bloom_spread), uv_y), 0);
        h_sum += texelFetch(txrmap, ivec2(uv_x - (3.0 * bloom_spread), uv_y), 0);
        h_sum += texelFetch(txrmap, ivec2(uv_x - (2.0 * bloom_spread), uv_y), 0);
        h_sum += texelFetch(txrmap, ivec2(uv_x - bloom_spread, uv_y), 0);
        h_sum += texelFetch(txrmap, ivec2(uv_x, uv_y), 0);
        h_sum += texelFetch(txrmap, ivec2(uv_x + bloom_spread, uv_y), 0);
        h_sum += texelFetch(txrmap, ivec2(uv_x + (2.0 * bloom_spread), uv_y), 0);
        h_sum += texelFetch(txrmap, ivec2(uv_x + (3.0 * bloom_spread), uv_y), 0);
        h_sum += texelFetch(txrmap, ivec2(uv_x + (4.0 * bloom_spread), uv_y), 0);
        sum += h_sum / 9.0;
    }

    FragColor = ((1-bloom_ratio)*texture(txrmap, t1)) + (sum/9 * bloom_ratio * bloom_intensity);
  /*
  vec2 uv;
  vec2 xy = 2.0 * t1.xy - 1.0;
  float d = length(xy);
  d = length(xy);
  float z = sqrt(1.0 - d * d);
  float r = atan(d, z) / PI;
  float phi = atan(xy.y, xy.x);
  uv.x = r * cos(phi) + 0.5;
  uv.y = r * sin(phi) + 0.5;
  vec4 c = texture2D(txrmap, uv);
  FragColor = c;
  */
  //FragColor = texture(txrmap, t1);
}