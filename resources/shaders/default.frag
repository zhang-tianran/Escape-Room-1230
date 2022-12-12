#version 330 core

// Light
struct light
{
    int type; // point: 0; spot: 1; directional: 2
    vec4 color;
    vec4 pos;
    vec4 dir;
    vec3 function; // Attenuation
    float penumbra;
    float angle;
};

uniform light lightArr[8];

// Material
struct Material {
   vec4 cAmbient;
   vec4 cDiffuse;
   vec4 cSpecular;
   float shininess;
};

uniform Material material;

// Coords
uniform vec4 worldCamPos;

in vec4 worldPos;
in vec4 worldNorm;

// Global data
uniform float ka;
uniform float kd;
uniform float ks;

out vec4 fragColor;

in vec2 uvCoordinate;

// For shadows
uniform samplerCube depthMap;
uniform float far_plane;

float shadowValue(vec3 pos) {
    vec3 lightVec = pos - vec3(lightArr[0].pos);
    float closestDepth = texture(depthMap, lightVec).r * far_plane;
    float currentDepth = length(lightVec);
    if (currentDepth - 0.05 > closestDepth) {
        return 1.0;
    } else {
        return 0.0;
    }
}

void main() {

    vec4 worldNormNormalized = normalize(worldNorm);
    vec4 incomingDir = normalize(worldPos - worldCamPos);

    vec4 ambient = ka * material.cAmbient;
    vec4 diffusion = vec4(0.f);
    vec4 specular = vec4(0.f);

    for (int i = 0; i < 8; i++) {

        if (lightArr[i].type > 2) {
            continue;
        }

        // Light direction
        vec4 lightDir;

        if (lightArr[i].type == 2) {
            lightDir = normalize(lightArr[i].dir);
        } else {
            lightDir = normalize(worldPos - lightArr[i].pos);
        }

        // Diffusion and specular
        vec4 temp_d = kd * material.cDiffuse * lightArr[i].color * clamp(dot(-lightDir, worldNormNormalized), 0.0, 1.0);
        vec4 temp_s = vec4(0.f);
        if (material.shininess > 0) {
            vec4 reflectDir = normalize(reflect(-lightDir, worldNormNormalized));
            temp_s += ks * material.cSpecular * lightArr[i].color * pow(clamp(dot(reflectDir, incomingDir), 0.0, 1.0), material.shininess);
        }

        // Attenuation
        if (lightArr[i].type == 0 || lightArr[i].type == 1) {
            float d = distance(worldPos, lightArr[i].pos);
            float f = 1.f / (lightArr[i].function[0] + d * lightArr[i].function[1] + d * d * lightArr[i].function[2]);
            temp_d *= f;
            temp_s *= f;
        }

        // Falloff
        if (lightArr[i].type == 1) {
            float x = abs(acos(dot(lightArr[i].dir, lightDir) / length(lightArr[i].dir) / length(lightDir)));
            float inner = lightArr[i].angle - lightArr[i].penumbra;
            float falloff;
            if (x <= inner){
                falloff = 1.f;
            } else if (x <= lightArr[i].angle) {
                falloff = -2.f * pow((x - inner) / lightArr[i].penumbra, 3) + 3.f * pow((x - inner) / lightArr[i].penumbra, 2);
                falloff = 1.f - falloff;
            } else {
                falloff = 0.f;
            }
            temp_d *= falloff;
            temp_s *= falloff;
        }
        diffusion += temp_d;
        specular += temp_s;
    }
    float shadow = shadowValue(vec3(worldPos));

    vec3 lightVec = vec3(worldPos) - vec3(lightArr[0].pos);
    float closestDepth = texture(depthMap, lightVec).r * far_plane;
    float currentDepth = length(lightVec);
    fragColor = vec4(vec3(currentDepth / far_plane), 1);

//    fragColor = vec4(vec3(ambient + (1.0) * (diffusion + specular)), 1.0);
}
