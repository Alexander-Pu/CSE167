#ifndef _PARTICLE_SYSTEM_H_
#define _PARTICLE_SYSTEM_H_

#include "Particle.h"
#include "Time.h"

#include <iostream>

class ParticleSystem
{
private:
	int maxNumParticles;
	std::vector<Particle*> particles;
	int particleIndex;
	GLuint VAO;
	GLuint VBO[2];

public:
	ParticleSystem(int numParticles);
	~ParticleSystem();

	void addParticle(glm::vec3 origin, glm::vec3 velocity, velocity_alterer velocityAlterer, float lifeSpan, glm::vec3 initColor, glm::vec3 endColor);
	void update();
	void draw(GLuint shader);
};

#endif
