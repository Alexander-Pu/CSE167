#ifndef _NODE_H_
#define _NODE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/transform.hpp>
#include "glm/gtx/string_cast.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <vector>

class Node
{
public:
	virtual void draw(GLuint shader, const glm::mat4& C) = 0;
	virtual void update(const glm::mat4& C) = 0;
};

#endif

