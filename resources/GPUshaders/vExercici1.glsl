#version 330

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vNormal;
layout (location = 2) in vec2 vTexCoord;

uniform mat4 model_view;
uniform mat4 projection;

// Vector normal i punt a considerar
out vec4 N;
out vec4 P;
out vec2 texCoord;

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

uniform vec4 obs;

uniform sampler2D texMap;

uniform vec2 windowSize;

out vec4 colorGouraud;

vec4 vectorL(int i) {
    vec4 L;
    int type = lights[i].type;

    // Càlcul de L segons tipus de llum
    if (type == POINTLIGHT) {
        L = normalize(lights[i].pos - vPosition);
    } else if (type == DIRECTIONALLIGHT) {
        // Vector direction normalitzat
        L = -lights[i].direction;
    } else if (type == SPOTLIGHT) {
        L = normalize(lights[i].pos - vPosition);

        // Comprovem si li dona llum al punt
        if (dot(-L, lights[i].direction) < cos(lights[i].angle)) {
            L = vec4(0,0,0,0);
        }
    } else {
        L = vec4(0,0,0,0);
    }

    return L;
}

float attenuationFactor(int i, float distance) {
    float attenuationFactor;
    int type = lights[i].type;

    // Càlcul del factor d'atenuacio segons el tipus de llum
    if (type == POINTLIGHT || type == SPOTLIGHT) {
        attenuationFactor = 1 / (lights[i].a + lights[i].b * distance +
                                 lights[i].c * distance * distance);
    } else if (type == DIRECTIONALLIGHT) {
        attenuationFactor = 1;
    } else {
        attenuationFactor = 1;
    }

    return attenuationFactor;
}

void main()
{
    gl_Position = projection*model_view*vPosition;
    gl_Position = gl_Position/gl_Position.w;
    N = vNormal;
    P = vPosition;
    texCoord = vTexCoord;

    vec3 result = vec3(0.0, 0.0, 0.0);
    vec3 result2 = vec3(0.0, 0.0, 0.0);
    vec3 lightPos;
    vec3 pointPosition = vPosition.xyz;
    vec3 L;
    vec3 V = normalize(obs.xyz - pointPosition);
    // Iterem pels llums.
    for (int i = 0; i < NUM_LIGHTS; i++)
    {
        lightPos = lights[i].pos.xyz;
        
        // Llum difosa.
        L = vectorL(i).xyz;

        // Calculem el cosinus de l'angle entre L i el vector normal al punt de tall.
        // No cal dividir entre el producte de normes ja que els vectors ja estan normalitzats.
        float cosDif = dot(L, vNormal.xyz);

        vec3 resultAtenuable = (lights[i].Id * material.kd * max(cosDif, 0.0f));

        // Si la llum es de tipus Spotlight i hi toca la llum,
        // o si la llum és d'un altre tipus, afegir-hi la component especular.
        if (((lights[i].type == SPOTLIGHT) && (dot(-L, lights[i].direction.xyz) >= cos(lights[i].angle))) ||
            (lights[i].type != SPOTLIGHT)) {
            // Llum especular.
            vec3 H = normalize(L + V);

            // Calculem el cosinus de l'angle entre H i el vector normal al punt de tall.
            // No cal dividir entre el producte de normes ja que els vectors ja estan normalitzats.
            float cosEspec = dot(H, vNormal.xyz);

            resultAtenuable += lights[i].Is * material.ks * pow(max(cosEspec, 0.0f), material.shininess);
        }


        // Atenuacio.
        float lightToPointDistance = sqrt((lightPos.x - pointPosition.x)*(lightPos.x - pointPosition.x) +
                                          (lightPos.y - pointPosition.y)*(lightPos.y - pointPosition.y) +
                                          (lightPos.z - pointPosition.z)*(lightPos.z - pointPosition.z));

        resultAtenuable *= attenuationFactor(i, lightToPointDistance);

        // Llum ambient.
        result += resultAtenuable + (lights[i].Ia * material.ka);
    }

    // Llum global.
    colorGouraud = vec4(result + ambientGlobalLight * material.ka, 1.0);
}
