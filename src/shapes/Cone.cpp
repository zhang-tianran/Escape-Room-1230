#include "Cone.h"

void Cone::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = param2;
    setVertexData();
}

// Create one tile for the side
void Cone::makeConeTile(glm::vec3 topLeft,
              glm::vec3 topRight,
              glm::vec3 bottomLeft,
              glm::vec3 bottomRight){

    glm::vec3 normTopLeft = glm::normalize(glm::vec3{2.f * topLeft[0], 0.25f - 0.5f * topLeft[1], 2.f * topLeft[2]});
    glm::vec3 normTopRight = glm::normalize(glm::vec3{2.f * topRight[0], 0.25f - 0.5f * topRight[1], 2.f * topRight[2]});
    glm::vec3 normBottomLeft = glm::normalize(glm::vec3{2.f * bottomLeft[0], 0.25f - 0.5f * bottomLeft[1], 2.f * bottomLeft[2]});
    glm::vec3 normBottomRight = glm::normalize(glm::vec3{2.f * bottomRight[0], 0.25f - 0.5f * bottomRight[1], 2.f * bottomRight[2]});

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

void Cone::makeCircleTile(glm::vec3 topLeft,
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
void Cone::makeCircleWedge(float currentTheta, float nextTheta, float y){
    // phi = atan(0.5f / m_param1 * i / 0.5 = i / m_param1)
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

// Make cone tip
void Cone::makeTip(float currentTheta, float nextTheta){
    float y = 1.f / (float) m_param1;
    float step = 2.f * M_PI / float(m_param2) * 0.5f;
    glm::vec3 bottomRight = glm::vec3 {
            atan(0.5) * y * cos(currentTheta),
            0.5f - y,
            atan(0.5) * y * sin(currentTheta)
    };
    glm::vec3 bottomLeft = glm::vec3 {
            atan(0.5) * y * cos(nextTheta),
            0.5f - y,
            atan(0.5) * y * sin(nextTheta)
    };
    glm::vec3 normTopRight = glm::normalize(glm::vec3{cos(currentTheta - step), 0.f, sin(currentTheta - step)});
    glm::vec3 normTopLeft = glm::normalize(glm::vec3{cos(nextTheta - step), 0.f, sin(nextTheta - step)});
    glm::vec3 normBottomLeft = glm::normalize(glm::vec3{2.f * bottomLeft[0], 0.25f - 0.5f * bottomLeft[1], 2.f * bottomLeft[2]});
    glm::vec3 normBottomRight = glm::normalize(glm::vec3{2.f * bottomRight[0], 0.25f - 0.5f * bottomRight[1], 2.f * bottomRight[2]});
    m_vertexData.insert(m_vertexData.end(), {
        0, 0.5f, 0,
        normTopLeft[0], 0.5f, normTopLeft[2],
        1.f, 1.f,
        bottomLeft[0], bottomLeft[1], bottomLeft[2],
        normBottomLeft[0], normBottomLeft[1], normBottomLeft[2],
        1.f, 1.f,
        bottomRight[0], bottomRight[1], bottomRight[2],
        normBottomRight[0], normBottomRight[1], normBottomRight[2],
        1.f, 1.f,
        bottomRight[0], bottomRight[1], bottomRight[2],
        normBottomRight[0], normBottomRight[1], normBottomRight[2],
        1.f, 1.f,
        0, 0.5f, 0,
        normTopRight[0], 0.5f, normTopRight[2],
        1.f, 1.f,
        0, 0.5f, 0,
        normTopLeft[0], 0.5f, normTopLeft[2],
        1.f, 1.f,
    });
}

void Cone::makeConeWedge(float currentTheta, float nextTheta){
    float y = 1.f / (float) m_param1;
    makeTip(currentTheta, nextTheta);
    for (int i = 1; i < m_param1; i++) {
        float r1 = atan(0.5) * y * i;
        float r2 = atan(0.5) * y * (i+1);
        glm::vec3 topLeft = glm::vec3 {
                r1 * cos(currentTheta),
                0.5f - y * i,
                r1 * sin(currentTheta)
        };
        glm::vec3 topRight = glm::vec3 {
                r1 * cos(nextTheta),
                0.5f - y * i,
                r1 * sin(nextTheta)
        };
        glm::vec3 bottomLeft = glm::vec3 {
                r2 * cos(currentTheta),
                0.5f - y * (i+1),
                r2 * sin(currentTheta)
        };
        glm::vec3 bottomRight = glm::vec3 {
                r2 * cos(nextTheta),
                0.5f - y * (i+1),
                r2 * sin(nextTheta)
        };
        makeConeTile(topRight, topLeft, bottomRight, bottomLeft);
    }
}

void Cone::makeCone() {
    float theta = glm::radians(360.f / m_param2);
    for (int i = 0; i < m_param2; i++) {
        makeCircleWedge(theta * (i + 1), theta * (i), -0.5f);
        makeConeWedge(theta * i + glm::radians(90.f), theta * (i + 1) + glm::radians(90.f));
    }
}


void Cone::setVertexData() {
    makeCone();
}

void Cone::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
