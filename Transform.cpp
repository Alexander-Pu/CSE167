#include "Transform.h"

Transform::Transform(const glm::mat4 scale, const glm::mat4 rotation, const glm::vec3 location)
	: scale(scale)
	, rotation(rotation)
	, location(location)
	, worldLocation(NULL)
	, materials(NULL)
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
	if (materials) {
		materials->sendMatToShader(shader);
	}

	glm::mat4 transformMatrix = glm::translate(location) * rotation * scale;

	glm::mat4 newMatrix = C * transformMatrix;

	for (Node* child : children) {
		child->draw(shader, newMatrix);
	}
}

void Transform::update(const glm::mat4& C)
{
	glm::mat4 transformMatrix = glm::translate(location) * rotation * scale;

	glm::mat4 newMatrix = C * transformMatrix;

	worldLocation = C * glm::vec4(location, 1);

	for (Node* child : children) {
		child->update(newMatrix);
	}
}

void Transform::addChild(Node* child)
{
	children.push_back(child);
}