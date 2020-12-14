#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/transform.hpp>
#include "glm/gtx/string_cast.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#include <functional>

#include "Time.h"

typedef glm::vec3 (*velocity_alterer)(glm::vec3); // type for conciseness

class Particle
{
private:
	glm::vec3 origin;
	glm::vec3 velocity;
	std::function<glm::vec3(glm::vec3)> velocityAlterer;
	float lifeSpan;
	float timeElapsed;
	glm::vec3 initColor;
	glm::vec3 endColor;
	glm::vec3 location;
	glm::vec3 color;
public:
	Particle(glm::vec3 origin, glm::vec3 velocity, velocity_alterer velocityAlterer, float lifeSpan, glm::vec3 initColor, glm::vec3 endColor);
	~Particle();

	void update();
	void alter(glm::vec3 origin, glm::vec3 velocity, velocity_alterer velocityAlterer, float lifeSpan, glm::vec3 initColor, glm::vec3 endColor);
	glm::vec3 getLocation() { return location; }
	glm::vec3 getColor() { return color; }
};

#endif
