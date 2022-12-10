#version 330 core
// FBO
uniform vec2 dudv;

in vec2 uv;

uniform sampler2D sampler;

out vec4 fragColor;

void main()
{
    fragColor = texture(sampler, uv);
}
