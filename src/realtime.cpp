#include "realtime.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "settings.h"
#include "shapes/Cone.h"
#include "shapes/Cube.h"
#include "shapes/Cylinder.h"
#include "shapes/Sphere.h"
#include "utils/shaderloader.h"

// ================== Project 5: Lights, Camera

Realtime::Realtime(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_prev_mouse_pos = glm::vec2(size().width()/2, size().height()/2);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    m_keyMap[Qt::Key_W]       = false;
    m_keyMap[Qt::Key_A]       = false;
    m_keyMap[Qt::Key_S]       = false;
    m_keyMap[Qt::Key_D]       = false;
    m_keyMap[Qt::Key_Control] = false;
    m_keyMap[Qt::Key_Space]   = false;

    // If you must use this function, do not edit anything above this
    m_defaultFBO = 2;
}

void Realtime::finish() {
    killTimer(m_timer);
    this->makeCurrent();

    // Students: anything requiring OpenGL calls when the program exits should be done here

    glDeleteProgram(m_shader);

    glDeleteBuffers(1, &m_sphere_vbo);
    glDeleteBuffers(1, &m_cube_vbo);
    glDeleteBuffers(1, &m_cone_vbo);
    glDeleteBuffers(1, &m_cylinder_vbo);
    glDeleteBuffers(1, &m_fullscreen_vbo);

    glDeleteVertexArrays(1, &m_sphere_vao);
    glDeleteVertexArrays(1, &m_cube_vao);
    glDeleteVertexArrays(1, &m_cone_vao);
    glDeleteVertexArrays(1, &m_cylinder_vao);
    glDeleteVertexArrays(1, &m_fullscreen_vao);

    glDeleteProgram(m_texture_shader);
    glDeleteTextures(1, &m_fbo_texture);
    glDeleteRenderbuffers(1, &m_fbo_renderbuffer);
    glDeleteFramebuffers(1, &m_fbo);

    this->doneCurrent();
}

void Realtime::updateSphereData(int shapeParam1, int shapeParam2) {
    Sphere sphere = Sphere{};
    sphere.updateParams(shapeParam1, shapeParam2);
    m_sphereData = sphere.generateShape();
}

void Realtime::updateCubeData(int shapeParam1) {
    Cube cube = Cube{};
    cube.updateParams(shapeParam1);
    m_cubeData = cube.generateShape();
}

void Realtime::updateConeData(int shapeParam1, int shapeParam2) {
    Cone cone = Cone{};
    cone.updateParams(shapeParam1, shapeParam2);
    m_coneData = cone.generateShape();
}

void Realtime::updateCylinderData(int shapeParam1, int shapeParam2) {
    Cylinder cylinder = Cylinder{};
    cylinder.updateParams(shapeParam1, shapeParam2);
    m_cylinderData = cylinder.generateShape();
}

// Updates the stored vertex data for each shape based on given shape parameters
void Realtime::updateShapeParameters() {
    updateSphereData(settings.shapeParameter1, settings.shapeParameter2);
    updateCubeData(settings.shapeParameter1);
    updateConeData(settings.shapeParameter1, settings.shapeParameter2);
    updateCylinderData(settings.shapeParameter1, settings.shapeParameter2);
}

// Passes uniforms to the current shader for up to 8 lights
void Realtime::passLightsUniforms() {
    for (int i = 0; i < 8 && i < settings.renderData.lights.size(); i++) {
        GLint lightTypeLoc = glGetUniformLocation(m_shader, ("lights[" + std::to_string(i) + "].lightType").c_str());
        GLint lightPosLoc = glGetUniformLocation(m_shader, ("lights[" + std::to_string(i) + "].lightPos").c_str());
        GLint lightDirLoc = glGetUniformLocation(m_shader, ("lights[" + std::to_string(i) + "].lightDir").c_str());
        GLint lightColorLoc = glGetUniformLocation(m_shader, ("lights[" + std::to_string(i) + "].lightColor").c_str());
        GLint lightFuncLoc = glGetUniformLocation(m_shader, ("lights[" + std::to_string(i) + "].function").c_str());
        GLint lightAngleLoc = glGetUniformLocation(m_shader, ("lights[" + std::to_string(i) + "].lightAngle").c_str());
        GLint lightPenumbraLoc = glGetUniformLocation(m_shader, ("lights[" + std::to_string(i) + "].penumbra").c_str());

        if (settings.renderData.lights[i].type == LightType::LIGHT_POINT) {
            glUniform1i(lightTypeLoc, 1);
        } else if (settings.renderData.lights[i].type == LightType::LIGHT_DIRECTIONAL) {
            glUniform1i(lightTypeLoc, 2);
        } else if (settings.renderData.lights[i].type == LightType::LIGHT_SPOT) {
            glUniform1i(lightTypeLoc, 3);
        }

        glUniform4fv(lightPosLoc, 1, &settings.renderData.lights[i].pos[0]);
        glUniform4fv(lightDirLoc, 1, &settings.renderData.lights[i].dir[0]);
        glUniform4fv(lightColorLoc, 1, &settings.renderData.lights[i].color[0]);
        glUniform3fv(lightFuncLoc, 1, &settings.renderData.lights[i].function[0]);
        glUniform1f(lightAngleLoc, settings.renderData.lights[i].angle);
        glUniform1f(lightPenumbraLoc, settings.renderData.lights[i].penumbra);
    }
    glUniform1i(glGetUniformLocation(m_shader, "numLights"), std::min(int(settings.renderData.lights.size()), 8));
}

// Updates the shape VAOs with the stored vertex data for each shape
void Realtime::bindShapeData() {
    glGenBuffers(1, &m_sphere_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_sphere_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_sphereData.size() * sizeof(GLfloat), m_sphereData.data(), GL_STATIC_DRAW);
    glGenVertexArrays(1, &m_sphere_vao);
    glBindVertexArray(m_sphere_vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));

    glGenBuffers(1, &m_cylinder_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_cylinder_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_cylinderData.size() * sizeof(GLfloat), m_cylinderData.data(), GL_STATIC_DRAW);
    glGenVertexArrays(1, &m_cylinder_vao);
    glBindVertexArray(m_cylinder_vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));

    glGenBuffers(1, &m_cube_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_cube_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_cubeData.size() * sizeof(GLfloat), m_cubeData.data(), GL_STATIC_DRAW);
    glGenVertexArrays(1, &m_cube_vao);
    glBindVertexArray(m_cube_vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));

    glGenBuffers(1, &m_cone_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_cone_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_coneData.size() * sizeof(GLfloat), m_coneData.data(), GL_STATIC_DRAW);
    glGenVertexArrays(1, &m_cone_vao);
    glBindVertexArray(m_cone_vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));

    // Clean-up bindings
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

void Realtime::initializeGL() {
    m_devicePixelRatio = this->devicePixelRatio();
    m_fbo_width = size().width() * m_devicePixelRatio;
    m_fbo_height = size().height() * m_devicePixelRatio;

    m_timer = startTimer(1000/60);
    m_elapsedTimer.start();

    // Initializing GL.
    // GLEW (GL Extension Wrangler) provides access to OpenGL functions.
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error while initializing GL: " << glewGetErrorString(err) << std::endl;
    }
    std::cout << "Initialized GL: Version " << glewGetString(GLEW_VERSION) << std::endl;

    // Allows OpenGL to draw objects appropriately on top of one another
    glEnable(GL_DEPTH_TEST);
    // Tells OpenGL to only draw the front face
    glEnable(GL_CULL_FACE);
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here
    glClearColor(0, 0, 0, 1);
    m_texture_shader = ShaderLoader::createShaderProgram(":/resources/shaders/texture.vert", ":/resources/shaders/texture.frag");
    m_shader = ShaderLoader::createShaderProgram(":/resources/shaders/default.vert", ":/resources/shaders/default.frag");

    glUseProgram(m_texture_shader);
    glUniform1i(glGetUniformLocation(m_texture_shader, "sampler"), 0);

    std::vector<GLfloat> fullscreen_quad_data =
    { //     POSITIONS    //
     -1.0f,  1.0f, 0.0f,
      0.0f,  1.0f,
     -1.0f, -1.0f, 0.0f,
      0.0f,  0.0f,
      1.0f, -1.0f, 0.0f,
      1.0f,  0.0f,
      1.0f,  1.0f, 0.0f,
      1.0f,  1.0f,
     -1.0f,  1.0f, 0.0f,
      0.0f,  1.0f,
      1.0f, -1.0f, 0.0f,
      1.0f,  0.0f
    };

    glGenBuffers(1, &m_fullscreen_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_fullscreen_vbo);
    glBufferData(GL_ARRAY_BUFFER, fullscreen_quad_data.size()*sizeof(GLfloat), fullscreen_quad_data.data(), GL_STATIC_DRAW);
    glGenVertexArrays(1, &m_fullscreen_vao);
    glBindVertexArray(m_fullscreen_vao);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    makeFBO();

    SceneParser::parse(settings.sceneFilePath, settings.renderData, settings.nearPlane, settings.farPlane, float(width()) / float(height()));
    updateShapeParameters();
}

// Passes uniforms to the current shader for camera and globals
void Realtime::passSceneUniforms() {
    glm::vec4 worldCameraPos = settings.renderData.cameraData.inverseViewMatrix * glm::vec4(0, 0, 0, 1);

    glUniform4fv(glGetUniformLocation(m_shader, "cameraPos"), 1, &worldCameraPos[0]);
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "view"), 1, GL_FALSE, &settings.renderData.cameraData.viewMatrix[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "proj"), 1, GL_FALSE, &settings.renderData.cameraData.projectionMatrix[0][0]);

    glUniform1f(glGetUniformLocation(m_shader, "ka"), settings.renderData.globalData.ka);
    glUniform1f(glGetUniformLocation(m_shader, "kd"), settings.renderData.globalData.kd);
    glUniform1f(glGetUniformLocation(m_shader, "ks"), settings.renderData.globalData.ks);
}

// Passes uniforms to the current shader for a shape
void Realtime::passShapeUniforms(RenderShapeData& shape) {
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "model"), 1, GL_FALSE, &shape.ctm[0][0]);
    glUniformMatrix3fv(glGetUniformLocation(m_shader, "invTModel"), 1, GL_FALSE, &shape.invTCtm[0][0]);

    glUniform4fv(glGetUniformLocation(m_shader, "materialAmbient"), 1, &shape.primitive.material.cAmbient[0]);
    glUniform4fv(glGetUniformLocation(m_shader, "materialDiffuse"), 1, &shape.primitive.material.cDiffuse[0]);
    glUniform4fv(glGetUniformLocation(m_shader, "materialSpecular"), 1, &shape.primitive.material.cSpecular[0]);
    glUniform1f(glGetUniformLocation(m_shader, "shininess"), shape.primitive.material.shininess);
}

// Paints the scene by calling glDrawArrays for each shape in the scene
void Realtime::paintScene() {
    glUseProgram(m_shader);

    bindShapeData();

    passSceneUniforms();
    for (RenderShapeData& shape : settings.renderData.shapes) {
        passShapeUniforms(shape);
        passLightsUniforms();

        switch(shape.primitive.type) {
            case PrimitiveType::PRIMITIVE_SPHERE:
                glBindVertexArray(m_sphere_vao);
                glDrawArrays(GL_TRIANGLES, 0, m_sphereData.size() / 6);
                break;
            case PrimitiveType::PRIMITIVE_CUBE:
                glBindVertexArray(m_cube_vao);
                glDrawArrays(GL_TRIANGLES, 0, m_cubeData.size() / 6);
                break;
            case PrimitiveType::PRIMITIVE_CONE:
                glBindVertexArray(m_cone_vao);
                glDrawArrays(GL_TRIANGLES, 0, m_coneData.size() / 6);
                break;
            case PrimitiveType::PRIMITIVE_CYLINDER:
                glBindVertexArray(m_cylinder_vao);
                glDrawArrays(GL_TRIANGLES, 0, m_cylinderData.size() / 6);
                break;
            default:
                break;
        }
    }
    glBindVertexArray(0);
    glUseProgram(0);
}

// Applies any post-processing filters using m_texture_shader
void Realtime::paintTexture(GLuint texture) {
    glUseProgram(m_texture_shader);
    glUniform1i(glGetUniformLocation(m_texture_shader, "fboWidth"), m_fbo_width);
    glUniform1i(glGetUniformLocation(m_texture_shader, "fboHeight"), m_fbo_height);

    glBindVertexArray(m_fullscreen_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Realtime::paintGL() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_fbo_width, m_fbo_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    paintScene();

    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    paintTexture(m_fbo_texture);
}

void Realtime::resizeGL(int w, int h) {
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here
    SceneCameraData cameraData = settings.renderData.cameraData;
    settings.renderData.cameraData.projectionMatrix =
            SceneParser::getProjectionMatrix(settings.nearPlane, settings.farPlane, cameraData.heightAngle, float(width()) / float(height()));
}

void Realtime::settingsChanged() {
    SceneCameraData cameraData = settings.renderData.cameraData;
    settings.renderData.cameraData.projectionMatrix =
            SceneParser::getProjectionMatrix(settings.nearPlane, settings.farPlane, cameraData.heightAngle, float(width()) / float(height()));

    updateShapeParameters();
    update(); // asks for a PaintGL() call to occur
}

// Makes the framebuffer
void Realtime::makeFBO(){
    glGenTextures(1, &m_fbo_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_fbo_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_fbo_width, m_fbo_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenRenderbuffers(1, &m_fbo_renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_fbo_renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_fbo_width, m_fbo_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbo_texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_fbo_renderbuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);
}

// ================== Project 6: Action!

void Realtime::keyPressEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = true;
}

void Realtime::keyReleaseEvent(QKeyEvent *event) {
    m_keyMap[Qt::Key(event->key())] = false;
}

void Realtime::mousePressEvent(QMouseEvent *event) {
    if (event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = true;
        m_prev_mouse_pos = glm::vec2(event->position().x(), event->position().y());
    }
}

void Realtime::mouseReleaseEvent(QMouseEvent *event) {
    if (!event->buttons().testFlag(Qt::LeftButton)) {
        m_mouseDown = false;
    }
}

glm::mat3 getRotationMatrix(float angle, glm::vec3 axis) {
    float x = axis[0];
    float y = axis[1];
    float z = axis[2];
    return glm::mat3(glm::vec3(cos(angle) + powf(x, 2) * (1 - cos(angle)),
                                x * y * (1 - cos(angle)) + z * sin(angle),
                                x * z * (1 - cos(angle)) - y * sin(angle)),
                      glm::vec3(x * y * (1 - cos(angle)) - z * sin(angle),
                                cos(angle) + powf(y, 2) * (1 - cos(angle)),
                                y * z * (1 - cos(angle)) + x * sin(angle)),
                      glm::vec3(x * z * (1 - cos(angle)) + y * sin(angle),
                                y * z * (1 - cos(angle)) - x * sin(angle),
                                cos(angle) + powf(z, 2) * (1 - cos(angle))));

}

void Realtime::mouseMoveEvent(QMouseEvent *event) {
    if (m_mouseDown) {
        int posX = event->position().x();
        int posY = event->position().y();
        int deltaX = posX - m_prev_mouse_pos.x;
        int deltaY = posY - m_prev_mouse_pos.y;
        m_prev_mouse_pos = glm::vec2(posX, posY);

        // Use deltaX and deltaY here to rotate
        SceneCameraData &cameraData = settings.renderData.cameraData;

        glm::mat3 xRotation = getRotationMatrix(-deltaX / 100.f, glm::vec3(0, 1, 0));
        glm::mat3 yRotation = getRotationMatrix(-deltaY / 100.f, glm::normalize(glm::cross(glm::vec3(cameraData.look), glm::vec3(cameraData.up))));

        cameraData.pos = glm::vec4(xRotation * yRotation * cameraData.pos, 0);
        cameraData.look = glm::vec4(xRotation * yRotation * cameraData.look, 0);
        cameraData.up = glm::vec4(xRotation * yRotation * cameraData.up, 0);

        cameraData.viewMatrix = SceneParser::getViewMatrix(cameraData.pos, cameraData.look, cameraData.up);
        cameraData.inverseViewMatrix = glm::inverse(cameraData.viewMatrix);

        update(); // asks for a PaintGL() call to occur
    }
}

void Realtime::timerEvent(QTimerEvent *event) {
    int elapsedms   = m_elapsedTimer.elapsed();
    float deltaTime = elapsedms * 0.001f;
    m_elapsedTimer.restart();

    // Use deltaTime and m_keyMap here to move around
    glm::vec4 shiftDirection = glm::vec4(0);

    if (m_keyMap[Qt::Key_W]) {
        shiftDirection += glm::vec4(0, 0, -1, 0);
    }
    if (m_keyMap[Qt::Key_S]) {
        shiftDirection += glm::vec4(0, 0, 1, 0);
    }
    if (m_keyMap[Qt::Key_A]) {
        shiftDirection += glm::vec4(-1, 0, 0, 0);
    }
    if (m_keyMap[Qt::Key_D]) {
        shiftDirection += glm::vec4(1, 0, 0, 0);
    }
    if (m_keyMap[Qt::Key_Space]) {
        shiftDirection += glm::vec4(0, 1, 0, 0);
    }
    if (m_keyMap[Qt::Key_Control]) {
        shiftDirection += glm::vec4(0, -1, 0, 0);
    }

    if (glm::length(shiftDirection) > 0) {
        SceneCameraData &cameraData = settings.renderData.cameraData;

        shiftDirection = cameraData.inverseViewMatrix * shiftDirection;
        shiftDirection = glm::normalize(shiftDirection);
        glm::vec4 shiftVector = 50 * deltaTime * shiftDirection;
        cameraData.pos += shiftVector;
        cameraData.viewMatrix = SceneParser::getViewMatrix(cameraData.pos,
                                                           cameraData.look,
                                                           cameraData.up);
        cameraData.inverseViewMatrix = glm::inverse(cameraData.viewMatrix);
    }


    update(); // asks for a PaintGL() call to occur
}
