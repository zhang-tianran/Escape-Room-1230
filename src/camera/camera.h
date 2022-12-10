#pragma once

#include <glm/glm.hpp>
#include "utils/scenedata.h"

// A class representing a virtual camera.

class Camera {
public:

    void setCamData(SceneCameraData &cameraData);
    void setCamWindow(float far, float near, float aspectRatio);
    void setCamPos(glm::vec3 translation);
    void setCamRotate(glm::vec3 axis, float angle);

    glm::mat4 getViewMatrix() { return m_view; };
    glm::mat4 getViewInverse() { return m_viewInverse; };
    glm::mat4 getProjection() { return m_projection; };

    glm::vec3 m_look;
    glm::vec3 m_up;
    glm::vec3 m_pos;

private:

    float m_far;
    float m_near;
    float m_aspectRatio;
    float m_heightAngle;

    glm::mat4 m_view;
    glm::mat4 m_viewInverse;
    glm::mat4 m_projection;

    void calcView();
    void calcProj();
};
