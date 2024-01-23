#include "shader_program.h"

#include <iostream>

Shader::Shader(GLenum type, const char* code)
{
    // TODO Partie 1: Créer et compiler le code du shader.
    // Il est toujours bonne pratique de vérifier s'il y a
    // des erreurs à la fin.
    m_id = glCreateShader(type);
    glShaderSource(m_id, 1, &code, NULL);
    glCompileShader(m_id);
    checkError();
}
    
Shader::~Shader()
{
    // TODO Partie 1: Suppression du shader.
    glDeleteShader(m_id);
}

GLuint Shader::id()
{
    // TODO Partie 1: Retourner l'id du shader.
    return m_id;
}

void Shader::checkError()
{
    GLint success;
    GLchar infoLog[1024];

    glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(m_id, 1024, NULL, infoLog);
        glDeleteShader(m_id);
        std::cout << "Shader compile error: " << infoLog << std::endl;
    }
}



ShaderProgram::ShaderProgram()
{
    // TODO Partie 1: Créer un programme d'OpenGL.
    m_id = glCreateProgram();
}
    
ShaderProgram::~ShaderProgram()
{        
    // TODO Partie 1: Suppression du programme d'OpenGL.
    glDeleteProgram(m_id);
}
    
void ShaderProgram::use()
{
    // TODO Partie 1: Utiliser le programme pour le rendu.
    glUseProgram(m_id);
}
    
void ShaderProgram::attachShader(Shader& s)
{        
    // TODO Partie 1: Attacher un nouveau shader au programme.
    glAttachShader(m_id, s.id());
}
    
void ShaderProgram::link()
{        
    // TODO Partie 1: Faire le linking du programme.
    // N'oublier pas de tester pour les erreurs.
    glLinkProgram(m_id);
    checkError();
}

GLint ShaderProgram::getUniformLoc(const char* name)
{
    // TODO Partie 2: Retourne la location d'un uniform dans le programme
}
    
void ShaderProgram::checkError()
{
    GLint success;
    GLchar infoLog[1024];

    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_id, 1024, NULL, infoLog);
        glDeleteProgram(m_id);
        std::cout << "Program linking error: " << infoLog << std::endl;
    }
}
