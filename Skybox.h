#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include "Geometry.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Skybox : public Geometry
{
public:
	Skybox();
	~Skybox();
	
	void draw(GLuint shader, const glm::mat4& C);
	void update();
};

#endif
