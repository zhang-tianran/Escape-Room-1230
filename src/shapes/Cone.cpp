#include "Cone.h"

void Cone::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = std::max(param2, 3);
    setVertexData();
}


void Cone::makeBottomTile(float currentTheta, float nextTheta) {
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

void Cone::makeSideTile(float currentTheta, float nextTheta) {
    float step = 0.5 / m_param1;

    float midpoint = (currentTheta + nextTheta) / 2;

    insertVec3(m_vertexData, glm::vec3(0, 0.5, 0));
    insertVec3(m_vertexData, glm::normalize(glm::vec3(2 * sin(midpoint), 1, 2 * cos(midpoint))));
    insertVec3(m_vertexData, glm::vec3(step * sin(currentTheta), 0.5 - 2 * step, step * cos(currentTheta)));
    insertVec3(m_vertexData, glm::normalize(glm::vec3(2 * step * sin(currentTheta), step, 2 * step * cos(currentTheta))));
    insertVec3(m_vertexData, glm::vec3(step * sin(nextTheta), 0.5 - 2 * step, step * cos(nextTheta)));
    insertVec3(m_vertexData, glm::normalize(glm::vec3(2 * step * sin(nextTheta), step, 2 * step * cos(nextTheta))));

    for (int i = 1; i < m_param1; i++) {
        float height1 = 0.5 - 2 * step * i;
        float height2 = 0.5 - 2 * step * (i + 1);

        float radius1 = step * i;
        float radius2 = step * (i + 1);

        insertVec3(m_vertexData, glm::vec3(radius1 * sin(currentTheta), height1, radius1 * cos(currentTheta)));
        insertVec3(m_vertexData, glm::normalize(glm::vec3(2 * radius1 * sin(currentTheta), 0.25 - 0.5 * height1, 2 * radius1 * cos(currentTheta))));

        insertVec3(m_vertexData, glm::vec3(radius2 * sin(currentTheta), height2, radius2 * cos(currentTheta)));
        insertVec3(m_vertexData, glm::normalize(glm::vec3(2 * radius2 * sin(currentTheta), 0.25 - 0.5 * height2, 2 * radius2 * cos(currentTheta))));

        insertVec3(m_vertexData, glm::vec3(radius1 * sin(nextTheta), height1, radius1 * cos(nextTheta)));
        insertVec3(m_vertexData, glm::normalize(glm::vec3(2 * radius1 * sin(nextTheta), 0.25 - 0.5 * height1, 2 * radius1 * cos(nextTheta))));

        insertVec3(m_vertexData, glm::vec3(radius2 * sin(currentTheta), height2, radius2 * cos(currentTheta)));
        insertVec3(m_vertexData, glm::normalize(glm::vec3(2 * radius2 * sin(currentTheta), 0.25 - 0.5 * height2, 2 * radius2 * cos(currentTheta))));

        insertVec3(m_vertexData, glm::vec3(radius2 * sin(nextTheta), height2, radius2 * cos(nextTheta)));
        insertVec3(m_vertexData, glm::normalize(glm::vec3(2 * radius2 * sin(nextTheta), 0.25 - 0.5 * height2, 2 * radius2 * cos(nextTheta))));

        insertVec3(m_vertexData, glm::vec3(radius1 * sin(nextTheta), height1, radius1 * cos(nextTheta)));
        insertVec3(m_vertexData, glm::normalize(glm::vec3(2 * radius1 * sin(nextTheta), 0.25 - 0.5 * height1, 2 * radius1 * cos(nextTheta))));
    }
}

void Cone::setVertexData() {
    float thetaStep = glm::radians(360.f / m_param2);
    for (int i = 0; i < m_param2; i++) {
        makeBottomTile(thetaStep * (float) i, thetaStep * (float) (i + 1));
        makeSideTile(thetaStep * (float) i, thetaStep * (float) (i + 1));
    }
}


// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Cone::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
