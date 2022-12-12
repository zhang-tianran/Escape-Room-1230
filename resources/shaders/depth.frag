#version 330 core
//in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

in vec4 worldPos;

out vec4 fragColor;

void main()
{
    float lightDistance = length(vec3(worldPos) - lightPos);

    fragColor = vec4(lightDistance / 25.0f);
}
