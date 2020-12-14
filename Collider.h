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
	bool kinematic;
public:
	virtual ColliderType getType() = 0;
	bool isKinematic() { return kinematic; }
	void setKinematic(bool isKinematic) { kinematic = isKinematic; }
	Transform* getTransform() { return transform; }
};
#endif