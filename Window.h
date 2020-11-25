#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "main.h"
#include "shader.h"
#include "Transform.h"
#include "Geometry.h"
#include "TriangleGeometry.h"
#include "DiscoBall.h"
#include "Skybox.h"
#include "CubeMapTexture.h"
#include "Camera.h"

class Window
{
public:
	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;
	
	// Movement constants
	static const float MOVE_SPEED;
	static const float ROTATION_SPEED;
	static const glm::vec3 FORWARD;
	static const glm::vec3 RIGHT;
	static const glm::vec3 UP;

	static float time;
	static float deltaTime;
	static float rotateLeft;
	static float moveForward;
	static float strafeRight;
	static float moveUp;

	// Shader Program ID
	static GLuint normalShaderProgram;
	static GLuint skyboxShaderProgram;
	static GLuint environmentMapShaderProgram;
	static std::vector<GLuint> shaders;

	// Textures
	static CubeMapTexture* skyboxTexture;

	// Models to Render
	static Geometry* bunnyModel;
	static Geometry* sandalModel;
	static Geometry* bearModel;

	// Skybox
	static Transform* skyboxWorld;
	static Transform* skyboxTransform;
	static Geometry* skybox;

	// World
	static Transform* world;

	// Environment Mapped World
	static Transform* environmentMappedWorld;

	// Camera Matrices
	static glm::mat4 projection;
	static Transform* cameraTransform;
	static Camera* mainCamera;

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
	static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);

private:
	static void updateProjection(const glm::mat4& projection);
};

#endif
