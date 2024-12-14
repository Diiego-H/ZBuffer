#version 330

#define CENTER vec4(0, 0, 0, 1)
#define RADIUS 150.0f
#define INNER_RADIUS 148.0f
#define SPEED 0.002f

// Vector normal i punt a considerar
in vec4 N;
in vec4 P;

// coordenades de textura
in vec2 texCoord;

uniform sampler2D texMap;

// LookFrom
uniform vec4 obs;

// Llums
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
uniform vec3 ambientGlobalLight;

// Material
#define NUM_TOON 4
struct Material {
    vec3 ka;
    vec3 kd;
    vec3 kdToon[NUM_TOON];
    vec3 ks;
    float shininess;
    float opacity;
};
uniform Material material;

// Color d'entrada
in vec4 colorGouraud;

// Temps
uniform int time;

// mida viewport
uniform vec2 windowSize;

// Color de sortida
out vec4 colorOut;

void main()
{

    vec2 center = vec2(windowSize.x/2, windowSize.y/2);

    float dist = length(gl_FragCoord.xy - center);
    
    // Gouraud si estem fora de l'esfera
    if (dist > RADIUS) {
        colorOut = clamp(colorGouraud, 0, 1);
    } 
    else if( dist < RADIUS && dist > INNER_RADIUS ) {
        colorOut = vec4(1.0, 1.0, 0.0, 1.0);
    } 
    // Gouraud ponderat amb textures + sèpia
    else {
        vec2 uTexCoord = vec2(texCoord.x + time * SPEED, texCoord.y);
        vec4 c = 0.5*colorGouraud + 0.5*vec4(texture(texMap, uTexCoord).rgb, 1.0);
        float sr = c.r * 0.393 + 
            c.b * 0.769 + c.g * 0.189;
        float sg = c.r * 0.349 +
             c.b * 0.686 + c.g * 0.168;
        float sb = c.r * 0.272 + 
            c.b * 0.534 + c.g * 0.131;
        
        colorOut = clamp(vec4(vec3(sr,sg,sb), 1.0), 0, 1);
    }
}
