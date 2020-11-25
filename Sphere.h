#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Object.h"

#include <vector>
#include <iostream>
#include "stb_image.h"

using namespace std;

class Sphere : public Object
{
private:
	glm::mat4 scale;
	glm::mat4 rotation;
	glm::vec3 location;

	int numVertices;

	GLuint VAO;
	GLuint VBO[2], EBO[2];
	GLuint TEXTURE;

public:
	Sphere(vector<std::string> textureFaceFiles, int numFacesVertical, int numFacesHorizontal);
	~Sphere();

	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void update();
	glm::mat4 getRotation() { return rotation; }
	void setRotation(glm::mat4 newRotation) { rotation = newRotation; }
};

#endif