#ifndef _PLANE_COLLIDER_H_
#define _PLANE_COLLIDER_H_

#include "Collider.h"

class PlaneCollider : public Collider
{
private:
	glm::vec3 point;
	glm::vec3 normal;
	glm::vec3 worldPoint;
	glm::vec3 worldNormal;

public:
	PlaneCollider(Transform* parentTransform, glm::vec3 point, glm::vec3 normal);
	~PlaneCollider();

	void draw(GLuint shader, const glm::mat4& C);
	void update(const glm::mat4& C);
	glm::vec3 getWorldPoint() { return worldPoint; }
	glm::vec3 getWorldNormal() { return worldNormal; }
	ColliderType getType() { return ColliderType::PLANE; }
	bool isKinematic() { return true; }
};

#endif