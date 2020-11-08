#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "main.h"
#include "shader.h"
#include "Object.h"
#include "TriangleFacedModelLoader.h"
#include "Cube.h"
#include "TriangleFacedModel.h"
#include "PointLight.h"

class Window
{
private:
	// Window state values
	static bool mouseRotation;
	static glm::vec3 lastPoint;

	static glm::vec3 trackBallMapping(double mouseXPos, double mouseYPos);
public:

	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;

	// TriangleFacedModelLoader
	static TriangleFacedModelLoader* triangleFacedModelLoader;

	// Objects to Render
	static GLfloat pointSize;
	static TriangleFacedModel* bunnyTriangleFacedModel;
	static TriangleFacedModel* sandalTriangleFacedModel;
	static TriangleFacedModel* bearTriangleFacedModel;

	// Materials
	static Materials* shinyMat;
	static Materials* diffuseMat;
	static Materials* shinyAndDiffuseMat;

	// Light
	static glm::vec3 pointLightLocation;
	static PointLight* pointLight;
	static TriangleFacedModel* pointLightModel;
	static Materials* lightMat;

	// Camera Matrices
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eyePos, lookAtPoint, upVector;

	// Shader Program ID
	static GLuint normalShaderProgram;
	static GLuint realisticShaderProgram;

	// Constructors and Destructors
	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();

	// Window functions
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);

	// Draw and Update functions
	static void idleCallback();
	static void displayCallback(GLFWwindow*);

	// Callbacks
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
};

#endif
