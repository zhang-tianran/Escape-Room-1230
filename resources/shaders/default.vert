#version 330 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 objPosition;
layout(location = 1) in vec3 objNormal;

out vec4 wrldPosition;
out vec3 wrldNormal;

uniform mat4 model;
uniform mat3 invTModel;

uniform mat4 view;
uniform mat4 proj;

void main() {
    wrldPosition = model * vec4(objPosition, 1);
    wrldNormal = invTModel * objNormal;

    gl_Position = proj * view * model * vec4(objPosition, 1);
}
