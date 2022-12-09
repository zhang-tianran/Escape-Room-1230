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

    glm::mat4 getViewMatrix() { return view; };
    glm::mat4 getViewInverse() { return viewInverse; };
    glm::mat4 getProjection() { return projection; };

    glm::vec3 look;
    glm::vec3 up;
    glm::vec3 pos;

private:

    float m_far;
    float m_near;
    float m_aspectRatio;
    float heightAngle;

    glm::mat4 view;
    glm::mat4 viewInverse;
    glm::mat4 projection;

    void calcView();
    void calcProj();
};
