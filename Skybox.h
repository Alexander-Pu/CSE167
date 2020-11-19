#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include "Object.h"
#include "stb_image.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Skybox : public Object
{
private:
	glm::mat4 scale;
	glm::mat4 rotation;
	glm::vec3 location;

	GLuint VAO;
	GLuint VBO, EBO;
	GLuint TEXTURE;

public:
	Skybox(vector<std::string> textureFaceFiles, GLfloat scale);
	~Skybox();
	
	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void update();
};

#endif
