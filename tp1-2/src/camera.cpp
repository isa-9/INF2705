#include "camera.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3& position, const glm::vec2& orientation)
{
	// TODO: Initialisation des attributs de la classe
}

glm::mat4 Camera::getFirstPersonViewMatrix()
{
	// TODO: Calcul et retour de la matrice de vue en première personne
	// à partir des attributs de la classe
}

glm::mat4 Camera::getThirdPersonViewMatrix()
{
	// TODO: Calcul et retour de la matrice de vue en troisième personne
	// à partir des attributs de la classe
}

