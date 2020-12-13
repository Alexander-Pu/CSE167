#include "AstroAI.h"

AstroAI::AstroAI(Astronaut* astronaut)
	: astronaut(astronaut)
	, state(AIState::IDLE)
	, timeSinceLastStateChange(0)
	, desiredDir(glm::normalize(glm::vec3(rand(), 0, rand())))
{
}

AstroAI::~AstroAI()
{
}

void AstroAI::setState(AIState newState) {
	state = newState;
	timeSinceLastStateChange = 0;
}

void AstroAI::update() {
	if (state == AIState::IDLE) {
		if (rand() % 10000 < 2 && timeSinceLastStateChange > 1) {
			setState(AIState::MOVING);
		}
	}

	if (state == AIState::MOVING) {
		astronaut->move(desiredDir, true);

		if (rand() % 10000 < 2 && timeSinceLastStateChange > 1) {
			setState(AIState::IDLE);
			astronaut->stop();
		}
	}

	timeSinceLastStateChange += Time::deltaTime;
}

void AstroAI::handleCollisions(std::vector<std::pair<Collider*, Collider*>> colliderPairs) {
	SphereCollider* astroCollider = (SphereCollider*) astronaut->getCollider();
	glm::vec3 newDesiredDir = glm::vec3(0);

	for (std::pair<Collider*, Collider*> colliderPair : colliderPairs) {
		if (colliderPair.first != astronaut->getCollider() && colliderPair.second != astronaut->getCollider()) {
			continue;
		}

		Collider* otherCollider = colliderPair.first == astronaut->getCollider() ? colliderPair.second : colliderPair.first;

		glm::vec3 normalVector;

		// Handle collision with spheres
		if (otherCollider->getType() == ColliderType::SPHERE) {
			SphereCollider* otherColliderSphere = (SphereCollider*) otherCollider;
			glm::vec3 pushDirection = astroCollider->getWorldCenter() - otherColliderSphere->getWorldCenter();

			// no vertival 
			pushDirection.y = 0;

			normalVector = glm::normalize(pushDirection);
		}

		// Handle collision with planes
		if (otherCollider->getType() == ColliderType::PLANE) {
			PlaneCollider* otherColliderPlane = (PlaneCollider*) otherCollider;
			normalVector = otherColliderPlane->getWorldNormal();
		}

		glm::vec3 reflection = glm::reflect(desiredDir, normalVector);

		if (glm::dot(desiredDir, normalVector) < 0) {
			newDesiredDir += reflection;
		}
	}

	if (glm::length(newDesiredDir) != 0) {
		desiredDir = glm::normalize(newDesiredDir);
	}
}