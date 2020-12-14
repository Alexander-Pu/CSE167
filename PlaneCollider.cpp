#include "PlaneCollider.h"

PlaneCollider::PlaneCollider(Transform* parentTransform, glm::vec3 point, glm::vec3 normal)
	: point(point)
	, normal(normal)
	, worldPoint(NULL)
	, worldNormal(NULL)
{
	transform = parentTransform;
	kinematic = true;
}

PlaneCollider::~PlaneCollider()
{
}

void PlaneCollider::draw(GLuint shader, const glm::mat4& C)
{
}

void PlaneCollider::update(const glm::mat4& C)
{
	worldPoint = C * glm::vec4(point, 1.0);
	worldNormal = C * glm::vec4(normal, 0);
}