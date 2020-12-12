#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "main.h"
#include "shader.h"
#include "Transform.h"
#include "Geometry.h"
#include "TriangleGeometry.h"
#include "QuadGeometry.h"
#include "Skybox.h"
#include "Texture.h"
#include "Materials.h"
#include "Camera.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Controller.h"
#include "Collider.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"
#include "CollisionChecker.h"
#include "CollisionPusher.h"
#include "TrackBall.h"

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
	static float moveForward;
	static float moveBackward;
	static float strafeLeft;
	static float strafeRight;

	static bool mouseRotation;
	static double mouseX;

	static TrackBall* trackBall;

	static Controller* playerController;
	static Collider* redCollider;
	static Collider* pinkCollider;
	static Collider* whiteCollider;
	static std::vector<Collider*> obstacleColliders;
	static CollisionChecker* collisionChecker;
	static CollisionPusher* collisionPusher;

	// Shader Program ID
	static GLuint phongShader;
	static GLuint toonShader;
	static std::vector<GLuint> shaders;

	// Models to Render
	static Geometry* lobby;
	static Geometry* astronaut_idle;
	static Geometry* astronaut_movingOne;
	static Geometry* astronaut_movingTwo;

	// Materials
	static Materials* lobbyMat;

	// Textures
	static Texture* lobbyTexture;
	static Texture* solidTexture;

	// World
	static Transform* world;
	static Transform* toonWorld;

	// Camera Matrices
	static glm::mat4 projection;
	static Transform* cameraLobbyTransform;
	static Transform* cameraTransform;
	static Camera* mainCamera;
	static bool firstPerson;
	static Transform* playerCameraTransform;
	static Camera* playerCamera;

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
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);

private:
	static glm::vec3 hexToRGB(int hex);
};

#endif
