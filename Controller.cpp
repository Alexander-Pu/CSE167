#include "Controller.h"

Controller::Controller(Transform* transform)
	: transform(transform)
	, moveSpeed(1000)
	, rotationSpeed(8 * glm::pi<float>())
{
}

Controller::~Controller()
{
}

void Controller::moveTransform(glm::vec3 direction, bool rotate) {
	if (glm::length(direction) == 0) {
		return;
	}

	transform->applyTranslation(direction * moveSpeed * Time::deltaTime);

	if (rotate) {
		glm::vec3 forward = transform->getForward();
		if (glm::length(forward + direction) == 0) {
			direction = normalize(direction + glm::vec3(.001, 0, .001));
		}

		GLfloat angle = glm::acos(glm::dot(forward, direction));
		if (std::isnan(angle) || angle == 0) {
			angle = .01;
		}

		glm::vec3 rotationAxis = glm::cross(direction, forward);

		// Ensure valid angle and rotation axis
		if (glm::length(rotationAxis) == 0) {
			rotationAxis = glm::vec3(0, 1, 0);
		}

		glm::mat4 newRotation = glm::rotate(-angle * rotationSpeed * Time::deltaTime, rotationAxis);
		transform->applyRotation(newRotation);
	}
}