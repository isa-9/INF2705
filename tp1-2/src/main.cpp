
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
    
    printGLInfo();
    
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
    }
    // Variables pour la mise à jour, ne pas modifier.
    float angleDeg = 0.0f;

    BasicShapeElements cube;
    cube.setData(cubeVertices, sizeof(cubeVertices), cubeIndexes, sizeof(cubeIndexes));
    cube.enableAttribute(0, 3, 6, 0);
    cube.enableAttribute(1, 3, 6, 3);


    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    
    
    int selectShape = 0;
    bool isRunning = true;
    while (isRunning)
    {
        if (w.shouldResize())
            glViewport(0, 0, w.getWidth(), w.getHeight());
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        transProg.use();
        
        angleDeg += 0.5f;
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(angleDeg), glm::vec3(0.1, 1, 0.1));
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, -0.5, -2));
        glm::mat4 proj = glm::perspective(glm::radians(70.0f), (float)w.getWidth() / (float)w.getHeight(), 0.1f, 10.0f);

        glm::mat4 matrix = proj * view * model;

        glUniformMatrix4fv(locMatrMVP, 1, GL_FALSE, glm::value_ptr(matrix));

        cube.draw(GL_TRIANGLES, 36);
        
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
