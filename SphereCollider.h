#ifndef _SPHERE_COLLIDER_H_
#define _SPHERE_COLLIDER_H_

#include "Collider.h"

class SphereCollider : public Collider
{
private:
	glm::vec3 center;
	float radius;
	glm::vec3 worldCenter;

public:
	SphereCollider(Transform* parentTransform, glm::vec3 center, float radius, bool colliderIsKinematic);
	~SphereCollider();

	void draw(GLuint shader, const glm::mat4& C);
	void update(const glm::mat4& C);
	glm::vec3 getWorldCenter() { return worldCenter; }
	float getRadius() { return radius; }
	ColliderType getType() { return ColliderType::SPHERE; }
};

#endif