#ifndef _MATERIALS_H_
#define _MATERIALS_H_

#include "Object.h"

class Materials
{
private:
	glm::vec3 ambient, diffuse, specular;
	float shininess;

public:
	Materials(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
	~Materials();

	void sendMatToShader(const int shaderID);
};

#endif
