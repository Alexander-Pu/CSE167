#ifndef _CUBE_MAP_TEXTURE_H_
#define _CUBE_MAP_TEXTURE_H_

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

using namespace std;

class CubeMapTexture
{
private:
	GLuint TEXTURE;

public:
	CubeMapTexture(vector<std::string> textureFaceFiles);
	~CubeMapTexture();

	void sendTextureToShader(GLuint shader);
};

#endif
