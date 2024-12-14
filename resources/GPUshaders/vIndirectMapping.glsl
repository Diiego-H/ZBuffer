#version 330

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vNormal;
layout (location = 2) in vec2 vTexCoord;

uniform mat4 model_view;
uniform mat4 projection;
uniform vec3 ambientGlobalLight;

uniform float radiMax;

#define PI 3.1415926535897932384626433832795f

out vec2 texCoord;

void main()
{
    // compute intersection point in the sphere of radius r with the straight line defined by point gl_position and vnormal vector


    // The line passes through p1 and p2:
    vec3 p1 = vec3(0.0,0.0,0.0);
    vec3 p2 = vPosition.xyz;

    // Sphere center is p3, radius is r:
    vec3 p3 = vec3(0,0,0);
    float r = radiMax;

    // normalized vector from center to vertex
    vec3 d = normalize(p2 - p1);

    float a = dot(d, d);
    float b = 2.0 * dot(d, p1 - p3);
    float c = dot(p3, p3) + dot(p1, p1) - 2.0 * dot(p3, p1) - r*r;

    float test = b*b - 4.0*a*c;

    float val = (-b - sqrt(test)) / (2.0 * a);
    vec3 hitp = normalize(p1 + val * (p2 - p1));
    


    float u = 0.5 - atan(hitp.z, hitp.x)/(2.0*PI);
    float v = 0.5 - asin(hitp.y)/PI;

    gl_Position = projection*model_view*vPosition;
    gl_Position = gl_Position/gl_Position.w;

    texCoord = vec2(u, v);

}
