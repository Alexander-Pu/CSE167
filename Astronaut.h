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
	glm::vec3 color;
	Materials* materials;
	Collider* collider;
	Controller* controller;
public:
	Astronaut(glm::vec3 spawnLocation, std::vector<Geometry*>& idleFrames, std::vector<Geometry*>& walkingFrames, glm::vec3 color);
	~Astronaut();

	Transform* getTransform() { return transform; }
	Collider* getCollider() { return collider; }
	glm::vec3 getColor() { return color; }
	void move(glm::vec3 moveDir, bool rotate);
	void stop();
	void hide();
};

#endif
