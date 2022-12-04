#include "Cylinder.h"

void Cylinder::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = std::max(param2, 3);
    setVertexData();
}

void Cylinder::makeCapTile(float currentTheta, float nextTheta) {
    auto step = 0.5 / m_param1;

    insertVec3(m_vertexData, glm::vec3(0, 0.5, 0));
    insertVec3(m_vertexData, glm::vec3(0, 1, 0));
    insertVec3(m_vertexData, glm::vec3(step * sin(currentTheta), 0.5, step * cos(currentTheta)));
    insertVec3(m_vertexData, glm::vec3(0, 1, 0));
    insertVec3(m_vertexData, glm::vec3(step * sin(nextTheta), 0.5, step * cos(nextTheta)));
    insertVec3(m_vertexData, glm::vec3(0, 1, 0));

    for (int i = 1; i < m_param1; i++) {
        auto radius1 = step * i;
        auto radius2 = step * (i + 1);

        insertVec3(m_vertexData, glm::vec3(radius1 * sin(currentTheta), 0.5, radius1 * cos(currentTheta)));
        insertVec3(m_vertexData, glm::vec3(0, 1, 0));
        insertVec3(m_vertexData, glm::vec3(radius2 * sin(currentTheta), 0.5, radius2 * cos(currentTheta)));
        insertVec3(m_vertexData, glm::vec3(0, 1, 0));
        insertVec3(m_vertexData, glm::vec3(radius1 * sin(nextTheta), 0.5, radius1 * cos(nextTheta)));
        insertVec3(m_vertexData, glm::vec3(0, 1, 0));
        insertVec3(m_vertexData, glm::vec3(radius2 * sin(currentTheta), 0.5, radius2 * cos(currentTheta)));
        insertVec3(m_vertexData, glm::vec3(0, 1, 0));
        insertVec3(m_vertexData, glm::vec3(radius2 * sin(nextTheta), 0.5, radius2 * cos(nextTheta)));
        insertVec3(m_vertexData, glm::vec3(0, 1, 0));
        insertVec3(m_vertexData, glm::vec3(radius1 * sin(nextTheta), 0.5, radius1 * cos(nextTheta)));
        insertVec3(m_vertexData, glm::vec3(0, 1, 0));
    }
}

void Cylinder::makeBottomTile(float currentTheta, float nextTheta) {
    auto step = 0.5 / m_param1;

    insertVec3(m_vertexData, glm::vec3(0, -0.5, 0));
    insertVec3(m_vertexData, glm::vec3(0, -1, 0));
    insertVec3(m_vertexData, glm::vec3(step * sin(nextTheta), -0.5, step * cos(nextTheta)));
    insertVec3(m_vertexData, glm::vec3(0, -1, 0));
    insertVec3(m_vertexData, glm::vec3(step * sin(currentTheta), -0.5, step * cos(currentTheta)));
    insertVec3(m_vertexData, glm::vec3(0, -1, 0));

    for (int i = 1; i < m_param1; i++) {
        auto radius1 = step * i;
        auto radius2 = step * (i + 1);

        insertVec3(m_vertexData, glm::vec3(radius1 * sin(nextTheta), -0.5, radius1 * cos(nextTheta)));
        insertVec3(m_vertexData, glm::vec3(0, -1, 0));
        insertVec3(m_vertexData, glm::vec3(radius2 * sin(nextTheta), -0.5, radius2 * cos(nextTheta)));
        insertVec3(m_vertexData, glm::vec3(0, -1, 0));
        insertVec3(m_vertexData, glm::vec3(radius1 * sin(currentTheta), -0.5, radius1 * cos(currentTheta)));
        insertVec3(m_vertexData, glm::vec3(0, -1, 0));
        insertVec3(m_vertexData, glm::vec3(radius2 * sin(nextTheta), -0.5, radius2 * cos(nextTheta)));
        insertVec3(m_vertexData, glm::vec3(0, -1, 0));
        insertVec3(m_vertexData, glm::vec3(radius2 * sin(currentTheta), -0.5, radius2 * cos(currentTheta)));
        insertVec3(m_vertexData, glm::vec3(0, -1, 0));
        insertVec3(m_vertexData, glm::vec3(radius1 * sin(currentTheta), -0.5, radius1 * cos(currentTheta)));
        insertVec3(m_vertexData, glm::vec3(0, -1, 0));
    }
}

void Cylinder::makeSideTile(float currentTheta, float nextTheta) {
    float step = 0.5 / m_param1;

    for (int i = 0; i < m_param1; i++) {
        float offset1 = 2 * step * i;
        float offset2 = 2 * step * (i + 1);

        insertVec3(m_vertexData, glm::vec3(0.5 * sin(currentTheta), 0.5 - offset1, 0.5 * cos(currentTheta)));
        insertVec3(m_vertexData, glm::vec3(sin(currentTheta), 0, cos(currentTheta)));

        insertVec3(m_vertexData, glm::vec3(0.5 * sin(currentTheta), 0.5 - offset2, 0.5 * cos(currentTheta)));
        insertVec3(m_vertexData, glm::vec3(sin(currentTheta), 0, cos(currentTheta)));

        insertVec3(m_vertexData, glm::vec3(0.5 * sin(nextTheta), 0.5 - offset1, 0.5 * cos(nextTheta)));
        insertVec3(m_vertexData, glm::vec3(sin(nextTheta), 0, cos(nextTheta)));

        insertVec3(m_vertexData, glm::vec3(0.5 * sin(currentTheta), 0.5 - offset2, 0.5 * cos(currentTheta)));
        insertVec3(m_vertexData, glm::vec3(sin(currentTheta), 0, cos(currentTheta)));

        insertVec3(m_vertexData, glm::vec3(0.5 * sin(nextTheta), 0.5 - offset2, 0.5 * cos(nextTheta)));
        insertVec3(m_vertexData, glm::vec3(sin(nextTheta), 0, cos(nextTheta)));

        insertVec3(m_vertexData, glm::vec3(0.5 * sin(nextTheta), 0.5 - offset1, 0.5 * cos(nextTheta)));
        insertVec3(m_vertexData, glm::vec3(sin(nextTheta), 0, cos(nextTheta)));
    }
}

void Cylinder::setVertexData() {
    float thetaStep = glm::radians(360.f / m_param2);
    for (int i = 0; i < m_param2; i++) {
        makeCapTile(thetaStep * (float) i, thetaStep * (float) (i + 1));
        makeBottomTile(thetaStep * (float) i, thetaStep * (float) (i + 1));
        makeSideTile(thetaStep * (float) i, thetaStep * (float) (i + 1));
    }
}

void Cylinder::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
