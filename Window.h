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
#include "Texture.h"
#include "Materials.h"
#include "Camera.h"
#include "PointLight.h"
#include "SpotLight.h"

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
	static GLuint phongShader;
	static GLuint environmentMapShaderProgram;
	static GLuint skyboxShaderProgram;
	static std::vector<GLuint> shaders;

	// Textures
	static CubeMapTexture* skyboxTexture;
	static Texture* white;
	static Texture* grass;

	// Materials
	static Materials* diffuseMat;
	static Materials* shinyAndDiffuseMat;

	// Models to Render
	static Geometry* plane;
	static Geometry* cube;
	static Geometry* cone;
	static Geometry* cylinder;

	// Skybox
	static Transform* skyboxWorld;
	static Transform* skyboxTransform;
	static Geometry* skybox;

	// Grass
	static Transform* grassWorld;

	// World
	static Transform* world;
	static Transform* basePlate;
	static Transform* spinner;
	static Transform* chair;
	static Transform* chairSeat;

	static float chairRotationAngle;
	static float chairRotationDirection;
	static float chairRotationAngleMin;
	static float chairRotationAngleMax;

	// Animations
	static bool rotateSpinner;
	static float rotateChair;
	static bool rotateChairSeat;

	// Environment Mapped World
	static Transform* environmentMappedWorld;
	static Transform* discoBallTransform;

	// Camera Matrices
	static glm::mat4 projection;
	static Transform* cameraTransform;
	static Camera* mainCamera;

	// Lights
	static PointLight* pointLight;
	static SpotLight* spotLight;

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
