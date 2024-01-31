
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

// Vertex Shader
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    
    out vec3 color;
    
    void main() {
        gl_Position = vec4(aPos, 1.0);
        color = aColor;
    }
)";

// Fragment Shader
const char* fragmentShaderSource = R"(
    #version 330 core
    in vec3 color;
    out vec4 FragColor;
    
    void main() {
        FragColor = vec4(color, 1.0);
    }
)";

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
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,   //1.0f, 0.0f, 0.0f, // Bas gauche, rouge
         0.5f, -0.5f, 0.0f,   //1.0f, 0.0f, 0.0f, // Bas droite, rouge
         0.0f,  0.5f, 0.0f   //1.0f, 0.0f, 0.0f  // Haut centre, rouge
    };

    // Création des buffers et des VAO
    //GLuint VBO, VAO;
    //glGenVertexArrays(1, &VAO);
    //glGenBuffers(1, &VBO);

    //// Liaison du VAO
    //glBindVertexArray(VAO);

    //// Remplissage du VBO
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    BasicShapeArrays triangle1(vertices, sizeof(vertices));

    // Attribut de position
   /* glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);*/
    triangle1.enableAttribute(0, 3, 3 * sizeof(float), 0);

    // Attribut de couleur
    /*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);*/

    basicProg.use();

    
    int selectShape = 0;
    bool isRunning = true;
    while (isRunning)
    {
        if (w.shouldResize())
            glViewport(0, 0, w.getWidth(), w.getHeight());
        GL_CHECK_ERROR;
        
        glClear(GL_COLOR_BUFFER_BIT);

        // Rendu du triangle
        /*glBindVertexArray(triangle1.m_vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);*/
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
