#version 330

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vNormal;

uniform mat4 model_view;
uniform mat4 projection;

// Vector normal i punt a considerar
out vec4 N;
out vec4 P;

void main()
{
    gl_Position = projection*model_view*vPosition;
    gl_Position = gl_Position/gl_Position.w;

    N = vNormal;
    P = vPosition;
}
