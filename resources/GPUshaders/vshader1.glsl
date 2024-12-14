#version 330


layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vColor;

uniform mat4 model_view;
uniform mat4 projection;
uniform vec3 ambientGlobalLight;

out vec4 color;

#define NUM_LIGHTS 1
#define POINTLIGHT 0
#define DIRECTIONALLIGHT 1
#define SPOTLIGHT 2
struct Light {
    int type;
    vec3 Ia;
    vec3 Id;
    vec3 Is;
    vec4 pos;
    vec4 direction;
    float angle;
    float a;
    float b;
    float c;
};
uniform Light lights[NUM_LIGHTS];

void main()
{
    gl_Position = projection*model_view*vPosition;
    gl_Position = gl_Position/gl_Position.w;

    color = vec4(2 - lights[0].type, 30 - lights[0].angle, 1, 1);
}
