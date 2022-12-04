#pragma once

// Defined before including GLEW to suppress deprecation messages on macOS
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <unordered_map>
#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QTime>
#include <QTimer>

#include "utils/sceneparser.h"

class Realtime : public QOpenGLWidget
{
public:
    Realtime(QWidget *parent = nullptr);
    void finish();                                      // Called on program exit
    void sceneChanged();
    void settingsChanged();

public slots:
    void tick(QTimerEvent* event);                      // Called once per tick of m_timer

protected:
    void initializeGL() override;                       // Called once at the start of the program
    void paintGL() override;                            // Called whenever the OpenGL context changes or by an update() request
    void resizeGL(int width, int height) override;      // Called when window size changes

private:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

    void updateSphereData(int shapeParam1, int shapeParam2);
    void updateCubeData(int shapeParam1);
    void updateConeData(int shapeParam1, int shapeParam2);
    void updateCylinderData(int shapeParam1, int shapeParam2);

    void updateShapeParameters();

    void bindShapeData();
    void passLightsUniforms();
    void passSceneUniforms();
    void passShapeUniforms(RenderShapeData& shape);
    void paintScene();
    void paintTexture(GLuint texture);

    void makeFBO();

    // Tick Related Variables
    int m_timer;                                        // Stores timer which attempts to run ~60 times per second
    QElapsedTimer m_elapsedTimer;                       // Stores timer which keeps track of actual time between frames

    // Input Related Variables
    bool m_mouseDown = false;                           // Stores state of left mouse button
    glm::vec2 m_prev_mouse_pos;                         // Stores mouse position
    std::unordered_map<Qt::Key, bool> m_keyMap;         // Stores whether keys are pressed or not

    // Device Correction Variables
    int m_devicePixelRatio;


    GLuint m_shader;     // Stores id of shader program
    GLuint m_texture_shader;

    GLuint m_sphere_vbo; // Stores id of vbo
    GLuint m_sphere_vao; // Stores id of vao
    std::vector<float> m_sphereData;
    GLuint m_cube_vbo; // Stores id of vbo
    GLuint m_cube_vao; // Stores id of vao
    std::vector<float> m_cubeData;
    GLuint m_cylinder_vbo; // Stores id of vbo
    GLuint m_cylinder_vao; // Stores id of vao
    std::vector<float> m_cylinderData;
    GLuint m_cone_vbo; // Stores id of vbo
    GLuint m_cone_vao; // Stores id of vao
    std::vector<float> m_coneData;

    GLuint m_fbo_texture;
    GLuint m_fbo_height;
    GLuint m_fbo_width;
    GLuint m_fbo_renderbuffer;
    GLuint m_fbo;
    GLuint m_defaultFBO;
    GLuint m_fullscreen_vbo;
    GLuint m_fullscreen_vao;
};
