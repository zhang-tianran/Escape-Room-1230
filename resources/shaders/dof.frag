#version 330 core

//uniform sampler2D positionTexture;
uniform sampler2D focusTexture;
uniform sampler2D outOfFocusTexture;

//uniform vec2 mouseFocusPoint; //TODO
uniform vec2 nearFar;

in vec2 uv;
in vec4 pos;
out vec4 fragColor;

void main() {
  float minDistance =  8.0;
  float maxDistance = 12.0;

//  float far = nearFar.y;

  vec4 focusColor = texture(focusTexture, uv);

  fragColor = focusColor;

//  vec4 position = texture(positionTexture, uv);
  vec4 position = pos;

  if (position.a <= 0) { fragColor = vec4(1.0); return; }

//  vec2 mouseFocusPoint = vec2(0.5f, 0.5f);

  vec4 outOfFocusColor = texture(outOfFocusTexture, uv);
//  vec4 focusPoint = texture(positionTexture, mouseFocusPoint);
  vec4 focusPoint = vec4(0.f);

  float blur = smoothstep(minDistance, maxDistance, length(position - focusPoint));

//  fragColor  = mix(focusColor, outOfFocusColor, blur);
  fragColor = outOfFocusColor;

}
