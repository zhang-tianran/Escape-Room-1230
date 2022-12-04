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
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, glm::cross(topRight - topLeft, topLeft - bottomRight));
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, glm::cross(topRight - topLeft, topLeft - bottomRight));
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, glm::cross(topRight - topLeft, topLeft - bottomRight));
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, glm::cross(topLeft - bottomLeft, topLeft - bottomRight));
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, glm::cross(topLeft - bottomLeft, topLeft - bottomRight));
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, glm::cross(topLeft - bottomLeft, topLeft - bottomRight));
}

void Cube::makeFace(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    auto lrInterval = (topRight - topLeft) / (float) m_param1;
    auto tbInterval = (bottomLeft - topLeft) / (float) m_param1;

    for (int i = 0; i < m_param1; i++) {
        for (int j = 0; j < m_param1; j++) {
            auto tl = topLeft + (float) i * lrInterval + (float) j * tbInterval;
            makeTile(tl, tl + lrInterval,
                     tl + tbInterval, tl + lrInterval + tbInterval);
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

     makeFace(glm::vec3(-0.5f,  0.5f, -0.5f),
              glm::vec3(-0.5f,  0.5f, 0.5f),
              glm::vec3(-0.5f, -0.5f, -0.5f),
              glm::vec3(-0.5f, -0.5f, 0.5f));

     makeFace(glm::vec3(0.5f,  0.5f, 0.5f),
              glm::vec3(0.5f,  0.5f, -0.5f),
              glm::vec3(0.5f, -0.5f, 0.5f),
              glm::vec3(0.5f, -0.5f, -0.5f));

     makeFace(glm::vec3(-0.5f, -0.5f, 0.5f),
              glm::vec3( 0.5f, -0.5f, 0.5f),
              glm::vec3(-0.5f, -0.5f, -0.5f),
              glm::vec3( 0.5f, -0.5f, -0.5f));

     makeFace(glm::vec3(0.5f, 0.5f, 0.5f),
              glm::vec3(-0.5f, 0.5f, 0.5f),
              glm::vec3(0.5f, 0.5f, -0.5f),
              glm::vec3(-0.5f, 0.5f, -0.5f));
}

void Cube::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
