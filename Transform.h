#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Node.h"
#include "Materials.h"

#include <vector>
#include <map>

class Transform : public Node
{
private:
	glm::mat4 scale;
	glm::mat4 rotation;
	glm::vec3 location;
	glm::vec3 worldLocation;
	std::vector<Node*> children;
	Materials* materials;

public:
	Transform(const glm::mat4 scale, const glm::mat4 rotation, const glm::vec3 location);
	~Transform();

	void draw(GLuint shader, const glm::mat4& C);
	void update(const glm::mat4& C);
	void addChild(Node* child);
	glm::mat4 getScale() { return scale; }
	void setScale(glm::mat4 newScale) { scale = newScale; }
	glm::mat4 getRotation() { return rotation; }
	void setRotation(glm::mat4 newRotation) { rotation = newRotation; }
	void applyRotation(glm::mat4 newRotation) { rotation = newRotation * rotation; }
	void applyLocalRotation(glm::mat4 newRotation) { rotation = rotation * newRotation; }
	void setLocation(glm::vec3 newLocation) { location = newLocation; }
	glm::vec3 getLocation() { return worldLocation; }
	void applyTranslation(glm::vec3 direction) { location += direction; }
	glm::vec3 getForward() { return glm::vec3(rotation * glm::vec4(0, 0, 1, 0)); }
	glm::vec3 getRight() { return glm::vec3(rotation * glm::vec4(-1, 0, 0, 0)); }

	void setMaterials(Materials* newMaterials) { materials = newMaterials; }
};

#endif