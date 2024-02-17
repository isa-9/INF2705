
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "window.h"
#include "shader_program.h"
#include "vertices_data.h"
#include "shapes.h"
#include "utils.h"
#include "camera.h"
#include "model.h"


void printGLInfo();

#define GL_CHECK_ERROR checkGLError(__FILE__, __LINE__)
void checkGLError(const char* file, int line);

std::string readFile(const char* path);

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
    
    /*printGLInfo();
    
    ShaderProgram transProg;
    GLuint locMatrMVP;

    {
        std::string transVertexShaderCode = readFile("shaders/tp1/transform.vs.glsl");
        std::string transFragmentShaderCode = readFile("shaders/tp1/transform.fs.glsl");

        Shader transVertexShader(GL_VERTEX_SHADER, transVertexShaderCode.c_str());
        transProg.attachShader(transVertexShader);

        Shader transFragmentShader(GL_FRAGMENT_SHADER, transFragmentShaderCode.c_str());
        transProg.attachShader(transFragmentShader);

        transProg.link();
        locMatrMVP = transProg.getUniformLoc("mvp");
    }*/

    ShaderProgram modelProg;
    GLuint locMatrMVP, locColor;

    {
        std::string modelVSCode = readFile("shaders/model.vs.glsl");
        std::string modelFSCode = readFile("shaders/model.fs.glsl");

        Shader transVertexShader(GL_VERTEX_SHADER, modelVSCode.c_str());
        modelProg.attachShader(transVertexShader);

        Shader transFragmentShader(GL_FRAGMENT_SHADER, modelFSCode.c_str());
        modelProg.attachShader(transFragmentShader);

        modelProg.link();
        locMatrMVP = modelProg.getUniformLoc("mvp");
        locColor = modelProg.getUniformLoc("color");
    }
    // Variables pour la mise à jour, ne pas modifier.
    float angleDeg = 0.0f;

    BasicShapeElements cube;
    cube.setData(cubeVertices, sizeof(cubeVertices), cubeIndexes, sizeof(cubeIndexes));
    cube.enableAttribute(0, 3, 6, 0);
    cube.enableAttribute(1, 3, 6, 3);

    Model suzanne("models/suzanne.obj");
    GL_CHECK_ERROR;

    BasicShapeElements sol;
    sol.setData(solVertex, sizeof(solVertex), solIndices, sizeof(solIndices));
    sol.enableAttribute(0, 3, 6, 0);
    sol.enableAttribute(1, 3, 6, 3);

    BasicShapeElements ruisseau;
    ruisseau.setData(ruisseauVertex, sizeof(ruisseauVertex), ruisseauIndices, sizeof(ruisseauIndices));
    ruisseau.enableAttribute(0, 3, 6, 0);
    ruisseau.enableAttribute(1, 3, 6, 3);


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

        shroomTransform[i] = shroomTransform[i] * scale; // scale(arbre) * translate * (scale(arbre) ^(-1)) * scale(0.05)


        // Transformations de la roche
        translate = glm::translate(glm::mat4(1.0f), { rand01() + 1.0f, 0.2f, 0.0f});
        scaleFactor = 0.3f;
        scale = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor));

        rockTransform[i] = rotate * translate * scale;
    }

    //glm::mat4 proj = glm::perspective(glm::radians(70.0f), (float)w.getWidth() / (float)w.getHeight(), 0.1f, 200.0f);
    

    glm::vec3 playerPosition = glm::vec3(0);
    glm::vec2 playerOrientation = glm::vec2(0);

    // ...

    bool isFirstPersonCam = false;


    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    
    
    
    int selectShape = 0;
    bool isRunning = true;
    while (isRunning)
    {
        if (w.shouldResize())
            glViewport(0, 0, w.getWidth(), w.getHeight());
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        modelProg.use();
        
        angleDeg += 0.5f;
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(angleDeg), glm::vec3(0.1, 1, 0.1));
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, -0.5, -2));
        glm::mat4 proj = glm::perspective(glm::radians(70.0f), (float)w.getWidth() / (float)w.getHeight(), 0.1f, 10.0f);

        glm::mat4 matrix = proj * view * model;

        glUniformMatrix4fv(locMatrMVP, 1, GL_FALSE, glm::value_ptr(matrix));
        glUniform3fv(locColor, 1, glm::value_ptr(glm::vec3(1.0f)));

        //cube.draw(GL_TRIANGLES, 36);
        suzanne.draw();
        GL_CHECK_ERROR;
        //sol.draw(GL_TRIANGLES, 6);
        
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
