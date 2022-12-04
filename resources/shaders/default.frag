#version 330 core

in vec4 wrldPosition;
in vec3 wrldNormal;

out vec4 fragColor;

struct LightData
{
    int lightType; // 1 for point, 2 for directional, 3 for spotlight
    vec4 lightPos;
    vec4 lightDir;

    float lightAngle;
    float penumbra;

    vec4 lightColor;
    vec3 function; // attenuation function
};

uniform float ka;
uniform float kd;
uniform float ks;

uniform vec4 cameraPos;

uniform vec4 materialAmbient;
uniform vec4 materialDiffuse;
uniform vec4 materialSpecular;
uniform float shininess;

uniform LightData lights[8];
uniform int numLights;

void main() {

    fragColor = vec4(0.0);

    fragColor += ka * materialAmbient;

    for (int i = 0; i < 3; i++) {
        LightData light = lights[i];

        vec3 L; float attenuation;

        if (light.lightType == 1) { // point
            L = normalize(vec3(light.lightPos - wrldPosition));
            float dist = distance(light.lightPos, wrldPosition);
            attenuation = min(1, 1.f / (light.function[0] + dist * light.function[1] + pow(dist, 2) * light.function[2]));
        } else if (lights[i].lightType == 2) { // directional
            L = normalize(vec3(-light.lightDir));
            attenuation = 1;
        } else if (lights[i].lightType == 3) { // spotlight
            L = normalize(vec3(light.lightPos - wrldPosition));
            float x = acos(min(1, max(-1, dot(-L, normalize(vec3(light.lightDir))))));
            float outer = light.lightAngle;
            float inner = light.lightAngle - light.penumbra;
            float falloff;
            if (x > outer) falloff = 1;
            else if (x > inner) falloff = -2 * pow((x - inner) / (outer - inner), 3) +
                    3 * pow((x - inner) / (outer - inner), 2);
            else falloff = 0;

            float dist = distance(light.lightPos, wrldPosition);
            attenuation = (1 - falloff) * min(1, 1.f / (light.function[0] + dist * light.function[1] + pow(dist, 2) * light.function[2]));
        }

        vec3 N = normalize(wrldNormal);

        float dotProd = max(dot(N, L), 0);
        fragColor += attenuation * light.lightColor * kd * materialDiffuse * dotProd;

        vec3 R = 2 * dotProd * N - L;
        vec3 V = normalize(vec3(cameraPos - wrldPosition));
        float RV = max(dot(R, V), 0);

        fragColor += attenuation * light.lightColor * ks * materialSpecular * max(pow(RV, shininess), 0);
    }
}
