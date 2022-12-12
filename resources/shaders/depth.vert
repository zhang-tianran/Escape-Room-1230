#version 330 core
layout(location = 0) in vec3 pos;

out vec4 worldPos;

uniform mat4 m_model;

uniform mat4 shadowMatrices[6];

void main() {
    worldPos = m_model * vec4(pos, 1.0);

    gl_Position = shadowMatrices[3] * worldPos;
}
