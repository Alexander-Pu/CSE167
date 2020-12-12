#include "CollisionPusher.h"

CollisionPusher::CollisionPusher()
{
}

CollisionPusher::~CollisionPusher()
{
}

void CollisionPusher::resolveCollision(Collider* colliderOne, Collider* colliderTwo) {
	// Neither should react if both kinematic
	if (colliderOne->isKinematic() && colliderTwo->isKinematic()) {
		return;
	}
	
	// Sphere & Sphere
	if (colliderOne->getType() == ColliderType::SPHERE && colliderTwo->getType() == ColliderType::SPHERE) {
		SphereCollider* sphereColliderOne = (SphereCollider*)colliderOne;
		SphereCollider* sphereColliderTwo = (SphereCollider*)colliderTwo;

		// Assume one collides with two. Direction is the "push" direction of two on one
		glm::vec3 direction = glm::normalize(sphereColliderOne->getWorldCenter() - sphereColliderTwo->getWorldCenter());

		float sumRadii = sphereColliderOne->getRadius() + sphereColliderTwo->getRadius();
		float currentDistance = glm::distance(sphereColliderOne->getWorldCenter(), sphereColliderTwo->getWorldCenter());

		// We want to assume two is the immovable object
		glm::vec3 moveVector = direction * (sumRadii - currentDistance);

		// no vertical movement
		moveVector.y = 0;

		Transform* transformOne = colliderOne->getTransform();
		transformOne->applyTranslation(moveVector);
		
		return;
	}

	// Sphere & Plane
	if ((colliderOne->getType() == ColliderType::SPHERE && colliderTwo->getType() == ColliderType::PLANE)
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

		// We want to assume two is the immovable object
		glm::vec3 moveVector = planeNormal * (sphereRadius - distance);

		// no vertical movement
		moveVector.y = 0;

		Transform* transformOne = colliderOne->getTransform();
		transformOne->applyTranslation(moveVector);
	}
}
