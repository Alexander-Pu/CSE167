#ifndef _POINT_LIGHT_H_
#define _POINT_LIGHT_H_

#include "Object.h"

#include <vector>
#include <string>
#include <iostream>

class PointLight
{
private:
	glm::vec3 pos, color, atten;

public:
	PointLight(glm::vec3 pos, glm::vec3 color, glm::vec3 atten);
	~PointLight();

	void sendLightToShader(const int shaderID);

	void setPos(glm::vec3 newPos);
	void setColor(glm::vec3 newColor);
};

#endif
