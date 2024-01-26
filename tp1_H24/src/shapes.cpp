#include "shapes.h"

BasicShapeArrays::BasicShapeArrays(const GLfloat* data, GLsizeiptr byteSize)
{
    GLuint locPosition = 0;
    GLint sizePosition = 3;

    // TODO Partie 1: Générer et bind le vao de la forme.
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // TODO Partie 1: Générer et bind le vbo de la forme.
    // Allouer l'espace nécessaire dans le mode approprié
    // et envoyer les données au gpu.
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, byteSize, data, GL_STATIC_DRAW);
    enableAttribute(locPosition, sizePosition, 0, 0);
    glBindVertexArray(0);
}

BasicShapeArrays::~BasicShapeArrays()
{
    // TODO Partie 1: Supprimer la mémoire de l'objet.
    // Assurez-vous que les ressources ne soient pas utilisées
    // pendant la suppression.
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}

void BasicShapeArrays::enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset)
{
    // TODO Partie 1: Activer un attribut et l'attacher correctement au state du vao.
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, &offset);
    glEnableVertexAttribArray(index);
}

void BasicShapeArrays::draw(GLenum mode, GLsizei count)
{
    // TODO Partie 1: Dessiner la forme.
    glBindVertexArray(m_vao);
    glDrawArrays(mode, 0, count);
    glBindVertexArray(0);

}



BasicShapeMultipleArrays::BasicShapeMultipleArrays(const GLfloat* pos, GLsizeiptr posByteSize, const GLfloat* color, GLsizeiptr colorByteSize)
{
    // TODO Partie 1: Générer et bind le vao de la forme.
    
    // TODO Partie 1: Générer et bind les vbos (un pour la position,
    // l'autre pour la couleur) de la forme. Allouer l'espace
    // nécessaire dans le mode approprié et envoyer les données au gpu.
}

BasicShapeMultipleArrays::~BasicShapeMultipleArrays()
{
    // TODO Partie 1: Supprimer la mémoire de l'objet.
    // Assurez-vous que les ressources ne soient pas utilisées
    // pendant la suppression.
}

void BasicShapeMultipleArrays::enablePosAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset)
{
    // TODO Partie 1: Activer l'attribut de position et l'attacher correctement au state du vao.
}

void BasicShapeMultipleArrays::enableColorAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset)
{
    // TODO Partie 1: Activer l'attribut de couleur et l'attacher correctement au state du vao.
}

void BasicShapeMultipleArrays::updateColorData(const GLfloat* color, GLsizeiptr colorByteSize)
{
    // TODO Partie 1: Modifier la totalité des données de couleur
}

GLfloat* BasicShapeMultipleArrays::mapPosData()
{
    // TODO Partie 1: Activer le mapping des données de position
}

void BasicShapeMultipleArrays::unmapPosData()
{
    // TODO Partie 1: Désactiver le mapping des données de position
}

void BasicShapeMultipleArrays::draw(GLenum mode, GLsizei count)
{
    // TODO Partie 1: Dessiner la forme.
}



BasicShapeElements::BasicShapeElements(const GLfloat* data, GLsizeiptr byteSize, const GLubyte* indexes, GLsizeiptr indexesByteSize)
{
    GLuint locPosition = 0;
    GLuint locColor = 1;

    GLint sizePosition = 3;
    GLint sizeColor = 4;
    
    // TODO Partie 1: Générer et bind le vao de la forme.
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    
    // TODO Partie 1: Générer et bind le vbo et ebo de la forme.
    // Allouer l'espace nécessaire dans le mode approprié
    // et envoyer les données au gpu.
    
    // VBO
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    enableAttribute(locPosition, sizePosition, sizeof(data), 0);
    enableAttribute(locColor, sizeColor, sizeof(data), (sizePosition * sizeof(float)));

    // EBO
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

BasicShapeElements::~BasicShapeElements()
{
    // TODO Partie 1: Supprimer la mémoire de l'objet.
    // Assurez-vous que les ressources ne soient pas utilisées
    // pendant la suppression.
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}

void BasicShapeElements::enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset)
{
    // TODO Partie 1: Activer un attribut et l'attacher correctement au state du vao.
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, &offset);
    glEnableVertexAttribArray(index);
}

void BasicShapeElements::draw(GLenum mode, GLsizei count)
{
    // TODO Partie 1: Dessiner la forme avec le ebo.
    glBindVertexArray(m_vao);
    glDrawElements(mode, count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
