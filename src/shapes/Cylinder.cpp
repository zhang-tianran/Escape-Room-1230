#include "Cylinder.h"

void Cylinder::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = param2;
    setVertexData();
}

void Cylinder::makeCylinderTile(glm::vec3 topLeft,
              glm::vec3 topRight,
              glm::vec3 bottomLeft,
              glm::vec3 bottomRight){
    glm::vec3 normTopLeft = glm::normalize(glm::vec3{topLeft[0], 0.f, topLeft[2]});
    glm::vec3 normTopRight = glm::normalize(glm::vec3{topRight[0], 0.f, topRight[2]});
    glm::vec3 normBottomLeft = glm::normalize(glm::vec3{bottomLeft[0], 0.f, bottomLeft[2]});
    glm::vec3 normBottomRight = glm::normalize(glm::vec3{bottomRight[0], 0.f, bottomRight[2]});

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

// Create the side of cylinder
void Cylinder::makeCylinderWedge(float currentTheta, float nextTheta){
    float r = 0.5f;
    float y = 1.f / (float) m_param1;
    for (int i = 0; i < m_param1; i++) {
        glm::vec3 topLeft = glm::vec3 {
                r * cos(currentTheta),
                y * i - 0.5f,
                r * sin(currentTheta)
        };
        glm::vec3 topRight = glm::vec3 {
                r * cos(nextTheta),
                y * i - 0.5f,
                r * sin(nextTheta)
        };
        glm::vec3 bottomLeft = glm::vec3 {
                r * cos(currentTheta),
                y * (i+1) - 0.5f,
                r * sin(currentTheta)
        };
        glm::vec3 bottomRight = glm::vec3 {
                r * cos(nextTheta),
                y * (i+1) - 0.5f,
                r * sin(nextTheta)
        };
        makeCylinderTile(topLeft, topRight, bottomLeft, bottomRight);
    }
}

void Cylinder::makeCircleTile(glm::vec3 topLeft,
              glm::vec3 topRight,
              glm::vec3 bottomLeft,
              glm::vec3 bottomRight){
    glm::vec3 norm1 = glm::normalize(glm::cross((topLeft - topRight), (bottomRight - topRight)));
    glm::vec3 norm2 = glm::normalize(glm::cross((bottomLeft - topLeft), (bottomRight - bottomLeft)));
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, norm1);
    m_vertexData.push_back(1.f);
    m_vertexData.push_back(1.f);
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, norm1);
    m_vertexData.push_back(1.f);
    m_vertexData.push_back(1.f);
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, norm1);
    m_vertexData.push_back(1.f);
    m_vertexData.push_back(1.f);
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, norm2);
    m_vertexData.push_back(1.f);
    m_vertexData.push_back(1.f);
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, norm2);
    m_vertexData.push_back(1.f);
    m_vertexData.push_back(1.f);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, norm2);
    m_vertexData.push_back(1.f);
    m_vertexData.push_back(1.f);
}

// Create top / bottom circle plane
void Cylinder::makeCircleWedge(float currentTheta, float nextTheta, float y){
    for (int i = 0; i < m_param1; i++) {
        float phi1 = atan((float) i / (float) m_param1);
        float phi2 = atan((float) (i+1) / (float) m_param1);
        float r1 = 0.5f / cos(phi1);
        float r2 = 0.5f / cos(phi2);
        glm::vec3 topLeft = glm::vec3 {
                r1 * sin(phi1) * sin(currentTheta),
                y,
                r1 * sin(phi1) * cos(currentTheta)
        };
        glm::vec3 topRight = glm::vec3 {
                r1 * sin(phi1) * sin(nextTheta),
                y,
                r1 * sin(phi1) * cos(nextTheta)
        };
        glm::vec3 bottomLeft = glm::vec3 {
                r2 * sin(phi2) * sin(currentTheta),
                y,
                r2 * sin(phi2) * cos(currentTheta)
        };
        glm::vec3 bottomRight = glm::vec3 {
                r2 * sin(phi2) * sin(nextTheta),
                y,
                r2 * sin(phi2) * cos(nextTheta)
        };
        makeCircleTile(topLeft, topRight, bottomLeft, bottomRight);
    }
}


void Cylinder::makeCylinder() {
    float theta = glm::radians(360.f / m_param2);
    for (int i = 0; i < m_param2; i++) {
        makeCircleWedge(theta * i, theta * (i + 1), 0.5f);
        makeCircleWedge(theta * (i + 1), theta * (i), -0.5f);
        makeCylinderWedge(theta * i + glm::radians(90.f), theta * (i + 1) + glm::radians(90.f));
    }

}

void Cylinder::setVertexData() {
    makeCylinder();
}

void Cylinder::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
