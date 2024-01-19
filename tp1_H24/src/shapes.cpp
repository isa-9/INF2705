#include "shapes.h"

BasicShapeArrays::BasicShapeArrays(const GLfloat* data, GLsizeiptr byteSize)
{
    // TODO Partie 1: Générer et bind le vao de la forme.
    
    // TODO Partie 1: Générer et bind le vbo de la forme.
    // Allouer l'espace nécessaire dans le mode approprié
    // et envoyer les données au gpu.      
}

BasicShapeArrays::~BasicShapeArrays()
{
    // TODO Partie 1: Supprimer la mémoire de l'objet.
    // Assurez-vous que les ressources ne soient pas utilisées
    // pendant la suppression.
}

void BasicShapeArrays::enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset)
{
    // TODO Partie 1: Activer un attribut et l'attacher correctement au state du vao.
}

void BasicShapeArrays::draw(GLenum mode, GLsizei count)
{
    // TODO Partie 1: Dessiner la forme.
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
    // TODO Partie 1: Générer et bind le vao de la forme.
    
    // TODO Partie 1: Générer et bind le vbo et ebo de la forme.
    // Allouer l'espace nécessaire dans le mode approprié
    // et envoyer les données au gpu.
}

BasicShapeElements::~BasicShapeElements()
{
    // TODO Partie 1: Supprimer la mémoire de l'objet.
    // Assurez-vous que les ressources ne soient pas utilisées
    // pendant la suppression.
}

void BasicShapeElements::enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset)
{
    // TODO Partie 1: Activer un attribut et l'attacher correctement au state du vao.
}

void BasicShapeElements::draw(GLenum mode, GLsizei count)
{
    // TODO Partie 1: Dessiner la forme avec le ebo.
}
