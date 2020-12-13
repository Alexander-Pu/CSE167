#include "ColliderTracker.h"

ColliderTracker::ColliderTracker()
{
}

ColliderTracker::~ColliderTracker()
{
}

void ColliderTracker::addCollider(Collider* collider) {
	if (collider->isKinematic()) {
		kinematicColliders.push_back(collider);
	}
	else {
		nonKinematicColliders.push_back(collider);
	}
}