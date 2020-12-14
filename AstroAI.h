#ifndef _ASTRO_AI_H_
#define _ASTRO_AI_H_

#include "Astronaut.h"
#include "Controller.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"
#include "Time.h"

#include <utility>
#include <cstdlib>

enum class AIState {
	SPAWNING,
	IDLE,
	MOVING
};

class AstroAI
{
private:
	Astronaut* astronaut;
	AIState state;
	float timeSinceLastStateChange;
	glm::vec3 desiredDir;
public:
	AstroAI(Astronaut* astronaut);
	~AstroAI();

	void setState(AIState newState);
	void update();
	void handleCollisions(std::vector<std::pair<Collider*, Collider*>> colliderPairs);
	Astronaut* getAstronaut() { return astronaut; }
	AIState getState() { return state; }
};

#endif
