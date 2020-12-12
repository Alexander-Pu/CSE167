#ifndef _TRACK_BALL_H_
#define _TRACK_BALL_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/transform.hpp>
#include "glm/gtx/string_cast.hpp"
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <vector>

struct TrackBallReturn {
	float angle;
	glm::vec3 rotationAxis;

	TrackBallReturn(float angleVal, glm::vec3 rotationAxisVal) {
		angle = angleVal;
		rotationAxis = rotationAxisVal;
	}

	~TrackBallReturn() {
	}
};


class TrackBall
{
private:
	glm::vec3 lastPoint;
	glm::vec3 currentPoint;
public:
	TrackBall();
	~TrackBall();

	std::shared_ptr<TrackBallReturn> handleMove(double xpos, double width, double ypos, double height);
};

#endif
