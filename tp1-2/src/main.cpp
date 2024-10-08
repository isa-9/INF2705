
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>

#include "window.h"
#include "shader_program.h"
#include "vertices_data.h"
#include "shapes.h"
#include "utils.h"
#include "camera.h"
#include "model.h"
#include "textures.h"


void printGLInfo();

#define GL_CHECK_ERROR checkGLError(__FILE__, __LINE__)
void checkGLError(const char* file, int line);

std::string readFile(const char* path);

void handleMouseMotion(const int& xMouse, const int& yMouse, glm::vec2& playerOrientation);
glm::mat4 modelMatrixSuzanne(const glm::vec3& pos, const glm::vec2& orientation);

void handleKeyboardPress(Window& w, glm::vec3& playerPosition, const glm::vec2& orientation);

int main(int argc, char* argv[])
{
    Window w;
    if (!w.init())
        return -1;
    
    GLenum rev = glewInit();
    if ( rev != GLEW_OK )
    {
        std::cout << "Could not initialize glew! GLEW_Error: " << glewGetErrorString(rev) << std::endl;
        return -2;
    }

    ShaderProgram modelProg;
    GLuint locMatrMVPModel, locTexModel;

    {
        std::string modelVSCode = readFile("shaders/model.vs.glsl");
        std::string modelFSCode = readFile("shaders/model.fs.glsl");

        Shader modelVertexShader(GL_VERTEX_SHADER, modelVSCode.c_str());
        modelProg.attachShader(modelVertexShader);

        Shader modelFragmentShader(GL_FRAGMENT_SHADER, modelFSCode.c_str());
        modelProg.attachShader(modelFragmentShader);

        modelProg.link();
        locMatrMVPModel = modelProg.getUniformLoc("mvp");
        locTexModel = modelProg.getUniformLoc("tex0");
    }

    ShaderProgram waterProg;
    GLuint locMatrMVPWater, locTexWater, locTime;

    {
        std::string waterVSCode = readFile("shaders/water.vs.glsl");
        std::string waterFSCode = readFile("shaders/water.fs.glsl");

        Shader waterVertexShader(GL_VERTEX_SHADER, waterVSCode.c_str());
        waterProg.attachShader(waterVertexShader);

        Shader waterFragmentShader(GL_FRAGMENT_SHADER, waterFSCode.c_str());
        waterProg.attachShader(waterFragmentShader);

        waterProg.link();
        locMatrMVPWater = waterProg.getUniformLoc("mvp");
        locTexWater = waterProg.getUniformLoc("tex0");
        locTime = waterProg.getUniformLoc("time");
    }

    ShaderProgram skyProg;
    GLuint locMatrMVPSky, locTexSky;

    {
        std::string skyVSCode = readFile("shaders/skybox.vs.glsl");
        std::string skyFSCode = readFile("shaders/skybox.fs.glsl");

        Shader skyVertexShader(GL_VERTEX_SHADER, skyVSCode.c_str());
        skyProg.attachShader(skyVertexShader);

        Shader skyFragmentShader(GL_FRAGMENT_SHADER, skyFSCode.c_str());
        skyProg.attachShader(skyFragmentShader);

        skyProg.link();
        locMatrMVPSky = skyProg.getUniformLoc("mvp");
        locTexSky = skyProg.getUniformLoc("skybox");
    }

    Texture2D suzanneTex("models/suzanneTexture.png", GL_CLAMP_TO_EDGE);
    Texture2D treeTex("models/treeTexture.png", GL_CLAMP_TO_EDGE);
    Texture2D shroomTex("models/mushroomTexture.png", GL_CLAMP_TO_EDGE);
    Texture2D rockTex("models/rockTexture.png", GL_CLAMP_TO_EDGE);

    Texture2D groundTex("textures/groundSeamless.jpg", GL_REPEAT);
    groundTex.enableMipmap();

    Texture2D waterTex("textures/waterSeamless.jpg", GL_REPEAT);
    waterTex.enableMipmap();

    Texture2D heartTex("textures/heart.png", GL_REPEAT);

    const char* pathes[] = {
    "textures/skybox/Daylight Box_Right.bmp",
    "textures/skybox/Daylight Box_Left.bmp",
    "textures/skybox/Daylight Box_Top.bmp",
    "textures/skybox/Daylight Box_Bottom.bmp",
    "textures/skybox/Daylight Box_Front.bmp",
    "textures/skybox/Daylight Box_Back.bmp",
    };

    TextureCubeMap skyBoxTex(pathes);
    GL_CHECK_ERROR;

    Model suzanne("models/suzanne.obj");

    Model tree("models/tree.obj");

    Model shroom("models/mushroom.obj");

    Model rock("models/rock.obj");

    BasicShapeElements sol;
    sol.setData(solVertex, sizeof(solVertex), solIndices, sizeof(solIndices));
    sol.enableAttribute(0, 3, 5, 0);
    sol.enableAttribute(1, 2, 5, 3);
    

    BasicShapeElements ruisseau;
    ruisseau.setData(ruisseauVertex, sizeof(ruisseauVertex), ruisseauIndices, sizeof(ruisseauIndices));
    ruisseau.enableAttribute(0, 3, 5, 0);
    ruisseau.enableAttribute(1, 2, 5, 3);

    BasicShapeElements hud;
    hud.setData(hudVertex, sizeof(hudVertex), hudIndices, sizeof(hudIndices));
    hud.enableAttribute(0, 3, 5, 0);
    hud.enableAttribute(1, 2, 5, 3);

    BasicShapeElements skyBox;
    skyBox.setData(skyboxVertices, sizeof(skyboxVertices), skyboxIndices, sizeof(skyboxIndices));
    skyBox.enableAttribute(0, 3, 3, 0);


    const int N_ROWS = 7;
    const int N_GROUPS = N_ROWS * N_ROWS;

    glm::mat4 groupsTransform[N_GROUPS];

    glm::mat4 treeTransform[N_GROUPS];
    glm::mat4 rockTransform[N_GROUPS];
    glm::mat4 shroomTransform[N_GROUPS];

    float x, z, scaleFactor = 0;
    glm::mat4 translate, rotate, scale;

    for (int i = 0; i < N_GROUPS; ++i) {
        // Transformations du groupe
        getGroupRandomPos(i, N_ROWS, x, z);
        translate = glm::translate(glm::mat4(1.0f), { x, -1, z });
        rotate = glm::rotate(glm::mat4(1.0f), glm::radians(float(rand01() * 360)), { 0, 1, 0 });

        scaleFactor = 0.7f + rand01() * (1.3f - 0.7f);
        scale = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor));

        groupsTransform[i] = translate * rotate * scale;


        // Transformations de l'arbre
        scaleFactor = 0.7f + rand01() * (1.3f - 0.7f);
        scale = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor));

        treeTransform[i] = scale;
        

        // Transformations du champignon
        translate = glm::translate(glm::mat4(1.0f), { 0.3f, 0.0f, 0.3f });

        shroomTransform[i] = scale * translate;
        
        scale = glm::inverse(scale);

        shroomTransform[i] = shroomTransform[i] * scale;

        scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f));

        shroomTransform[i] = shroomTransform[i] * scale;


        // Transformations de la roche
        translate = glm::translate(glm::mat4(1.0f), { rand01() + 1.0f, 0.2f, 0.0f});
        scaleFactor = 0.3f;
        scale = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor));

        rockTransform[i] = rotate * translate * scale;
    }    

    glm::vec3 playerPosition = glm::vec3(0);
    glm::vec2 playerOrientation = glm::vec2(0);

    Camera camera(playerPosition, playerOrientation);

    bool isFirstPersonCam = false;


    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    int xMouse = 0, yMouse = 0;
    
    glm::mat4 model, view, proj;
    
    int selectShape = 0;
    bool isRunning = true;
    while (isRunning)
    {
        if (w.shouldResize())
            glViewport(0, 0, w.getWidth(), w.getHeight());
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        modelProg.use();

        w.getMouseMotion(xMouse, yMouse);
        handleMouseMotion(xMouse, yMouse, playerOrientation);

        handleKeyboardPress(w, playerPosition, playerOrientation);

        if (w.getMouseScrollDirection() != 0)
            isFirstPersonCam = w.getMouseScrollDirection() > 0;
        
        model = glm::mat4(1.0f);
        view = isFirstPersonCam ? camera.getFirstPersonViewMatrix() : camera.getThirdPersonViewMatrix();
        proj = glm::perspective(glm::radians(70.0f), (float)w.getWidth() / (float)w.getHeight(), 0.1f, 200.0f);

        // Draw Suzanne
        if (!isFirstPersonCam) {
            model = modelMatrixSuzanne(playerPosition, playerOrientation);
            glUniformMatrix4fv(locMatrMVPModel, 1, GL_FALSE, glm::value_ptr(proj * view * model));
            suzanneTex.use();
            suzanne.draw();
            suzanneTex.unuse();
        }

        // Draw ruisseau
        waterProg.use();
        glUniformMatrix4fv(locMatrMVPWater, 1, GL_FALSE, glm::value_ptr(proj * view));
        glUniform1f(locTime, w.getTick() / 1000.0f);
        waterTex.use();
        ruisseau.draw(GL_TRIANGLES, 6);
        waterTex.unuse();


        // Draw sol
        modelProg.use();
        glUniformMatrix4fv(locMatrMVPModel, 1, GL_FALSE, glm::value_ptr(proj * view));
        groundTex.use();
        sol.draw(GL_TRIANGLES, 6);
        groundTex.unuse();

        // Draw model 
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        for (int i = 0; i < N_GROUPS; ++i) {
            glUniformMatrix4fv(locMatrMVPModel, 1, GL_FALSE, glm::value_ptr(proj* view* groupsTransform[i] * treeTransform[i]));
            treeTex.use();
            tree.draw();
            treeTex.unuse();

            glUniformMatrix4fv(locMatrMVPModel, 1, GL_FALSE, glm::value_ptr(proj* view* groupsTransform[i] * rockTransform[i]));
            rockTex.use();
            rock.draw();
            rockTex.unuse();

            glUniformMatrix4fv(locMatrMVPModel, 1, GL_FALSE, glm::value_ptr(proj* view* groupsTransform[i] * shroomTransform[i]));
            shroomTex.use();
            shroom.draw();
            shroomTex.unuse();
        }
        glDisable(GL_CULL_FACE);

        // HUD (carré/coeur dans l'écran)
        glDepthFunc(GL_ALWAYS);
        model = glm::translate(glm::mat4(1.0f), { w.getWidth() / -2.0f, w.getHeight() / -2.0f, 0 });
        model = glm::translate(model, { 100.0f / 4, 100.0f / 4, 0 });
        proj = glm::ortho((float)w.getWidth() / -2, (float)w.getWidth() / 2, (float)w.getHeight() / -2, (float)w.getHeight() / 2, -1.0f, 200.0f);

        glUniformMatrix4fv(locMatrMVPModel, 1, GL_FALSE, glm::value_ptr(proj * model));

        heartTex.use();
        hud.draw(GL_TRIANGLES, 6);
        heartTex.unuse();


        // SkyBox
        glDepthFunc(GL_EQUAL);
        glDepthMask(GL_FALSE);

        skyProg.use();

        view = glm::mat4(glm::mat3(view));

        proj = glm::perspective(glm::radians(70.0f), (float)w.getWidth() / (float)w.getHeight(), 0.1f, 200.0f);
        
        glUniformMatrix4fv(locMatrMVPSky, 1, GL_FALSE, glm::value_ptr(proj * view));
        skyBoxTex.use();
        skyBox.draw(GL_TRIANGLES, 36);

        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);


        
        w.swap();
        w.pollEvent();
        isRunning = !w.shouldClose() && !w.getKeyPress(Window::Key::ESC);
    }

    return 0;
}



void checkGLError(const char* file, int line)
{
    GLenum error;
    while ( ( error = glGetError() ) != GL_NO_ERROR )
    {
        std::cerr << "GL_ERROR, File " << file << " (Line " << line << "): ";
        switch ( error )
        {
        case GL_INVALID_ENUM:
            std::cerr << "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            std::cerr << "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            std::cerr << "GL_INVALID_OPERATION";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
        case GL_OUT_OF_MEMORY:
            std::cerr << "GL_OUT_OF_MEMORY";
            break;
        case GL_STACK_UNDERFLOW:
            std::cerr << "GL_STACK_UNDERFLOW";
            break;
        case GL_STACK_OVERFLOW:
            std::cerr << "GL_STACK_OVERFLOW";
            break;
        default:
            std::cerr << "Unknown gl error occured!";
            break;
        }
        std::cerr << std::endl;
    }    
}

void printGLInfo()
{
    std::cout << "OpenGL info:" << std::endl;
    std::cout << "    Vendor: "   << glGetString(GL_VENDOR)   << std::endl;
    std::cout << "    Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "    Version: "  << glGetString(GL_VERSION)  << std::endl;
    std::cout << "    Shading version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

std::string readFile(const char* path)
{
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void handleMouseMotion(const int& xMouse, const int& yMouse, glm::vec2& playerOrientation) {
    playerOrientation.x += yMouse * 0.01f;
    playerOrientation.y += xMouse * 0.01f;
}

void handleKeyboardPress(Window& w, glm::vec3& playerPosition, const glm::vec2& orientation) {
    glm::vec3 moveForward(sin(glm::radians(orientation.y * -1)), 0.0f, cos(glm::radians(orientation.y * -1)));
    glm::vec3 moveRight(sin(glm::radians((orientation.y + 90) * -1)), 0.0f, cos(glm::radians((orientation.y + 90) * -1)));

    if (w.getKeyHold(Window::Key::S))
        playerPosition += 0.1f * moveForward;
    if (w.getKeyHold(Window::Key::W))
        playerPosition -= 0.1f * moveForward;
    if (w.getKeyHold(Window::Key::D))
        playerPosition -= 0.1f * moveRight;
    if (w.getKeyHold(Window::Key::A))
        playerPosition += 0.1f * moveRight;
}

glm::mat4 modelMatrixSuzanne(const glm::vec3& pos, const glm::vec2& orientation) {
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), { pos.x, -1.0, pos.z });
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), { 0, 1, 0 });
    rotate = glm::rotate(rotate, glm::radians(orientation.y * -1), { 0, 1, 0 });
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

    return translate * rotate * scale;
}
