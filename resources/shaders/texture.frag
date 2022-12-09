#version 330 core
// FBO
uniform bool invert;
uniform bool grayscale;
uniform bool sharpenFilter;
uniform bool blurFilter;

uniform vec2 dudv;

in vec2 uv;

uniform sampler2D sampler;

out vec4 fragColor;

const mat3 sharpenKernel = (1.f / 9.f) * mat3(-1.f, -1.f, -1.f,
                                              -1.f, 17.f, -1.f,
                                              -1.f, -1.f, -1.f);

void main()
{
    fragColor = texture(sampler, uv);

    // kernel filter
    if (sharpenFilter) {
        int kernal_size = 3;
        vec3 newColor = vec3(0.f);
        for (int i = kernal_size - 1; i >= 0; i--) {
            for (int j = kernal_size - 1; j >= 0; j--) {
                float ku = uv[0] + (i - 1) * dudv[0];
                float kv = uv[1] + (j - 1) * dudv[1];
                newColor += vec3(texture(sampler, vec2(ku, kv))) * sharpenKernel[i][j];
            }
        }
        fragColor = vec4(newColor, 1.f);
    }

    if (blurFilter) {
        int kernal_size = 5;
        vec3 newColor = vec3(0.f);
        for (int i = kernal_size - 1; i >= 0; i--) {
            for (int j = kernal_size - 1; j >= 0; j--) {
                float ku = uv[0] + (i - 1) * dudv[0];
                float kv = uv[1] + (j - 1) * dudv[1];
                newColor += vec3(texture(sampler, vec2(ku, kv))) / 25.f;
            }
        }
        fragColor = vec4(newColor, 1.f);
    }

    // per pixel
    if (invert) {
        fragColor = 1.f - fragColor;
    }
    if (grayscale) {
        float grayColor = (fragColor[0] + fragColor[1] + fragColor[2]) / 3.f;
        fragColor = vec4(grayColor);
    }
    fragColor[3] = 1.f;
}
