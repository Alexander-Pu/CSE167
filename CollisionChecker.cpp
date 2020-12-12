#include "CollisionChecker.h"

CollisionChecker::CollisionChecker()
{
}

CollisionChecker::~CollisionChecker()
{
}

bool CollisionChecker::collides(Collider* colliderOne, Collider* colliderTwo) {
	bool collision = false;

	// Sphere & Sphere
	if (colliderOne->getType() == ColliderType::SPHERE && colliderTwo->getType() == ColliderType::SPHERE) {
		SphereCollider* sphereColliderOne = (SphereCollider*)colliderOne;
		SphereCollider* sphereColliderTwo = (SphereCollider*)colliderTwo;
		float sumRadii = sphereColliderOne->getRadius() + sphereColliderTwo->getRadius();

		collision = glm::distance(sphereColliderOne->getWorldCenter(), sphereColliderTwo->getWorldCenter()) < sumRadii;
	}

	// Sphere & Plane
	else if ((colliderOne->getType() == ColliderType::SPHERE && colliderTwo->getType() == ColliderType::PLANE)
		|| (colliderOne->getType() == ColliderType::PLANE && colliderTwo->getType() == ColliderType::SPHERE)) {

		SphereCollider* sphereCollider;
		PlaneCollider* planeCollider;

		if (colliderOne->getType() == ColliderType::SPHERE) {
			sphereCollider = (SphereCollider*)colliderOne;
			planeCollider = (PlaneCollider*)colliderTwo;
		}
		else {
			sphereCollider = (SphereCollider*)colliderTwo;
			planeCollider = (PlaneCollider*)colliderOne;
		}

		glm::vec3 spherePoint = sphereCollider->getWorldCenter();
		float sphereRadius = sphereCollider->getRadius();

		glm::vec3 planePoint = planeCollider->getWorldPoint();
		glm::vec3 planeNormal = planeCollider->getWorldNormal();

		float distance = glm::dot((spherePoint - planePoint), planeNormal);

		collision = glm::abs(distance) < sphereRadius;
	}

	return collision;
}
