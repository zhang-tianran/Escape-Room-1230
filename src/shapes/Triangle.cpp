#include "Triangle.h"

void Triangle::updateParams() {
    m_vertexData = std::vector<float>();
    setVertexData();
}

void Triangle::setVertexData() {
   insertVec3(m_vertexData, glm::normalize(glm::vec3(0.0, 0.0, 1.0)));
   insertVec3(m_vertexData, glm::vec3(-0.5, -0.5, 0.0));
   insertVec3(m_vertexData, glm::normalize(glm::vec3(0.0, 0.0, 1.0)));
   insertVec3(m_vertexData, glm::vec3(0.5, -0.5, 0.0));
   insertVec3(m_vertexData, glm::normalize(glm::vec3(0.0, 0.0, 1.0)));
}

void Triangle::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
