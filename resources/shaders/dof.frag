#version 330 core
uniform sampler2D dofSampler;
uniform vec2 dudv;

in vec2 uv;
in vec4 pos;

uniform bool enableDof;

out vec4 fragColor;

vec4 convolve(int kernal_size) {
    vec3 newColor = vec3(0.f);
    for (int i = kernal_size - 1; i >= 0; i--) {
        for (int j = kernal_size - 1; j >= 0; j--) {
            float ku = uv[0] + (i - 1) * dudv[0];
            float kv = uv[1] + (j - 1) * dudv[1];
            newColor += vec3(texture(dofSampler, vec2(ku, kv))) / (kernal_size * kernal_size);
        }
    }
    return vec4(newColor, 1.f);
}

void main() {

if (!enableDof) {
    fragColor = texture(dofSampler, uv);
    return;
}

  vec4 focusColor = texture(dofSampler, uv);

  vec2 focusPoint = vec2(0.5f);

  float blur = smoothstep(0.f, 1.f, length(uv - focusPoint));

//  vec4 outOfFocusColor = convolve(7);
//  fragColor = (1.f - blur) * focusColor + blur * outOfFocusColor;

  fragColor = focusColor;
  if (blur > 0.65f) {
//      fragColor = vec4(1.f);
      fragColor = convolve(7);
  } else if (blur > 0.5f){
//      fragColor = vec4(0.5f);
      fragColor = convolve(5);
  } else if (blur > 0.3f){
//      fragColor = vec4(0.f);
      fragColor = convolve(3);
  }

//  fragColor = vec4(vec3(blur), 1.f);
//  fragColor = mix(focusColor, outOfFocusColor, blur);
}
