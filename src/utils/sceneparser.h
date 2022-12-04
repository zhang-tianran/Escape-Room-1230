#pragma once

#include "scenedata.h"
#include <vector>
#include <string>

// Struct which contains data for a single primitive, to be used for rendering
struct RenderShapeData {
    ScenePrimitive primitive;
    glm::mat4 ctm; // the cumulative transformation matrix
    glm::mat3 invTCtm; // the cumulative transformation matrix
};

// Struct which contains all the data needed to render a scene
struct RenderData {
    SceneGlobalData globalData;
    SceneCameraData cameraData;

    std::vector<SceneLightData> lights;
    std::vector<RenderShapeData> shapes;
};

class SceneParser {
public:
    // Parse the scene and store the results in renderData.
    // @param filepath    The path of the scene file to load.
    // @param renderData  On return, this will contain the metadata of the loaded scene.
    // @return            A boolean value indicating whether the parse was successful.
    static bool parse(std::string filepath, RenderData &renderData, float nearPlane, float farPlane, float aspectRatio);
    static glm::mat4 getProjectionMatrix(float nearPlane, float farPlane, float heightAngle, float aspectRatio);
    static glm::mat4 getViewMatrix(glm::vec4 pos, glm::vec4 look, glm::vec4 up);
};

