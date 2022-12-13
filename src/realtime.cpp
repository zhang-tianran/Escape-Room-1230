#include "realtime.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>
#include "settings.h"
#include "utils/shaderloader.h"
#include "utils/sceneparser.h"
#include <glm/gtx/string_cast.hpp>

#include "utils/obj_loader.h"

// ================== Project 5: Lights, Camera

void Realtime::setBlurUniforms(){
    // Set uniform for kernel-based filter
//    glUseProgram(m_fxaa_shader);
//    glm::vec2 dudv = glm::vec2(1.f / size().width(), 1.f / size().height());
//    glUniform2fv(glGetUniformLocation(m_fxaa_shader, "dudv"), 1, &dudv[0]);
//    glUseProgram(0);
}

void Realtime::setFxaaUniforms(){
    glUseProgram(m_fxaa_shader);
    glUniform1i(glGetUniformLocation(m_fxaa_shader, "fxaaSampler"), 0);
    glUseProgram(0);
}

void Realtime::loadObjFromFile(std::string filepath) {
    loadObj(filepath, m_vertices, m_indexes);
}

void Realtime::updateShapeParameter(){
    m_cube->updateParams(std::max(1, settings.shapeParameter1));
    m_sphere->updateParams(std::max(2, settings.shapeParameter1), std::max(3, settings.shapeParameter2));
    m_cone->updateParams(std::max(1, settings.shapeParameter1), std::max(3, settings.shapeParameter2));
    m_cylinder->updateParams(std::max(1, settings.shapeParameter1), std::max(3, settings.shapeParameter2));
    m_shapeVertices = std::vector<std::vector<GLfloat>>();
    m_shapeVertices.push_back(m_cube->generateShape());
    m_shapeVertices.push_back(m_sphere->generateShape());
    m_shapeVertices.push_back(m_cone->generateShape());
    m_shapeVertices.push_back(m_cylinder->generateShape());
}

void Realtime::initShapes(){
    m_cube = new Cube();
    m_sphere = new Sphere();
    m_cylinder = new Cylinder();
    m_cone = new Cone();
    updateShapeParameter();
}

void Realtime::updateVBOat(int idx){
    glBindBuffer(GL_ARRAY_BUFFER, m_vbos[idx]);

    GLsizeiptr size = m_shapeVertices[idx].size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, size, m_shapeVertices[idx].data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void *>(6 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Realtime::initShapeVertexObjects(){
    GLuint vbo;
    m_vbos.push_back(vbo);
    int idx = m_vbos.size() - 1;
    glGenBuffers(1, &m_vbos[idx]);

    GLuint vao;
    m_vaos.push_back(vao);
    glGenVertexArrays(1, &m_vaos[idx]);
    glBindVertexArray(m_vaos[idx]);

    updateVBOat(idx);
    glBindVertexArray(0);
}

void Realtime::drawPrimitive(RenderShapeData& obj) {
    switch(obj.primitive.type) {
        case PrimitiveType::PRIMITIVE_CUBE:
            glBindVertexArray(m_vaos[0]);
            glDrawArrays(GL_TRIANGLES, 0, (int) m_shapeVertices[0].size() / 8);
            glBindBuffer(0, m_vaos[0]);
            break;
        case PrimitiveType::PRIMITIVE_SPHERE:
            glBindVertexArray(m_vaos[1]);
            glDrawArrays(GL_TRIANGLES, 0, (int) m_shapeVertices[1].size() / 8);
            glBindBuffer(0, m_vaos[1]);
            break;
        case PrimitiveType::PRIMITIVE_CONE:
            glBindVertexArray(m_vaos[2]);
            glDrawArrays(GL_TRIANGLES, 0, (int) m_shapeVertices[2].size() / 8);
            glBindBuffer(0, m_vaos[2]);
            break;
        case PrimitiveType::PRIMITIVE_CYLINDER:
            glBindVertexArray(m_vaos[3]);
            glDrawArrays(GL_TRIANGLES, 0, (int) m_shapeVertices[3].size() / 8);
            glBindBuffer(0, m_vaos[3]);
            break;
        default:
            break;
    }
}

// camera
void Realtime::updateCameraUniforms(){
    glUseProgram(m_shader);

    glUniformMatrix4fv(glGetUniformLocation(m_shader, "m_view"), 1, GL_FALSE, &m_camera.getViewMatrix()[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "m_proj"), 1, GL_FALSE, &m_camera.getProjection()[0][0]);
    glm::vec4 worldCamPos = glm::vec4(m_camera.m_pos, 1.f);
    glUniform4fv(glGetUniformLocation(m_shader, "worldCamPos"), 1, &worldCamPos[0]);

    glUseProgram(0);
}

void Realtime::initSceneUniforms(){

    glUseProgram(m_shader);

    // light
    for (int i = 0; i < 32; i++) {
        glUniform1i(glGetUniformLocation(m_shader, ("lightArr[" + std::to_string(i) + "].type").c_str()), 3);
        glUniform4f(glGetUniformLocation(m_shader, ("lightArr[" + std::to_string(i) + "].color").c_str()), 0, 0, 0, 0);
        glUniform4f(glGetUniformLocation(m_shader, ("lightArr[" + std::to_string(i) + "].dir").c_str()), 0, 0, 0, 0);
        glUniform4f(glGetUniformLocation(m_shader, ("lightArr[" + std::to_string(i) + "].pos").c_str()), 0, 0, 0, 0);
        glUniform3f(glGetUniformLocation(m_shader, ("lightArr[" + std::to_string(i) + "].function").c_str()), 0, 0, 0);
        glUniform1f(glGetUniformLocation(m_shader, ("lightArr[" + std::to_string(i) + "].penumbra").c_str()), 0);
        glUniform1f(glGetUniformLocation(m_shader, ("lightArr[" + std::to_string(i) + "].angle").c_str()), 0);

        if (i < m_metaData.lights.size()) {
            const SceneLightData &light = m_metaData.lights[i];
            glUniform4f(glGetUniformLocation(m_shader, ("lightArr[" + std::to_string(i) + "].color").c_str()), light.color.r, light.color.g, light.color.b, light.color.a);
            glUniform1i(glGetUniformLocation(m_shader, ("lightArr[" + std::to_string(i) + "].id").c_str()), light.id);
            switch(light.type) {
                case LightType::LIGHT_DIRECTIONAL:
                    glUniform1i(glGetUniformLocation(m_shader, ("lightArr[" + std::to_string(i) + "].type").c_str()), 2);
                    glUniform4fv(glGetUniformLocation(m_shader, ("lightArr[" + std::to_string(i) + "].dir").c_str()), 1, &light.dir[0]);
                    break;
                case LightType::LIGHT_POINT:
                    glUniform1i(glGetUniformLocation(m_shader, ("lightArr[" + std::to_string(i) + "].type").c_str()), 0);
                    glUniform4fv(glGetUniformLocation(m_shader, ("lightArr[" + std::to_string(i) + "].pos").c_str()), 1, &light.pos[0]);
                    glUniform3fv(glGetUniformLocation(m_shader, ("lightArr[" + std::to_string(i) + "].function").c_str()), 1, &light.function[0]);
                    break;
                case LightType::LIGHT_SPOT:
                    glUniform1i(glGetUniformLocation(m_shader, ("lightArr[" + std::to_string(i) + "].type").c_str()), 1);
                    glUniform4fv(glGetUniformLocation(m_shader, ("lightArr[" + std::to_string(i) + "].dir").c_str()), 1, &light.dir[0]);
                    glUniform4fv(glGetUniformLocation(m_shader, ("lightArr[" + std::to_string(i) + "].pos").c_str()), 1, &light.pos[0]);
                    glUniform3fv(glGetUniformLocation(m_shader, ("lightArr[" + std::to_string(i) + "].function").c_str()), 1, &light.function[0]);
                    glUniform1f(glGetUniformLocation(m_shader, ("lightArr[" + std::to_string(i) + "].penumbra").c_str()), light.penumbra);
                    glUniform1f(glGetUniformLocation(m_shader, ("lightArr[" + std::to_string(i) + "].angle").c_str()), light.angle);
                    break;
                default:
                    break;
            }
        }
    }

    // global data
    glUniform1f(glGetUniformLocation(m_shader, "ka"), m_metaData.globalData.ka);
    glUniform1f(glGetUniformLocation(m_shader, "kd"), m_metaData.globalData.kd);
    glUniform1f(glGetUniformLocation(m_shader, "ks"), m_metaData.globalData.ks);

    // shadows
    if (settings.drawShadows == "true") {
        glUniform1i(glGetUniformLocation(m_shader, "useShadows"), 1);
    } else {
        glUniform1i(glGetUniformLocation(m_shader, "useShadows"), 0);
    }
    glUniform1i(glGetUniformLocation(m_shader, "depthMap"), 1);
    glUniform1f(glGetUniformLocation(m_shader, "far_plane"), 25.0f);


    glUseProgram(0);

    updateCameraUniforms();
}

void Realtime::makeShadowFbos() {
    glGenFramebuffers(1, &m_shadow_fbo);

    glGenTextures(1, &m_depthTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, m_depthTexture);

    glTexImage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 0, GL_DEPTH_COMPONENT, settings.mapSize, settings.mapSize,
                 6 * m_metaData.lights.size(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, m_shadow_fbo);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Realtime::setShadowUniforms(SceneLightData& light) {
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, 25.0f);
    glm::vec3 lightPos = glm::vec3(light.pos);

    std::vector<glm::mat4> shadowTransforms;

    shadowTransforms.push_back(shadowProj *
                   glm::lookAt(lightPos, lightPos + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
    shadowTransforms.push_back(shadowProj *
                   glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
    shadowTransforms.push_back(shadowProj *
                   glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
    shadowTransforms.push_back(shadowProj *
                   glm::lookAt(lightPos, lightPos + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0)));
    shadowTransforms.push_back(shadowProj *
                   glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0)));
    shadowTransforms.push_back(shadowProj *
                   glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0)));

    glUniform3fv(glGetUniformLocation(m_depth_shader, "lightPos"), 1, &lightPos[0]);
    glUniform1f(glGetUniformLocation(m_depth_shader, "far_plane"), 25.0f);
    glUniform1i(glGetUniformLocation(m_depth_shader, "lightId"), light.id);
    for (int i = 0; i < 6; i++) {
        glUniformMatrix4fv(glGetUniformLocation(m_depth_shader, ("shadowMatrices[" + std::to_string(i) + "]").c_str()),
                           1, GL_FALSE, &shadowTransforms[i][0][0]);
    }
}

void Realtime::makeFbo(GLuint& fbo, GLuint& texture, GLuint& renderbuffer){

    // Generate and bind an empty texture, set its min/mag filter interpolation, then unbind
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_fbo_width, m_fbo_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Generate and bind a renderbuffer of the right size, set its format, then unbind
    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_fbo_width, m_fbo_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // Generate and bind an FBO
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Add texture as a color attachment, and our renderbuffer as a depth+stencil attachment, to our FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

    // Unbind the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);
}

void Realtime::initFbo(){
    // height and width data
    m_defaultFBO = 3;
    m_screen_width = size().width() * m_devicePixelRatio;
    m_screen_height = size().height() * m_devicePixelRatio;
    m_fbo_width = m_screen_width;
    m_fbo_height = m_screen_height;

    std::vector<GLfloat> fullscreen_quad_data =
    { //     POSITIONS    //
        -1.f,  1.f, 0.0f,
         0.f,  1.f,
        -1.f, -1.f, 0.0f,
         0.f,  0.f,
         1.f, -1.f, 0.0f,
         1.f,  0.f,
         1.f,  1.f, 0.0f,
         1.f,  1.f,
        -1.f,  1.f, 0.0f,
         0.f,  1.f,
         1.f, -1.f, 0.0f,
         1.f,  0.f,
    };

    // Generate and bind a VBO and a VAO for a fullscreen quad
    glGenBuffers(1, &m_fullscreen_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_fullscreen_vbo);
    glBufferData(GL_ARRAY_BUFFER, fullscreen_quad_data.size()*sizeof(GLfloat), fullscreen_quad_data.data(), GL_STATIC_DRAW);
    glGenVertexArrays(1, &m_fullscreen_vao);
    glBindVertexArray(m_fullscreen_vao);

    // Task 14: modify the code below to add a second attribute to the vertex attribute array
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void*>(0 * sizeof(GLfloat)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));

    // Unbind the fullscreen quad's VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

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
    initShapes();
    m_camera = Camera();
}

void Realtime::finish() {
    killTimer(m_timer);
    this->makeCurrent();

    // Students: anything requiring OpenGL calls when the program exits should be done here
    for (int i = 0; i < m_shapeVertices.size(); i++) {
        glDeleteBuffers(1, &m_vbos[i]);
        glDeleteVertexArrays(1, &m_vaos[i]);
    }
    deleteFbo();
    glDeleteProgram(m_shader);
    glDeleteProgram(m_fxaa_shader);
    glDeleteProgram(m_depth_shader);

    delete m_sphere;
    delete m_cone;
    delete m_cube;
    delete m_cylinder;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &m_shadow_fbo);

    this->doneCurrent();
}

void Realtime::initializeGL() {
    m_devicePixelRatio = this->devicePixelRatio();

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
    // Tells OpenGL to interpolate across cube map faces
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here
    glClearColor(0, 0, 0, 1);

    m_shader = ShaderLoader::createShaderProgram(":/resources/shaders/default.vert", ":/resources/shaders/default.frag");
    m_fxaa_shader = ShaderLoader::createShaderProgram(":/resources/shaders/fxaa.vert", ":/resources/shaders/fxaa.frag");
    m_depth_shader = ShaderLoader::createShaderProgramWithGeometry(":/resources/shaders/depth.vert", ":/resources/shaders/depth.frag", ":/resources/shaders/depth.geom");

    // Vao/Vbo
    for (int i = 0; i < m_shapeVertices.size(); i++) {
        initShapeVertexObjects();
    }

    // Scene
    SceneParser::parse(settings.sceneFilePath, m_metaData);
    m_camera.setCamData(m_metaData.cameraData);
    m_camera.setCamWindow(settings.farPlane, settings.nearPlane, (float) width() / height());
    initSceneUniforms();

    // Obj
    loadObjFromFile(settings.objFilePath);

    glGenBuffers(1, &m_obj_vbo);
    glGenVertexArrays(1, &m_obj_vao);
    glBindVertexArray(m_obj_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_obj_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Fbo
    initFbo();
    makeFbo(m_fxaa_fbo, m_fxaa_texture, m_fxaa_renderbuffer);
    setFxaaUniforms();
    makeShadowFbos();
}

void Realtime::paintGeometry(){
    for (RenderShapeData &obj: m_metaData.shapes) {
        // Transformation matrices
        glUniformMatrix4fv(glGetUniformLocation(m_shader, "m_model"), 1, GL_FALSE, &obj.ctm[0][0]);
        glm::mat3 inv = inverse(transpose(glm::mat3(obj.ctm)));
        glUniformMatrix3fv(glGetUniformLocation(m_shader, "m_norm"), 1, GL_FALSE, &inv[0][0]);


        // Material
        SceneMaterial material = obj.primitive.material;
        glUniform4f(glGetUniformLocation(m_shader, "material.cAmbient"), material.cAmbient.r, material.cAmbient.g, material.cAmbient.b, material.cAmbient.a);
        glUniform4f(glGetUniformLocation(m_shader, "material.cDiffuse"), material.cDiffuse.r, material.cDiffuse.g, material.cDiffuse.b, material.cDiffuse.a);
        glUniform4f(glGetUniformLocation(m_shader, "material.cSpecular"), material.cSpecular.r, material.cSpecular.g, material.cSpecular.b, material.cSpecular.a);
        glUniform1f(glGetUniformLocation(m_shader, "material.shininess"), material.shininess);

        drawPrimitive(obj);
    }
}

void Realtime::paintObj() {
    glBindVertexArray(m_obj_vao);
    glm::mat4 translation = glm::mat4(glm::vec4(1, 0, 0, 0),
                                      glm::vec4(0, 1, 0, 0),
                                      glm::vec4(0, 0, 1, 0),
                                      glm::vec4(-26, 0, 0, 1));

    glm::mat4 ctm = translation;
    glm::mat3 inv = inverse(transpose(glm::mat3(ctm)));
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "m_model"), 1, GL_FALSE, &ctm[0][0]);
    glUniformMatrix3fv(glGetUniformLocation(m_shader, "m_norm"), 1, GL_FALSE, &inv[0][0]);

    glUniform4f(glGetUniformLocation(m_shader, "material.cAmbient"), 0, 0, 0, 0);
    glUniform4f(glGetUniformLocation(m_shader, "material.cDiffuse"), 1, 1, 1, 1);
    glUniform4f(glGetUniformLocation(m_shader, "material.cSpecular"), 0, 0, 0, 0);
    glUniform1f(glGetUniformLocation(m_shader, "material.shininess"), 0);

    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() / 6);
}

void Realtime::paintTexture(GLuint texture){
    glBindVertexArray(m_fullscreen_vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawArrays(GL_TRIANGLES, 0, 8);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}

void Realtime::paintShadows() {
    glUseProgram(m_depth_shader);

    if (settings.drawSceneGeometry == "true") {
        for (int i = 0; i < m_metaData.lights.size(); i++) {
            setShadowUniforms(m_metaData.lights[i]);
            for (RenderShapeData &obj: m_metaData.shapes) {
                // Transformation matrices
                glUniformMatrix4fv(glGetUniformLocation(m_depth_shader, "m_model"), 1, GL_FALSE, &obj.ctm[0][0]);

                drawPrimitive(obj);
            }
        }
    }

    if (settings.drawObjMeshes == "true") {
        for (int i = 0; i < m_metaData.lights.size(); i++) {
            setShadowUniforms(m_metaData.lights[i]);
            glm::mat4 translation = glm::mat4(glm::vec4(1, 0, 0, 0),
                                              glm::vec4(0, 1, 0, 0),
                                              glm::vec4(0, 0, 1, 0),
                                              glm::vec4(-26, 0, 0, 1));

            glm::mat4 ctm = translation;
            glUniformMatrix4fv(glGetUniformLocation(m_depth_shader, "m_model"), 1, GL_FALSE, &ctm[0][0]);

            glBindVertexArray(m_obj_vao);
            glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() / 6);
        }
    }
}

void Realtime::paintGL() {
    if (!m_shadowsDrawn) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_shadow_fbo);
        glViewport(0, 0, settings.mapSize, settings.mapSize);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // paint shadows
        glUseProgram(m_depth_shader);
        paintShadows();

        m_shadowsDrawn = true;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_fxaa_fbo);
    glViewport(0, 0, m_fbo_width, m_fbo_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // paint geometry
    glUseProgram(m_shader);
    if (settings.drawSceneGeometry == "true") {
        paintGeometry();
    }
    if (settings.drawObjMeshes == "true") {
        paintObj();
    }

    glUseProgram(m_fxaa_shader);
    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);
    glViewport(0, 0, m_screen_width, m_screen_height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // paint texture
    paintTexture(m_fxaa_texture);
    glUseProgram(0);
}

void Realtime::deleteFbo(){
    glDeleteTextures(1, &m_fxaa_texture);
    glDeleteRenderbuffers(1, &m_fxaa_renderbuffer);
    glDeleteFramebuffers(1, &m_fxaa_fbo);
}

void Realtime::resizeGL(int w, int h) {
    // Tells OpenGL how big the screen is
    glViewport(0, 0, size().width() * m_devicePixelRatio, size().height() * m_devicePixelRatio);

    // Students: anything requiring OpenGL calls when the program starts should be done here
    deleteFbo();
    m_screen_width = size().width() * m_devicePixelRatio;
    m_screen_height = size().height() * m_devicePixelRatio;
    m_fbo_width = m_screen_width;
    m_fbo_height = m_screen_height;

    makeFbo(m_fxaa_fbo, m_fxaa_texture, m_fxaa_renderbuffer);

    m_camera.setCamWindow(settings.farPlane, settings.nearPlane, (float) width() / height());
    updateCameraUniforms();
}

void Realtime::sceneChanged() {
    SceneParser::parse(settings.sceneFilePath, m_metaData);
    m_camera.setCamData(m_metaData.cameraData);
    m_camera.setCamWindow(settings.farPlane, settings.nearPlane, (float) width() / height());
    initSceneUniforms();

    update(); // asks for a PaintGL() call to occur
}

void Realtime::settingsChanged() {
    // camera change
    m_camera.setCamWindow(settings.farPlane, settings.nearPlane, (float) width() / height());

    // update if scene is loaded
    if (m_vbos.size() > 0) {
        for (int i = 0; i < m_shapeVertices.size(); i++) {
            updateVBOat(i);
        }
        updateCameraUniforms();
    }

    update(); // asks for a PaintGL() call to occur
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

void Realtime::mouseMoveEvent(QMouseEvent *event) {
    if (m_mouseDown) {
        int posX = event->position().x();
        int posY = event->position().y();
        int deltaX = posX - m_prev_mouse_pos.x;
        int deltaY = posY - m_prev_mouse_pos.y;
        m_prev_mouse_pos = glm::vec2(posX, posY);

        // Use deltaX and deltaY here to rotate
        float angleX = (float) deltaX / width();
        m_camera.setCamRotate(glm::vec3(0, 1, 0), angleX);

        float angleY = (float) deltaY / height();
        glm::vec3 axis = glm::normalize(glm::cross(glm::vec3(m_camera.m_look), glm::vec3(m_camera.m_up)));
        m_camera.setCamRotate(axis, angleY);

        updateCameraUniforms();

        update(); // asks for a PaintGL() call to occur
    }
}

void Realtime::timerEvent(QTimerEvent *event) {
    int elapsedms   = m_elapsedTimer.elapsed();
    float deltaTime = elapsedms * 0.001f;
    m_elapsedTimer.restart();

    glm::vec3 translation = glm::vec3(0.f);
    glm::vec3 xDir = glm::cross(glm::vec3(m_camera.m_look), glm::vec3(m_camera.m_up));

    // Use deltaTime and m_keyMap here to move around
    if (m_keyMap[Qt::Key_W]) {
        translation += glm::normalize(m_camera.m_look);
    }
    if (m_keyMap[Qt::Key_S]) {
        translation -= glm::normalize(m_camera.m_look);
    }
    if (m_keyMap[Qt::Key_A]) {
        translation -= glm::normalize(xDir);
    }
    if (m_keyMap[Qt::Key_D]) {
        translation += glm::normalize(xDir);
    }
    if (m_keyMap[Qt::Key_Space]) {
        translation += glm::normalize(m_camera.m_up);
    }
    if (m_keyMap[Qt::Key_Control]) {
        translation -= glm::normalize(m_camera.m_up);
    }
    if (translation != glm::vec3(0.f)) {
        translation *= 5.f * deltaTime;
        m_camera.setCamPos(translation);
//        std::cout << "x: " << m_camera.m_pos[0] << "y: " << m_camera.m_pos[1] << "z: " << m_camera.m_pos[2] << std::endl;
        updateCameraUniforms();
    }
    update(); // asks for a PaintGL() call to occur
}
