#ifndef _COLLISION_PUSHER_H_
#define _COLLISION_PUSHER_H_

#include "Collider.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"
#include "Transform.h"

class CollisionPusher
{
public:
	CollisionPusher();
	~CollisionPusher();

	void resolveCollision(Collider* colliderOne, Collider* colliderTwo);
};

#endif
