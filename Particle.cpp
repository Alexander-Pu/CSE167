#include "Particle.h"

Particle::Particle(glm::vec3 origin, glm::vec3 velocity, velocity_alterer velocityAlterer, float lifeSpan, glm::vec3 initColor, glm::vec3 endColor)
	: origin(origin)
	, velocity(velocity)
	, velocityAlterer(velocityAlterer)
	, location(origin)
	, lifeSpan(lifeSpan)
	, timeElapsed(0)
	, initColor(initColor)
	, endColor(endColor)
	, color(initColor)
{
}

Particle::~Particle()
{
}

void Particle::update()
{
	timeElapsed += Time::deltaTime;

	float lerpValue = timeElapsed / lifeSpan;

	if (lerpValue <= 1) {
		location += velocity * Time::deltaTime;
		color = (1 - lerpValue) * initColor + lerpValue * endColor;
		velocity = velocityAlterer(velocity);
	}
	else {
		location = glm::vec3(0, 300, 0);
	}
}

void Particle::alter(glm::vec3 newOrigin, glm::vec3 newVelocity, velocity_alterer newVelocityAlterer, float newLifeSpan, glm::vec3 newInitColor, glm::vec3 newEndColor)
{
	origin = newOrigin;
	velocity = newVelocity;
	velocityAlterer = newVelocityAlterer;
	location = newOrigin;
	lifeSpan = newLifeSpan;
	timeElapsed = 0;
	initColor = newInitColor;
	endColor = newEndColor;
	color = newInitColor;
}
