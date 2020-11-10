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
	SpotLight(glm::vec3 pos, glm::vec3 color, glm::vec3 direction, glm::vec3 atten, float cutoff, float exponent);
	~SpotLight();

	void sendLightToShader(const int shaderID);
	void clear(const int shaderID);

	void setPos(glm::vec3 pos);
	void setDirection(glm::vec3 direction);
};

#endif
