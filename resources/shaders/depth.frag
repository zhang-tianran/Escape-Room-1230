#version 330 core
in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

out vec4 fragColor;

void main()
{
    float lightDistance = length(FragPos.xyz - lightPos);

    lightDistance = lightDistance / far_plane;

    gl_FragDepth = lightDistance;

    fragColor = vec4(lightDistance);
}
