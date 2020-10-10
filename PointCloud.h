#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

#include "Object.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;

class PointCloud : public Object
{
private:
	std::vector<glm::vec3> points;

	GLuint VAO, VBO;
	GLfloat pointSize;

public:
	PointCloud(std::vector<glm::vec3> points, GLfloat pointSize);
	~PointCloud();
	
	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void update();

	void updatePointSize(GLfloat size);
	void spin(float deg);
};

#endif
