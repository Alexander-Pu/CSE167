#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "Node.h"
#include "Geometry.h"
#include "Time.h"

#include <vector>

class Animation : public Node
{
private:
	std::vector<Geometry*> geometry;
	float secondsPerFrame;
	float secondsElapsed;
	bool active;
public:
	Animation(std::vector<Geometry*> geometry, float secondsPerFrame);
	~Animation();

	void draw(GLuint shader, const glm::mat4& C);
	void update(const glm::mat4& C);
	void setActive(bool isActive) { active = isActive; }
};

#endif
