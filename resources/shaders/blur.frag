#version 330 core
uniform vec2 dudv;

in vec2 uv;

uniform sampler2D blurSampler;

out vec4 fragColor;

void main()
{
    int kernal_size = 5;
    vec3 newColor = vec3(0.f);
    for (int i = kernal_size - 1; i >= 0; i--) {
        for (int j = kernal_size - 1; j >= 0; j--) {
            float ku = uv[0] + (i - 1) * dudv[0];
            float kv = uv[1] + (j - 1) * dudv[1];
            newColor += vec3(texture(blurSampler, vec2(ku, kv))) / (kernal_size * kernal_size);
        }
    }
    fragColor = vec4(newColor, 1.f);
}
