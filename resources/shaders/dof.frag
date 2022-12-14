#version 330 core
uniform sampler2D dofSampler;
uniform vec2 dudv;

in vec2 uv;
in vec4 pos;

out vec4 fragColor;

void main() {
  float minDistance =  8.0;
  float maxDistance = 12.0;

  vec4 focusColor = texture(dofSampler, uv);

//  if (enabled.x != 1) { return; }
  int kernal_size = 5;
  vec3 newColor = vec3(0.f);
  for (int i = kernal_size - 1; i >= 0; i--) {
      for (int j = kernal_size - 1; j >= 0; j--) {
          float ku = uv[0] + (i - 1) * dudv[0];
          float kv = uv[1] + (j - 1) * dudv[1];
          newColor += vec3(texture(dofSampler, vec2(ku, kv))) / (kernal_size * kernal_size);
      }
  }
  vec4 outOfFocusColor = vec4(newColor, 1.f);

  vec2 focusPoint = vec2(0.5f);
//  float blur = smoothstep(minDistance, maxDistance, length(uv - focusPoint));

//  fragColor  = mix(focusColor, outOfFocusColor, blur);
//  fragColor = vec4(p);
  float blur = length(uv - focusPoint);
//  fragColor = (1.f - blur) * focusColor + blur * outOfFocusColor;

//  fragColor = focusColor + ease * (outOfFocusColor - focusColor);

  fragColor = focusColor;
  fragColor = outOfFocusColor;
  if (blur < 0.3f) {
      fragColor = focusColor;
  } else {
      fragColor = outOfFocusColor;
  }
//  fragColor = vec4(blur);
}
