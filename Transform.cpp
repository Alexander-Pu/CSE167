#include "Transform.h"

Transform::Transform(const glm::mat4 scale, const glm::mat4 rotation, const glm::vec3 location)
	: transformMatrix(glm::translate(location) * rotation * scale)
{
}

Transform::~Transform()
{
	for (Node* child : children) {
		delete child;
	}
}

void Transform::draw(GLuint shader, const glm::mat4& C)
{
	glm::mat4 newMatrix = C * transformMatrix;

	for (Node* child : children) {
		child->draw(shader, newMatrix);
	}
}

void Transform::update()
{
}

void Transform::addChild(Node* child)
{
	children.push_back(child);
}

void Transform::applyTransformation(glm::mat4 transformationMatrix)
{
	transformMatrix = transformMatrix * transformationMatrix;
}