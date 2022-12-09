#include "camera.h"
#include "glm/gtx/transform.hpp"
#include <iostream>


void Camera::setCamData(SceneCameraData &cameraData) {
//    m_cameraData = cameraData;
    look = glm::vec3(cameraData.look);
    up = glm::vec3(cameraData.up);
    pos = glm::vec3(cameraData.pos);
    heightAngle = cameraData.heightAngle;
    Camera::calcView();
}

void Camera::setCamWindow(float far, float near, float aspectRatio) {
    m_far = far;
    m_near = near;
    m_aspectRatio = aspectRatio;
    Camera::calcProj();
}

void Camera::setCamPos(glm::vec3 translation) {
    pos += translation;
    calcView();
}

void Camera::setCamRotate(glm::vec3 axis, float angle) {
    glm::mat3 rotation = glm::mat3(cos(angle) + (axis[0] * axis[0]) * (1.f - cos(angle)), axis[0] * axis[1] * (1.f - cos(angle)) + axis[2] * sin(angle), axis[0] * axis[2] * (1.f - cos(angle)) - axis[1] * sin(angle),
                                   axis[0] * axis[1] * (1.f - cos(angle)) - axis[2] * sin(angle), cos(angle) + (axis[1] * axis[1]) * (1.f - cos(angle)), axis[1] * axis[2] * (1.f - cos(angle)) + axis[0] * sin(angle),
                                   axis[0] * axis[2] * (1.f - cos(angle)) + axis[1] * sin(angle), axis[1] * axis[2] * (1.f - cos(angle)) - axis[0] * sin(angle), cos(angle) + (axis[2] * axis[2]) * (1.f - cos(angle)));
    look = rotation * look;
    calcView();
}

void Camera::calcView() {
    glm::mat4 t = glm::mat4(1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 1, 0,
                            -pos[0], -pos[1], -pos[2], 1);

    glm::vec3 w = - glm::normalize(look);
    glm::vec3 v = glm::normalize(up - glm::dot(up, w) * w);
    glm::vec3 u = glm::cross(v, w);
    glm::mat4 r = glm::mat4(u[0], v[0], w[0], 0,
                            u[1], v[1], w[1], 0,
                            u[2], v[2], w[2], 0,
                            0, 0, 0, 1);
    view = r * t;
    viewInverse = glm::inverse(view);
}

void Camera::calcProj(){
    float widthAngle = m_aspectRatio * heightAngle;
    glm::mat4 glMatrix = glm::mat4(1.f, 0.f, 0.f, 0.f,
                                   0.f, 1.f, 0.f, 0.f,
                                   0.f, 0.f, -2.f, 0.f,
                                   0.f, 0.f, -1.f, 1.f);
    glm::mat4 scale = glm::mat4(1.f / (m_far * tan(widthAngle / 2.f)), 0.f, 0.f, 0.f,
                                0.f, 1.f / (m_far * tan(heightAngle / 2.f)), 0.f, 0.f,
                                0.f, 0.f, 1.f / m_far, 0.f,
                                0.f, 0.f, 0.f, 1.f);
    float c = -m_near / m_far;
    glm::mat4 unhinge = glm::mat4(1.f, 0.f, 0.f, 0.f,
                                  0.f, 1.f, 0.f, 0.f,
                                  0.f, 0.f, 1.f / (1.f + c), -1.f,
                                  0.f, 0.f, -c / (1.f + c), 0.f);
    projection = glMatrix * unhinge * scale;
}
