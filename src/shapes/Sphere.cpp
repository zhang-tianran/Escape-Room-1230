#include "Sphere.h"
#include "glm/ext/scalar_constants.hpp"

void Sphere::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = std::max(param1, 2);
    m_param2 = std::max(param2, 3);
    setVertexData();
}

void Sphere::makeTile(glm::vec3 topLeft,
                      glm::vec3 topRight,
                      glm::vec3 bottomLeft,
                      glm::vec3 bottomRight) {
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, glm::normalize(topLeft));
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, glm::normalize(topRight));
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, glm::normalize(bottomRight));
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, glm::normalize(topLeft));
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, glm::normalize(bottomRight));
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, glm::normalize(bottomLeft));
}

void Sphere::makeWedge(float currentTheta, float nextTheta) {
    auto const pi = glm::pi<float>();

    auto phiStep = pi / m_param1;

    for (int i = 1; i < m_param1; i++) {
        auto topLeft = glm::vec3(m_radius * sin(i * phiStep) * sin(currentTheta),
                                 m_radius * cos(i * phiStep),
                                 m_radius * sin(i * phiStep) * cos(currentTheta));

        auto topRight = glm::vec3(m_radius * sin((i+1) * phiStep) * sin(nextTheta),
                                  m_radius * cos((i+1) * phiStep),
                                  m_radius * sin((i+1) * phiStep) * cos(nextTheta));

        auto bottomLeft = glm::vec3(m_radius * sin((i-1) * phiStep) * sin(currentTheta),
                                    m_radius * cos((i-1) * phiStep),
                                    m_radius * sin((i-1) * phiStep) * cos(currentTheta));

        auto bottomRight = glm::vec3(m_radius * sin(i * phiStep) * sin(nextTheta),
                                     m_radius * cos(i * phiStep),
                                     m_radius * sin(i * phiStep) * cos(nextTheta));
        makeTile(topLeft, topRight, bottomLeft, bottomRight);
    }
}

void Sphere::makeSphere() {
    float thetaStep = glm::radians(360.f / m_param2);
    for (int i = 0; i < m_param2; i++) {
        makeWedge(thetaStep * (float) i, thetaStep * (float) (i + 1));
    }
}

void Sphere::setVertexData() {
     makeSphere();
}

void Sphere::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
