#include "TrackBall.h"

TrackBall::TrackBall()
{
	lastPoint = glm::vec3(0);
	currentPoint = glm::vec3(0);
}

TrackBall::~TrackBall()
{
}

std::shared_ptr<TrackBallReturn> TrackBall::handleMove(double xpos, double width, double ypos, double height) {
	// Sanitize inputs
	if (std::isnan(xpos) || std::isnan(width) || std::isnan(ypos) || std::isnan(height) || width == 0 || height == 0) {
		return std::shared_ptr<TrackBallReturn>(new TrackBallReturn(0, glm::vec3(0, 1, 0)));
	}

	glm::vec3 mouseProjectedVector;
	mouseProjectedVector.x = (2.0f * xpos - width) / width;
	mouseProjectedVector.y = (height - 2.0f * ypos) / height;
	mouseProjectedVector.z = 0.0f;
	GLfloat distanceFromOrigin = glm::length(mouseProjectedVector);
	distanceFromOrigin = glm::min(distanceFromOrigin, 1.0f);
	mouseProjectedVector.z = glm::sqrt(1.001f - distanceFromOrigin * distanceFromOrigin);

	currentPoint = glm::normalize(mouseProjectedVector);

	glm::vec3 rotationAxis = glm::cross(lastPoint, currentPoint);
	GLfloat angle = glm::acos(glm::dot(lastPoint, currentPoint));

	// Ensure valid rotation values
	if (std::isnan(angle) || glm::length(rotationAxis) == 0) {
		rotationAxis = glm::vec3(0, 1, 0);
		angle = 0;
	}

	lastPoint = currentPoint;

	return std::shared_ptr<TrackBallReturn>(new TrackBallReturn(angle, rotationAxis));
}
