
#version 460 core
layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 outColor;

layout(location = 0) uniform float time;
layout(location = 1) uniform vec2 resolution;

float hash(vec2 p)
{
    return fract(sin(dot(p,vec2(127.1,311.7))) * 43758.5453);
}

void main()
{
    vec2 px = uv * resolution;
    float n = hash(px + time*100.0);
    outColor = vec4(vec3(n),1.0);
}
