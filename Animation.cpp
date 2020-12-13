#include "Animation.h"

Animation::Animation(std::vector<Geometry*> geometry, float secondsPerFrame)
	: geometry(geometry)
	, secondsPerFrame(secondsPerFrame)
	, secondsElapsed(0)
	, active(false)
{
}

Animation::~Animation()
{
}

void Animation::draw(GLuint shader, const glm::mat4& C)
{
	if (active) {
		Geometry* geometryToUse = geometry[int(secondsElapsed / secondsPerFrame)];
		geometryToUse->draw(shader, C);
	}
}

void Animation::update(const glm::mat4& C)
{
	secondsElapsed += Time::deltaTime;
	if (secondsElapsed / secondsPerFrame >= geometry.size()) {
		secondsElapsed = 0;
	}
}
