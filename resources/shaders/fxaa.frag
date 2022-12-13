#version 330 core
// FBO
in vec2 uv;

uniform sampler2D fxaaSampler;

out vec4 fragColor;

void main()
{
    fragColor = texture(fxaaSampler, uv);
}
