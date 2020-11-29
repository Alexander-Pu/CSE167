#ifndef _MATERIALS_H_
#define _MATERIALS_H_

#include "Object.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "glm/gtx/string_cast.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <vector>

class Materials
{
private:
	glm::vec3 ambient, diffuse, specular;
	float shininess;

public:
	Materials(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
	~Materials();

	void sendMatToShader(GLuint shaderID);
};

#endif
