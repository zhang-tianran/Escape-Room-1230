#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

struct Settings {
    std::string sceneFilePath;
    int shapeParameter1 = 25;
    int shapeParameter2 = 25;
    float nearPlane = 0.01;
    float farPlane = 100;
    int mapSize = 256;
};


// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif // SETTINGS_H
