#include "SphereCollider.h"

SphereCollider::SphereCollider(Transform* parentTransform, glm::vec3 center, float radius, bool kinematic)
	: center(center)
	, radius(radius)
	, worldCenter(NULL)
	, kinematic(kinematic)
{
	transform = parentTransform;
}

SphereCollider::~SphereCollider()
{
}

void SphereCollider::draw(GLuint shader, const glm::mat4& C)
{
}

void SphereCollider::update(const glm::mat4& C)
{
	worldCenter = C * glm::vec4(center, 1.0);
}