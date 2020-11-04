#ifndef _TRIANGLE_FACED_MODEL_H_
#define _TRIANGLE_FACED_MODEL_H_

#include "Object.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;

class TriangleFacedModel : public Object
{
private:
	std::vector<glm::vec3> vertexes;
	std::vector<glm::vec3> vertexNormals;
	std::vector<glm::uvec3> vertexIndexes;
	std::vector<glm::uvec3> vertexNormalIndexes;

	GLuint VAO;
	GLuint VBO, EBO;

public:
	TriangleFacedModel(std::vector<glm::vec3> vertexes, std::vector<glm::vec3> vertexNormals, std::vector<glm::uvec3> vertexIndexes, std::vector<glm::uvec3> vertexNormalIndexes);
	~TriangleFacedModel();
	
	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void update();

	void spin(float deg);
};

#endif