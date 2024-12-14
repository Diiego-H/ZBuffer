#version 330

in vec4 color;
in vec2 texCoord;
out vec4 colorOut;

uniform sampler2D texMap;

void main()
{
    colorOut = vec4(texture(texMap, texCoord).rgb, 1.0);
}
