#version 330

// Vector normal i punt a considerar
in vec4 N;
in vec4 P;

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

// Color de sortida
out vec4 colorOut;

vec4 vectorL(int i) {
    vec4 L;
    int type = lights[i].type;

    // Càlcul de L segons tipus de llum
    if (type == POINTLIGHT) {
        L = normalize(lights[i].pos - P);
    } else if (type == DIRECTIONALLIGHT) {
        // Vector direction normalitzat
        L = -lights[i].direction;
    } else if (type == SPOTLIGHT) {
        L = normalize(lights[i].pos - P);

        // Comprovem si li dona llum al punt
        if (dot(-L, lights[i].direction) < cos(lights[i].angle)) {
            L = vec4(0,0,0,0);
        }
    } else {
        L = vec4(0,0,0,0);
    }

    return L;
}

void main()
{
    vec3 color = vec3(0,0,0);

    // Vector del punt a l'observador
    vec4 V = normalize(obs - P);

    for (int i = 0; i < NUM_LIGHTS; i++) {
        vec3 lightColor = vec3(0,0,0);

        // Vector del punt a la llum
        vec4 L = vectorL(i);

        // Discretització intensitat
        float cosDif = max(dot(L, N), 0.0);

        float cosDifNormalVision = dot(N, V);

        int j;
        if (cosDif > 0.95) { j=0; }
        else if (cosDif > 0.5) { j=1; }
        else if (cosDif > 0.25) { j=2; }
        else { j=3; }

        if (cosDifNormalVision < 0.00 || cosDifNormalVision > 0.20){
            // Component difosa
            lightColor += material.kdToon[j] * lights[i].Id;

            // Component ambient
            lightColor += material.ka * lights[i].Ia;
        }

        else{
            // Component difosa només (èmfasi silueta)
            lightColor += material.kdToon[j] * lights[i].Id * (1-cosDifNormalVision);
        }

        color += lightColor;
    }

    // Llum global
    color += ambientGlobalLight * material.ka;

    // Carreguem el color final
    colorOut = vec4(color, 1);
}
