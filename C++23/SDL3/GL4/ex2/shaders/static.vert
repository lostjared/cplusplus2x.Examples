
#version 460 core
layout(location = 0) out vec2 uv;

void main()
{
    vec2 pos;
    if (gl_VertexID == 0) pos = vec2(-1,-1);
    if (gl_VertexID == 1) pos = vec2( 3,-1);
    if (gl_VertexID == 2) pos = vec2(-1, 3);

    gl_Position = vec4(pos,0,1);
    uv = pos * 0.5 + 0.5;
}
