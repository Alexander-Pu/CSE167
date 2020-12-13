#ifndef _COLLIDER_TRACKER_H_
#define _COLLIDER_TRACKER_H_

#include "Collider.h"

class ColliderTracker
{
private:
	std::vector<Collider*> nonKinematicColliders;
	std::vector<Collider*> kinematicColliders;
public:
	ColliderTracker();
	~ColliderTracker();

	void addCollider(Collider* collider);
	std::vector<Collider*>& getNonKinematicColliders() { return nonKinematicColliders; }
	std::vector<Collider*>& getKinematicColliders() { return kinematicColliders; }
};

#endif
