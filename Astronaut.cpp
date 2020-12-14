#include "Astronaut.h"

Astronaut::Astronaut(glm::vec3 spawnLocation, std::vector<Geometry*>& idleFrames, std::vector<Geometry*>& walkingFrames, glm::vec3 color)
	: color(color)
{
	transform = new Transform(glm::scale(glm::vec3(1)), glm::mat4(1), spawnLocation);
	idle = new Animation(idleFrames, 1);
	walking = new Animation(walkingFrames, .2);
	transform->addChild(idle);
	transform->addChild(walking);
	materials = new Materials(color, glm::vec3(1), glm::vec3(1), 8);
	transform->setMaterials(materials);
	collider = new SphereCollider(transform, glm::vec3(0, 1, 0), 1.1, false);
	transform->addChild(collider);
	controller = new Controller(transform);
}

Astronaut::~Astronaut()
{
	delete transform;
	delete idle;
	delete walking;
	delete materials;
	delete collider;
	delete controller;
}

void Astronaut::move(glm::vec3 moveDir, bool rotate) {
	idle->setActive(false);
	walking->setActive(true);
	controller->moveTransform(moveDir, rotate);
}

void Astronaut::stop() {
	idle->setActive(true);
	walking->setActive(false);
}

void Astronaut::hide() {
	idle->setActive(false);
	walking->setActive(false);
}