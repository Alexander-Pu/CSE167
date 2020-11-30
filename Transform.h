#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Node.h"

#include <vector>
#include <map>

class Transform : public Node
{
private:
	glm::mat4 transformMatrix;
	std::vector<Node*> children;

public:
	Transform(const glm::mat4 scale, const glm::mat4 rotation, const glm::vec3 location);
	~Transform();

	void draw(GLuint shader, const glm::mat4& C);
	void update();
	void addChild(Node* child);
	void applyTransformation(glm::mat4 transformationMatrix);
	glm::vec3 getRelativeLocation() { return glm::vec3(transformMatrix[3][0], transformMatrix[3][1], transformMatrix[3][2]); }
	glm::mat4 getTransformMatrix() { return transformMatrix; }
};

#endif