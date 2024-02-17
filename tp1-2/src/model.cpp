#include "model.h"
#include "vertices_data.h"

#include "obj_loader.h"
#include <iostream>

Model::Model(const char* path) : m_shape()
{
	// TODO: Initalisation du modèle et des attibuts de la classe
	std::vector<GLfloat> vertexData;
	std::vector<GLuint> indices;

	loadObj(path, vertexData, indices);

	std::cout << vertexData.size() * 4 << std::endl;

	m_shape.setData(vertexData.data(), vertexData.size() * sizeof(GLfloat), indices.data(), indices.size() * sizeof(GLuint));
	m_shape.enableAttribute(0, 3, 3, 0);

	m_count = indices.size();
}

void Model::loadObj(const char* path, std::vector<GLfloat>& vertexData, std::vector<GLuint>& indices)
{
	objl::Loader loader;
	bool loadout = loader.LoadFile(path);
	if (!loadout)
	{
		std::cout << "Unable to load model " << path << std::endl;
		return;
	}

	for (size_t i = 0; i < loader.LoadedVertices.size(); i++)
	{
		objl::Vector3 p = loader.LoadedVertices[i].Position;
		vertexData.push_back(p.X);
		vertexData.push_back(p.Y);
		vertexData.push_back(p.Z);
		// TODO: Décommenter lors de la partie 2
		//objl::Vector2 t = loader.LoadedVertices[i].TextureCoordinate;
		//vertexData.push_back(t.X);
		//vertexData.push_back(t.Y);
	}
	indices = loader.LoadedIndices;
}

void Model::draw()
{
	// TODO: Dessine le modèle en triangle
	m_shape.draw(GL_TRIANGLES, m_count);
}
