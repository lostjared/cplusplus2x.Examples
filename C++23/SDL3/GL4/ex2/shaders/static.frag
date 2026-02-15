#version 460 core
layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 outColor;

layout(location = 0) uniform float time;
layout(location = 1) uniform vec2 resolution;

float hash(vec2 p)
{
    return fract(sin(dot(p, vec2(127.1,311.7))) * 43758.5453);
}

float hash13(vec3 p)
{
    return fract(sin(dot(p, vec3(127.1,311.7, 74.7))) * 43758.5453);
}

void main()
{
    vec2 px = uv * resolution;

    float refreshHz = 240.0;
    float frame = floor(time * refreshHz);

    float snow = hash13(vec3(px, frame));

    float clump = hash(vec2(floor(px.x * 0.25), floor(px.y * 0.25)) + frame * 0.01);
    clump = mix(0.4, 1.2, clump);

    float bands = sin((uv.y + time * 1.5) * resolution.y * 0.15) * 0.12;
    float scan  = sin(uv.y * resolution.y * 1.5 + time * 30.0) * 0.03;

    float pump = 0.85 + sin(time * 6.0) * 0.15;
    float rf = sin(uv.y * resolution.y * 0.3 + time * 40.0) * 0.08;

    vec2 c = uv * 2.0 - 1.0;
    float vignette = 1.0 - dot(c, c) * 0.25;

    float v = snow * clump + bands + scan + rf;
    v *= pump;
    v *= vignette;

    outColor = vec4(vec3(v), 1.0);
}

