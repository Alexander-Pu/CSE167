#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "Transform.h"

class Controller
{
private:
	Transform* transform;
	GLfloat moveSpeed;
	GLfloat rotationSpeed;

public:
	Controller(Transform* transform);
	~Controller();

	void moveTransform(glm::vec3 direction, bool rotate, float deltaTime);
	Transform* getTransform() { return transform; }
};

#endif
