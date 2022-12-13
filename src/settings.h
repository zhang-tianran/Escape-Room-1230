#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

struct Settings {
    std::string sceneFilePath;
    std::string objFilePath;
    std::string drawSceneGeometry;
    std::string drawObjMeshes;
    std::string drawShadows = "true";
    int shapeParameter1 = 25;
    int shapeParameter2 = 25;
    float nearPlane = 1;
    float farPlane = 20;
    int mapSize = 2048;
};


// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif // SETTINGS_H
