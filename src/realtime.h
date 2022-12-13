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

#include "camera/camera.h"
#include "utils/sceneparser.h"
#include "shapes/Cube.h"
#include "shapes/Sphere.h"
#include "shapes/Cylinder.h"
#include "shapes/Cone.h"


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

    // Tick Related Variables
    int m_timer;                                        // Stores timer which attempts to run ~60 times per second
    QElapsedTimer m_elapsedTimer;                       // Stores timer which keeps track of actual time between frames

    // Input Related Variables
    bool m_mouseDown = false;                           // Stores state of left mouse button
    glm::vec2 m_prev_mouse_pos;                         // Stores mouse position
    std::unordered_map<Qt::Key, bool> m_keyMap;         // Stores whether keys are pressed or not

    // Device Correction Variables
    int m_devicePixelRatio;
    GLuint m_defaultFBO;
    int m_fbo_width;
    int m_fbo_height;
    int m_screen_width;
    int m_screen_height;

    // Shader
    GLuint m_shader;
    GLuint m_texture_shader;

    // vao/vbo
    void initShapeVertexObjects();
    void updateVBOat(int idx);
    std::vector<GLuint> m_vbos;   //Stores id for vbo
    std::vector<GLuint> m_vaos;   //Stores id for vbo
    std::vector<std::vector<GLfloat>> m_shapeVertices;

    //fbo
    void initFbo();
    void makeFbo();
    void initFboFilter();
    void deleteFbo();
    GLuint m_fullscreen_vbo;
    GLuint m_fullscreen_vao;
    GLuint m_fbo;
    GLuint m_fbo_texture;
    GLuint m_fbo_renderbuffer;

    // shadow objects
    void makeShadowFbos();
    void drawDepthMaps();
    void setShadowUniforms(SceneLightData& light);
    GLuint m_depth_shader;
    GLuint m_shadow_fbo;
    GLuint m_depthTexture;

    // paint
    void paintGeometry();
    void paintShadows();
    void paintTexture(GLuint texture);

    // Scene info
    Camera m_camera;
    RenderData m_metaData;
    void initSceneUniforms();
    void updateCameraUniforms();

    // Shapes
    void updateShapeParameter();
    void initShapes();
    void drawPrimitive(RenderShapeData& obj);
    Cube* m_cube;
    Sphere* m_sphere;
    Cylinder* m_cylinder;
    Cone* m_cone;
};
