#ifndef _SPOT_LIGHT_H_
#define _SPOT_LIGHT_H_

#include "Object.h"

#include <vector>
#include <string>
#include <iostream>

class SpotLight
{
private:
	glm::vec3 pos, color, direction, atten;
	float cutoff, exponent;

public:
	SpotLight(glm::vec3 pos, glm::vec3 color, glm::vec3 atten, glm::vec3 direction, float cutoff, float exponent);
	~SpotLight();

	void sendLightToShader(const int shaderID);

	void setPos(glm::vec3 newPos);
	void setDirection(glm::vec3 newDirection);
	void setColor(glm::vec3 newColor);
};

#endif
