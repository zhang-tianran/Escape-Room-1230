#include "Sphere.h"

void Sphere::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = param2;
    setVertexData();
}

void Sphere::makeTile(glm::vec3 topLeft,
                      glm::vec3 topRight,
                      glm::vec3 bottomLeft,
                      glm::vec3 bottomRight) {
    glm::vec3 normTopLeft = glm::normalize(topLeft);
    glm::vec3 normTopRight = glm::normalize(topRight);
    glm::vec3 normBottomLeft = glm::normalize(bottomLeft);
    glm::vec3 normBottomRight = glm::normalize(bottomRight);
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, normTopLeft);
    m_vertexData.push_back(1.f);
    m_vertexData.push_back(1.f);
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, normBottomLeft);
    m_vertexData.push_back(1.f);
    m_vertexData.push_back(1.f);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, normBottomRight);
    m_vertexData.push_back(1.f);
    m_vertexData.push_back(1.f);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, normBottomRight);
    m_vertexData.push_back(1.f);
    m_vertexData.push_back(1.f);
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, normTopRight);
    m_vertexData.push_back(1.f);
    m_vertexData.push_back(1.f);
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, normTopLeft);
    m_vertexData.push_back(1.f);
    m_vertexData.push_back(1.f);
}

void Sphere::makeWedge(float currentTheta, float nextTheta) {
    float phi = glm::radians(180.f) / (float) m_param1;
    for (int i = 0; i < m_param1; i++) {
        glm::vec3 topLeft = glm::vec3 {
                0.5 * sin(i * phi) * sin(currentTheta),
                0.5 * cos(i * phi),
                0.5 * sin(i * phi) * cos(currentTheta)
        };
        glm::vec3 topRight = glm::vec3 {
                0.5 * sin(i * phi) * sin(nextTheta),
                0.5 * cos(i * phi),
                0.5 * sin(i * phi) * cos(nextTheta)
        };
        glm::vec3 bottomLeft = glm::vec3 {
                0.5 * sin((i+1) * phi) * sin(currentTheta),
                0.5 * cos((i+1) * phi),
                0.5 * sin((i+1) * phi) * cos(currentTheta)
        };
        glm::vec3 bottomRight = glm::vec3 {
                0.5 * sin((i+1) * phi) * sin(nextTheta),
                0.5 * cos((i+1) * phi),
                0.5 * sin((i+1) * phi) * cos(nextTheta)
        };
        makeTile(topLeft, topRight, bottomLeft, bottomRight);
    }
}

void Sphere::makeSphere() {
    float theta = glm::radians(360.f / m_param2);
    for (int i = 0; i < m_param2; i++) {
        makeWedge(theta * i, theta * (i + 1));
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
