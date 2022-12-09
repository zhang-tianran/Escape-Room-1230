#include "camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <iostream>


void Camera::setCamData(SceneCameraData &cameraData) {
    m_look = glm::vec3(cameraData.look);
    m_up = glm::vec3(cameraData.up);
    m_pos = glm::vec3(cameraData.pos);
    m_heightAngle = cameraData.heightAngle;
    calcView();
}

void Camera::setCamWindow(float far, float near, float aspectRatio) {
    m_far = far;
    m_near = near;
    m_aspectRatio = aspectRatio;
    calcProj();
}

void Camera::setCamPos(glm::vec3 translation) {
    m_pos += translation;
    calcView();
}

void Camera::setCamRotate(glm::vec3 axis, float angle) {
    glm::mat3 rotation = glm::mat3(cos(angle) + (axis[0] * axis[0]) * (1.f - cos(angle)), axis[0] * axis[1] * (1.f - cos(angle)) + axis[2] * sin(angle), axis[0] * axis[2] * (1.f - cos(angle)) - axis[1] * sin(angle),
                                   axis[0] * axis[1] * (1.f - cos(angle)) - axis[2] * sin(angle), cos(angle) + (axis[1] * axis[1]) * (1.f - cos(angle)), axis[1] * axis[2] * (1.f - cos(angle)) + axis[0] * sin(angle),
                                   axis[0] * axis[2] * (1.f - cos(angle)) + axis[1] * sin(angle), axis[1] * axis[2] * (1.f - cos(angle)) - axis[0] * sin(angle), cos(angle) + (axis[2] * axis[2]) * (1.f - cos(angle)));
    m_look = rotation * m_look;
    calcView();
}

void Camera::calcView() {
    m_view = glm::lookAt(m_pos, m_pos + m_look, m_up);
    m_viewInverse = glm::inverse(m_view);
}

void Camera::calcProj() {
    m_projection = glm::perspective(m_heightAngle, m_aspectRatio, m_near, m_far);
}
