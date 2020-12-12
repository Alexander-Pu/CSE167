#ifndef _COLLISION_CHECKER_H_
#define _COLLISION_CHECKER_H_

#include "Collider.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"

class CollisionChecker
{
public:
	CollisionChecker();
	~CollisionChecker();

	bool collides(Collider* colliderOne, Collider* colliderTwo);
};

#endif
