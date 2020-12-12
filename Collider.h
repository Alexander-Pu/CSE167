#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include "Node.h"
#include "Transform.h"

enum class ColliderType {
	SPHERE,
	PLANE
};

class Collider : public Node
{
protected:
	Transform* transform;
public:
	virtual ColliderType getType() = 0;
	virtual bool isKinematic() = 0;
	Transform* getTransform() { return transform; }
};
#endif