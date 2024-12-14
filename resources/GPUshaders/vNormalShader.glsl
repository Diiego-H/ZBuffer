#version 330

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vNormal;

uniform mat4 model_view;
uniform mat4 projection;
uniform vec3 ambientGlobalLight;

out vec4 normal;

void main()
{
    gl_Position = projection*model_view*vPosition;
    gl_Position = gl_Position/gl_Position.w;
    
    // portar les components a [0,1]
    normal = 0.5f * (vNormal + 1.0f);
}
