#version 330 core

in vec2 uvCoordinate;

uniform sampler2D sampler;

uniform int fboWidth;
uniform int fboHeight;

out vec4 fragColor;

void main()
{
    fragColor = texture(sampler, uvCoordinate);
}
