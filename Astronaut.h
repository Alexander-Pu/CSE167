#ifndef _ASTRONAUT_H_
#define _ASTRONAUT_H_

#include "Controller.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "Geometry.h"
#include "Animation.h"
#include "Materials.h"

class Astronaut
{
private:
	Transform* transform;
	Animation* idle;
	Animation* walking;
	Materials* materials;
	Collider* collider;
	Controller* controller;
public:
	Astronaut(glm::vec3 spawnLocation, std::vector<Geometry*>& idleFrames, std::vector<Geometry*>& walkingFrames, glm::vec3 color);
	~Astronaut();

	Transform* getTransform() { return transform; }
	Collider* getCollider() { return collider; }
	void move(glm::vec3 moveDir, bool rotate);
	void stop();
};

#endif
