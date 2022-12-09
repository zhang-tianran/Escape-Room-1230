#include "sceneparser.h"
#include "scenefilereader.h"
#include "glm/gtx/transform.hpp"

#include <chrono>
#include <memory>
#include <iostream>


void dfs(SceneNode* root, RenderData &renderData, glm::mat4 ctm) {
    if (root == NULL)
        return;

    for (int i = 0; i < root->transformations.size(); i++) {
        SceneTransformation* trans = root->transformations[i];
        switch(trans->type) {
            case TransformationType::TRANSFORMATION_TRANSLATE:
                ctm = glm::translate(ctm, trans->translate);
                break;
            case TransformationType::TRANSFORMATION_SCALE:
                ctm = glm::scale(ctm, trans->scale);
                break;
            case TransformationType::TRANSFORMATION_ROTATE:
                ctm = glm::rotate(ctm, trans->angle, trans->rotate);
                break;
            case TransformationType::TRANSFORMATION_MATRIX:
                ctm = ctm * trans->matrix;
                break;
        }
    }

    for (ScenePrimitive* prim: root->primitives) {
        RenderShapeData data;
        data.ctm = ctm;
        data.ctmInverse = glm::inverse(ctm);
        data.primitive = *prim;
        renderData.shapes.push_back(data);
    }

    for (SceneNode* node: root->children) {
        dfs(node, renderData, ctm);
    }
}

bool SceneParser::parse(std::string filepath, RenderData &renderData) {
    ScenefileReader fileReader = ScenefileReader(filepath);
    bool success = fileReader.readXML();
    if (!success) {
        return false;
    }

    // populate renderData with global data, lights, and camera data
    SceneNode* root = fileReader.getRootNode();
    renderData.cameraData = fileReader.getCameraData();
    renderData.globalData = fileReader.getGlobalData();
    renderData.lights = fileReader.getLights();
    // populate renderData's list of primitives and their transforms.
    renderData.shapes.clear();
    glm::mat4 m(1.0f);
    dfs(root, renderData, m);
    return true;
}
