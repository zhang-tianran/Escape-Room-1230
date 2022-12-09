#include "Cube.h"

void Cube::updateParams(int param1) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    setVertexData();
}

void Cube::makeTile(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
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

void Cube::makeFace(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    glm::vec3 w = (topRight - topLeft) / (float) m_param1;
    glm::vec3 h = (bottomLeft - topLeft) / (float) m_param1;
    for (int i = 0; i < m_param1; i++) {
        for (int j = 0; j < m_param1; j++) {
            makeTile(topLeft + w * (float) i + h * (float) j,
                     topLeft + w * (float)(i+1) + h * (float)(j),
                     topLeft + w * (float)(i) + h * (float)(j+1),
                     topLeft + w * (float)(i+1) + h * (float)(j+1));
        }
    }
}

void Cube::setVertexData() {
     makeFace(glm::vec3(-0.5f,  0.5f, 0.5f),
              glm::vec3( 0.5f,  0.5f, 0.5f),
              glm::vec3(-0.5f, -0.5f, 0.5f),
              glm::vec3( 0.5f, -0.5f, 0.5f));

     makeFace(glm::vec3( 0.5f,  0.5f, -0.5f),
              glm::vec3(-0.5f,  0.5f, -0.5f),
              glm::vec3( 0.5f, -0.5f, -0.5f),
              glm::vec3(-0.5f, -0.5f, -0.5f));

     makeFace(glm::vec3(0.5f,  0.5f, -0.5f),
              glm::vec3(0.5f, -0.5f, -0.5f),
              glm::vec3(0.5f,  0.5f,  0.5f),
              glm::vec3(0.5f, -0.5f,  0.5f));

     makeFace(glm::vec3(-0.5f, -0.5f, -0.5f),
              glm::vec3(-0.5f,  0.5f, -0.5f),
              glm::vec3(-0.5f, -0.5f,  0.5f),
              glm::vec3(-0.5f,  0.5f,  0.5f));

     makeFace(glm::vec3(-0.5f, 0.5f, -0.5f),
              glm::vec3( 0.5f, 0.5f, -0.5f),
              glm::vec3(-0.5f, 0.5f,  0.5f),
              glm::vec3( 0.5f, 0.5f,  0.5f));

     makeFace(glm::vec3( 0.5f, -0.5f, -0.5f),
              glm::vec3(-0.5f, -0.5f, -0.5f),
              glm::vec3( 0.5f, -0.5f,  0.5f),
              glm::vec3(-0.5f, -0.5f,  0.5f));
}

void Cube::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
