#version 450
layout(location = 0) out vec2 tc;

void main() {
    vec2 pos = vec2((gl_VertexID << 1) & 2, gl_VertexID & 2);
    tc = pos;
    gl_Position = vec4(pos * 2.0 - 1.0, 0.0, 1.0);
}
