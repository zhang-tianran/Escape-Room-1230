#include "sceneparser.h"
#include "scenefilereader.h"
#include "glm/gtx/transform.hpp"

#include <chrono>
#include <memory>
#include <iostream>
#include <QOpenGLWidget>

void populateShapeData(SceneNode *node, glm::mat4 ctm, std::vector<RenderShapeData> &acc) {

    for (auto sceneTransform : node->transformations) {
        switch (sceneTransform->type) {
            case TransformationType::TRANSFORMATION_TRANSLATE:
                ctm = ctm * glm::translate(sceneTransform->translate);
                break;
            case TransformationType::TRANSFORMATION_ROTATE:
                ctm = ctm * glm::rotate(sceneTransform->angle, sceneTransform->rotate);
                break;
            case TransformationType::TRANSFORMATION_SCALE:
                ctm = ctm * glm::scale(sceneTransform->scale);
                break;
            case TransformationType::TRANSFORMATION_MATRIX:
                ctm = ctm * sceneTransform->matrix;
        }
    }
    for (auto primitive : node->primitives) {
        auto shapeData = RenderShapeData{*primitive, ctm, glm::inverse(glm::transpose(glm::mat3(ctm)))};
        acc.push_back(shapeData);
    }
    for (auto child : node->children) {
        populateShapeData(child, ctm, acc);
    }
}

glm::mat4 SceneParser::getViewMatrix(glm::vec4 pos, glm::vec4 look, glm::vec4 up) {
    auto w = -look / glm::length(look);

    auto expr = up - glm::dot(up, w) * w;
    auto v = expr / glm::length(expr);

    auto u = glm::cross(glm::vec3(v), glm::vec3(w));

    auto col1r = glm::vec4(u[0], v[0], w[0], 0);
    auto col2r = glm::vec4(u[1], v[1], w[1], 0);
    auto col3r = glm::vec4(u[2], v[2], w[2], 0);
    auto col4r = glm::vec4(0, 0, 0, 1);
    auto matRotate = glm::mat4(col1r, col2r, col3r, col4r);

    auto col1t = glm::vec4(1, 0, 0, 0);
    auto col2t = glm::vec4(0, 1, 0, 0);
    auto col3t = glm::vec4(0, 0, 1, 0);
    auto col4t = glm::vec4(-pos[0], -pos[1], -pos[2], 1);

    auto matTranslate = glm::mat4(col1t, col2t, col3t, col4t);
    return matRotate * matTranslate;
}

glm::mat4 SceneParser::getProjectionMatrix(float nearPlane, float farPlane, float heightAngle, float aspectRatio) {
    auto c = -nearPlane / farPlane;
    auto unhinging = glm::mat4(glm::vec4(1, 0, 0, 0),
                             glm::vec4(0, 1, 0, 0),
                             glm::vec4(0, 0, 1.f / (1.f + c), -1),
                             glm::vec4(0, 0, - c / (1.f + c), 0));

    auto widthAngle = 2 * atanf(tanf(heightAngle / 2) * aspectRatio);
    auto scaling = glm::mat4(glm::vec4(1.f / (tanf(widthAngle / 2) * farPlane), 0, 0, 0),
                             glm::vec4(0, 1.f / (tanf(heightAngle / 2) * farPlane), 0, 0),
                             glm::vec4(0, 0, 1.f / farPlane, 0),
                             glm::vec4(0, 0, 0, 1));

    auto transformToOpenGL = glm::mat4(glm::vec4(1, 0, 0, 0),
                                       glm::vec4(0, 1, 0, 0),
                                       glm::vec4(0, 0, -2, 0),
                                       glm::vec4(0, 0, -1, 1));

    return transformToOpenGL * unhinging * scaling;
}

bool SceneParser::parse(std::string filepath, RenderData &renderData, float nearPlane, float farPlane, float aspectRatio) {
    ScenefileReader fileReader = ScenefileReader(filepath);
    bool success = fileReader.readXML();
    if (!success) {
        return false;
    }

    renderData.globalData = fileReader.getGlobalData();
    renderData.lights = fileReader.getLights();
    renderData.cameraData = fileReader.getCameraData();
    renderData.cameraData.viewMatrix = getViewMatrix(renderData.cameraData.pos,
                                                     renderData.cameraData.look,
                                                     renderData.cameraData.up);
    renderData.cameraData.inverseViewMatrix = glm::inverse(renderData.cameraData.viewMatrix);

    renderData.cameraData.projectionMatrix = SceneParser::getProjectionMatrix(nearPlane,
                                                                              farPlane,
                                                                              renderData.cameraData.heightAngle,
                                                                              aspectRatio);

    renderData.shapes.clear();

    auto rootNode = fileReader.getRootNode();
    auto ctm = glm::mat4(1.f);
    populateShapeData(rootNode, ctm, renderData.shapes);

    return true;
}
