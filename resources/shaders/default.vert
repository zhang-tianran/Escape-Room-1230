#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec4 color;

out vec4 worldPos;
out vec4 worldNorm;
out vec4 matDiffuse;

uniform mat4 m_model;
uniform mat3 m_norm;

uniform mat4 m_view;
uniform mat4 m_proj;

void main() {
    // To world space
    worldPos = m_model * vec4(pos, 1.0);
    worldNorm = vec4(m_norm * normalize(norm), 0.0);
    matDiffuse = color;

    gl_Position = m_proj * m_view * worldPos;
}
