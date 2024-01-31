
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
    
    // TODO Partie 1: Instancier les shader programs ici.
    
    ShaderProgram basicProg;
    GL_CHECK_ERROR;

    // ... basic;
    { // Les accolades vont permettre de détruire le code des shaders plus rapidement
        // Vous devez lire le code des shaders dans "shaders/"
        // avec la fonction "readFile".
        std::string basicVertexShaderCode = readFile("shaders/basic.vs.glsl");
        std::string basicFragmentShaderCode = readFile("shaders/basic.fs.glsl");

        
        // Vous pouvez par la suite instancier vos shaders, les attacher et les lier
        // au programme.
        Shader basicVertexShader(GL_VERTEX_SHADER, basicVertexShaderCode.c_str());
        basicProg.attachShader(basicVertexShader);

        Shader basicFragmentShader(GL_FRAGMENT_SHADER, basicFragmentShaderCode.c_str());
        basicProg.attachShader(basicFragmentShader);

        basicProg.link();
    } 
    
    // TODO Partie 1: Instancier vos formes ici.
   
    // Param byteSize doit être std::size(trivertices)
    BasicShapeArrays triangle1(triVertices, sizeof
    (triVertices));
    // enable et reste appelé ici!!!
    triangle1.enableAttribute(0, 3, 3 * sizeof(GLfloat), 0);

    /*BasicShapeArrays square1(squareVertices, std::size(squareVertices));
    square1.enableAttribute(0, 3, 0, 0);

    BasicShapeArrays coloredTriangle1(colorTriVertices, std::size(colorTriVertices));
    coloredTriangle1.enableAttribute(0, 3, 6 * sizeof(GLfloat), 0);
    coloredTriangle1.enableAttribute(1, 3, 6 * sizeof(GLfloat), 3 * sizeof(GLfloat));

    BasicShapeArrays coloredSquare(colorSquareVertices, std::size(colorSquareVertices));
    coloredSquare.enableAttribute(0, 3, 6 * sizeof(GLfloat), 0);
    coloredSquare.enableAttribute(1, 3, 6 * sizeof(GLfloat), 3 * sizeof(GLfloat));

    BasicShapeMultipleArrays coloredTriangle2(triVertices, std::size(triVertices), onlyColorTriVertices, std::size(onlyColorTriVertices));
    coloredTriangle2.enablePosAttribute(0, 3, 0, 0);
    coloredTriangle2.enableColorAttribute(1, 3, 0, 0);

    BasicShapeElements coloredSquare2(colorSquareVerticesReduced, std::size(colorSquareVerticesReduced), indexes, std::size(indexes));
    coloredSquare2.enableAttribute(0, 3, 6 * sizeof(GLfloat), 0);
    coloredSquare2.enableAttribute(1, 3, 6 * sizeof(GLfloat), 3 * sizeof(GLfloat));*/

    // TODO Partie 2: Instancier le cube ici.
    // ...
    
    // TODO Partie 1: Donner une couleur de remplissage aux fonds.
    glClearColor(1.0, 1.0, 1.0, 1.0);
    
    // TODO Partie 2: Activer le depth test.
    glEnable(GL_DEPTH_TEST);
    
    int selectShape = 0;
    bool isRunning = true;
    while (isRunning)
    {
        if (w.shouldResize())
            glViewport(0, 0, w.getWidth(), w.getHeight());
        GL_CHECK_ERROR;
        
        // TODO Partie 1: Nettoyer les tampons appropriées.
        glClear(GL_COLOR_BUFFER_BIT);
        GL_CHECK_ERROR;
        
        // TODO Partie 1: Utiliser le bon shader programme selon la forme.
        // N'hésiter pas à utiliser le fallthrough du switch case.
        basicProg.use();
        GL_CHECK_ERROR;
        
        // TODO Partie 1: Dessiner la forme sélectionnée.
        triangle1.draw(GL_TRIANGLES, 3);
        
        w.swap();
        w.pollEvent();
        isRunning = !w.shouldClose() && !w.getKey(Window::Key::ESC);
    }
    GL_CHECK_ERROR;
    
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
