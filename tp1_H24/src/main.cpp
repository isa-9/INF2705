
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

void changeRGB(GLfloat* color);
void changePos(GLfloat* pos, float& cx, float& cy, float& dx, float& dy);

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
    
    ShaderProgram basicProg;

    {
        std::string basicVertexShaderCode = readFile("shaders/basic.vs.glsl");
        std::string basicFragmentShaderCode = readFile("shaders/basic.fs.glsl");

        
        Shader basicVertexShader(GL_VERTEX_SHADER, basicVertexShaderCode.c_str());
        basicProg.attachShader(basicVertexShader);

        Shader basicFragmentShader(GL_FRAGMENT_SHADER, basicFragmentShaderCode.c_str());
        basicProg.attachShader(basicFragmentShader);

        basicProg.link();
    }

    ShaderProgram colorProg;

    {
        std::string colorVertexShaderCode = readFile("shaders/color.vs.glsl");
        std::string colorFragmentShaderCode = readFile("shaders/color.fs.glsl");

        Shader colorVertexShader(GL_VERTEX_SHADER, colorVertexShaderCode.c_str());
        colorProg.attachShader(colorVertexShader);

        Shader colorFragmentShader(GL_FRAGMENT_SHADER, colorFragmentShaderCode.c_str());
        colorProg.attachShader(colorFragmentShader);

        colorProg.link();
    }
    
    ShaderProgram transProg;
    GLuint locMatrMVP;

    {
        std::string transVertexShaderCode = readFile("shaders/transform.vs.glsl");
        std::string transFragmentShaderCode = readFile("shaders/transform.fs.glsl");

        Shader transVertexShader(GL_VERTEX_SHADER, transVertexShaderCode.c_str());
        transProg.attachShader(transVertexShader);

        Shader transFragmentShader(GL_FRAGMENT_SHADER, transFragmentShaderCode.c_str());
        transProg.attachShader(transFragmentShader);

        transProg.link();
        locMatrMVP = transProg.getUniformLoc("mvp");
    }
    // Variables pour la mise à jour, ne pas modifier.
    float cx = 0, cy = 0;
    float dx = 0.019f;
    float dy = 0.0128f;
    
    float angleDeg = 0.0f;
    
    // Tableau non constant de la couleur
    GLfloat onlyColorTriVertices[] = {
        0.0, 0.0, 1.0,
        0.0, 1.0, 0.0,
        1.0, 0.0, 0.0
    };
   
    BasicShapeArrays triangle1(triVertices, sizeof
    (triVertices));
    triangle1.enableAttribute(0, 3, 0, 0);

    BasicShapeArrays square1(squareVertices, sizeof(squareVertices));
    square1.enableAttribute(0, 3, 0, 0);

    BasicShapeArrays coloredTriangle1(colorTriVertices, sizeof(colorTriVertices));
    coloredTriangle1.enableAttribute(0, 3, 6, 0);
    coloredTriangle1.enableAttribute(1, 3, 6, 3);

    BasicShapeArrays coloredSquare(colorSquareVertices, sizeof(colorSquareVertices));
    coloredSquare.enableAttribute(0, 3, 6, 0);
    coloredSquare.enableAttribute(1, 3, 6, 3);

    BasicShapeMultipleArrays coloredTriangle2(triVertices, sizeof(triVertices), onlyColorTriVertices, sizeof(onlyColorTriVertices));
    coloredTriangle2.enablePosAttribute(0, 3, 0, 0);
    coloredTriangle2.enableColorAttribute(1, 3, 0, 0);

    BasicShapeElements coloredSquare2(colorSquareVerticesReduced, sizeof(colorSquareVerticesReduced), indexes, sizeof(indexes));
    coloredSquare2.enableAttribute(0, 3, 6, 0);
    coloredSquare2.enableAttribute(1, 3, 6, 3);

    BasicShapeElements cube(cubeVertices, sizeof(cubeVertices), cubeIndexes, sizeof(cubeIndexes));
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
        
        if (w.getKey(Window::Key::T))
        {
            selectShape = ++selectShape < 7 ? selectShape : 0;
            std::cout << "Selected shape: " << selectShape << std::endl;
        }
        
        changeRGB(&onlyColorTriVertices[0]);
        changeRGB(&onlyColorTriVertices[3]);
        changeRGB(&onlyColorTriVertices[6]);

        coloredTriangle2.updateColorData(onlyColorTriVertices, sizeof(onlyColorTriVertices));

        GLfloat* posPtr = coloredTriangle2.mapPosData();
        changePos(posPtr, cx, cy, dx, dy);
        coloredTriangle2.unmapPosData();
        
        switch (selectShape)
        {
        case 0:
        case 1:
            basicProg.use();
            break;

        case 2:
        case 3:
        case 4:
        case 5:
            colorProg.use();
            break;      
        case 6:
            transProg.use();
            break;
        }
        
        if (selectShape == 6)
        {
            angleDeg += 0.5f;
            glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(angleDeg), glm::vec3(0.1, 1, 0.1));
            glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, -0.5, -2));
            glm::mat4 proj = glm::perspective(glm::radians(70.0f), (float)w.getWidth() / (float)w.getHeight(), 0.1f, 10.0f);

            glm::mat4 matrix = proj * view * model;

            glUniformMatrix4fv(locMatrMVP, 1, GL_FALSE, glm::value_ptr(matrix));
        }
        switch (selectShape)
        {
        case 0:
            triangle1.draw(GL_TRIANGLES, 3);
            break;
        case 1:
            square1.draw(GL_TRIANGLES, 6);
            break;
        case 2:
            coloredTriangle1.draw(GL_TRIANGLES, 3);
            break;
        case 3:
            coloredSquare.draw(GL_TRIANGLES, 6);
            break;
        case 4:
            coloredTriangle2.draw(GL_TRIANGLES, 3);
            break;
        case 5:
            coloredSquare2.draw(GL_TRIANGLES, 6);
            break;
        case 6:
            cube.draw(GL_TRIANGLES, 36);
            break;
        }
        
        w.swap();
        w.pollEvent();
        isRunning = !w.shouldClose() && !w.getKey(Window::Key::ESC);
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


void changeRGB(GLfloat* color)
{
    unsigned char r = color[0]*255;
    unsigned char g = color[1]*255;
    unsigned char b = color[2]*255;
    
    if(r > 0 && b == 0)
    {
        r--;
        g++;
    }
    if(g > 0 && r == 0)
    {
        g--;
        b++;
    }
    if(b > 0 && g == 0)
    {
        r++;
        b--;
    }
    color[0] = r/255.0f;
    color[1] = g/255.0f;
    color[2] = b/255.0f;
}

void changePos(GLfloat* pos, float& cx, float& cy, float& dx, float& dy)
{
    if ((cx < -1 && dx < 0) || (cx > 1 && dx > 0))
        dx = -dx;
    pos[0] += dx;
    pos[3] += dx;
    pos[6] += dx;
    cx += dx;
    if ((cy < -1 && dy < 0) || (cy > 1 && dy > 0))
        dy = -dy;
    pos[1] += dy;
    pos[4] += dy;
    pos[7] += dy;
    cy += dy;
}


std::string readFile(const char* path)
{
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
