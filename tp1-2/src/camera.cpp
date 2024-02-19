#include "camera.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3& position, const glm::vec2& orientation) : m_position(position), m_orientation(orientation)
{
	// TODO: Initialisation des attributs de la classe
}

glm::mat4 Camera::getFirstPersonViewMatrix()
{
	// TODO: Calcul et retour de la matrice de vue en première personne
	// à partir des attributs de la classe
	glm::mat4 position = glm::translate(glm::mat4(1.0f), m_position);

	// orientation = (x, y)
	glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(m_orientation.x), { 1, 0, 0 });
	glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(m_orientation.y), { 0, 1, 0 });

	return glm::inverse(rotationY * rotationX * position);
}

glm::mat4 Camera::getThirdPersonViewMatrix()
{
	// TODO: Calcul et retour de la matrice de vue en troisième personne
	// à partir des attributs de la classe

	// On ajoute 180 degrés, car si m_orientation.y == 0 -> il pointe vers l'axe z
	float orientationY = glm::radians(m_orientation.y + 90);
	float radius = 6;
	//glm::vec3 pos_cam = glm::vec3(cos(orientationY) * radius,
	//	0.0f,//cos(orientationY) * radius,
	//	sin(glm::radians(m_orientation.x)) * radius);

	glm::vec3 pos_cam = m_position + glm::vec3(cos(orientationY) * radius * cos(glm::radians(m_orientation.x)),
		sin(glm::radians(m_orientation.x)) * radius,
		sin(orientationY) * radius * cos(glm::radians(m_orientation.x)));

	return glm::lookAt(pos_cam, m_position, { 0.0, 1.0, 0.0 });
}

