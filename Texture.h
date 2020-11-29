#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>

#include "stb_image.h"

class Texture
{
private:
	GLuint TEXTURE;
public:
	Texture(std::string fileName);
	~Texture();

	void sendTextureToShader(GLuint shader);
};

#endif
