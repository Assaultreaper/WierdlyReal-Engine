#include "Transformation.h"

void Transform::Translate(glm::mat4 model, glm::vec3 Translate)
{
	model = glm::translate(model, Translate);
}

void Transform::Scale(glm::mat4 model, glm::vec3 scale)
{
	model = glm::scale(model, scale);
}
