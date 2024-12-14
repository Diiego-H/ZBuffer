#version 330

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec2 vTexCoord;

uniform mat4 model_view;
uniform mat4 projection;
uniform vec3 ambientGlobalLight;

struct Material {
    vec3 ka;
    vec3 kd;
    vec3 ks;
    float shininess;
    float opacity;
};

uniform Material material;

out vec4 color;
out vec2 texCoord;

void main()
{
    gl_Position = projection*model_view*vPosition;
    gl_Position = gl_Position/gl_Position.w;

    color = vec4(material.kd, 1);
    texCoord = vTexCoord;
}
