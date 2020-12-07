#include "Window.h"

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "CSE 167";

const float Window::MOVE_SPEED = 15;
const float Window::ROTATION_SPEED = glm::pi<float>();
const glm::vec3 Window::FORWARD = glm::vec3(0, 0, -1);
const glm::vec3 Window::RIGHT = glm::vec3(1, 0, 0);
const glm::vec3 Window::UP = glm::vec3(0, 1, 0);

float Window::time = 0;
float Window::deltaTime = 0.1;
float Window::rotateLeft = 0;
float Window::moveForward = 0;
float Window::strafeRight = 0;
float Window::moveUp = 0;

// Shader Program ID
GLuint Window::phongShader;
GLuint Window::toonShader;
std::vector<GLuint> Window::shaders;

// Models to Render
Geometry* Window::lobby;
Geometry* Window::astronaut_idle;
Geometry* Window::astronaut_movingOne;
Geometry* Window::astronaut_movingTwo;

// Materials
Materials* Window::lobbyMat;

// Textures 
Texture* Window::lobbyTexture;
Texture* Window::solidTexture;

// World
Transform* Window::world;

// Camera Matrices 
glm::mat4 Window::projection; 
Transform* Window::cameraTransform;
Camera* Window::mainCamera;

// Lights
PointLight* Window::pointLight;
SpotLight* Window::spotLight;

bool Window::initializeProgram() {
	phongShader = LoadShaders("shaders/realistic.vert", "shaders/realistic.frag");
	toonShader = LoadShaders("shaders/toon.vert", "shaders/toon.frag");
	shaders.push_back(phongShader);
	shaders.push_back(toonShader);
	bool shadersLoaded = true;
	for (GLuint shader : shaders) {
		shadersLoaded = shadersLoaded | shader;
	}

	// Check the shader program.
	if (!shadersLoaded)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		while (true) {
		}
		return false;
	}

	// Enable face culling to only show "front" of faces.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	return true;
}

bool Window::initializeObjects()
{
	// Lobby.
	lobby = new TriangleGeometry("Objects/amongus_lobby.obj");

	lobbyMat = new Materials(glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.9), glm::vec3(0), 64.0);
	lobby->setMaterials(lobbyMat);

	lobbyTexture = new Texture("Textures/amongus_lobby.png", GL_REPEAT);
	lobby->setTexture(lobbyTexture);

	// Astronaut
	solidTexture = new Texture("Textures/white.jpg", GL_REPEAT);

	astronaut_idle = new TriangleGeometry("Objects/amongus_astro_still.obj");
	astronaut_idle->setTexture(solidTexture);
	astronaut_movingOne = new TriangleGeometry("Objects/amongus_astro_moving1.obj");
	astronaut_movingOne->setTexture(solidTexture);
	astronaut_movingTwo = new TriangleGeometry("Objects/amongus_astro_moving2.obj");
	astronaut_movingTwo->setTexture(solidTexture);

	//Place objects in world
	world = new Transform(glm::mat4(1), glm::mat4(1), glm::vec3(0));
	world->addChild(lobby);

	Transform* redAstronaut = new Transform(glm::mat4(1), glm::mat4(1), glm::vec3(0, .4, 10));
	redAstronaut->addChild(astronaut_idle);
	Materials* redMat = new Materials(hexToRGB(0xC51211), glm::vec3(0.9), glm::vec3(0), 1);
	redAstronaut->setMaterials(redMat);
	world->addChild(redAstronaut);

	Transform* pinkAstronaut = new Transform(glm::mat4(1), glm::mat4(1), glm::vec3(3, .4, 10));
	pinkAstronaut->addChild(astronaut_movingOne);
	Materials* pinkMat = new Materials(hexToRGB(0xEC54BB), glm::vec3(0.9), glm::vec3(0), 1);
	pinkAstronaut->setMaterials(pinkMat);
	world->addChild(pinkAstronaut);

	Transform* whiteAstronaut = new Transform(glm::mat4(1), glm::mat4(1), glm::vec3(-3, .4, 10));
	whiteAstronaut->addChild(astronaut_movingTwo);
	Materials* whiteMat = new Materials(hexToRGB(0xD6DFF1), glm::vec3(0.9), glm::vec3(0), 1);
	whiteAstronaut->setMaterials(whiteMat);
	world->addChild(whiteAstronaut);
	
	// Initialize camera
	cameraTransform = new Transform(glm::mat4(1), glm::rotate(glm::quarter_pi<float>(), glm::vec3(-1, 0, 0)), glm::vec3(0, 15, 20));
	world->addChild(cameraTransform);
	mainCamera = new Camera(shaders, glm::radians(60.0), double(Window::width) / (double)Window::height, 1, 1000);
	cameraTransform->addChild(mainCamera);

	// Initialize lights
	Transform* pointLightTransform = new Transform(glm::scale(glm::vec3(1)), glm::mat4(1), glm::vec3(3.0, 10, 6.0));
	world->addChild(pointLightTransform);

	pointLight = new PointLight(glm::vec3(3.0, 10.0, 6.0), glm::vec3(0.6, 0.6, 0.8), glm::vec3(1, .005, 0.0));
	world->addChild(pointLight);

	Transform* spotLightTransform = new Transform(glm::scale(glm::vec3(.5)), glm::rotate(glm::quarter_pi<float>()/2, glm::vec3(-1, 0, 0)), glm::vec3(0, 20, 6.0));
	world->addChild(spotLightTransform);

	spotLight = new SpotLight(spotLightTransform->getRelativeLocation(), glm::vec3(0.8, 0.6, 0.6), glm::vec3(1, .005, 0.0), glm::normalize(glm::vec3(0,0,0) - spotLightTransform->getRelativeLocation()), 0.3, 20);
	world->addChild(spotLight);

	return true;
}

void Window::cleanUp()
{
	// Deallocate the objects.
	delete lobby;
	
	// Delete materials
	delete lobbyMat;

	// Delete textures
	delete lobbyTexture;

	// Deallocate worlds.
	delete world;

	// Delete the shader program.
	glDeleteProgram(phongShader);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);
}

void Window::idleCallback()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - time;
	time = currentFrame;
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	world->draw(toonShader, glm::mat4(1));

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	// Update camera
	glm::mat4 rotationMatrix = glm::rotate(Window::deltaTime * Window::rotateLeft * Window::ROTATION_SPEED, Window::UP);

	glm::mat4 translateMatrix = glm::mat4(1);
	if (moveForward != 0 || strafeRight != 0 || moveUp != 0) {
		glm::vec3 moveDir = glm::normalize(moveForward * Window::FORWARD + strafeRight * Window::RIGHT + moveUp * Window::UP);
		translateMatrix = glm::translate(glm::mat4(1), Window::deltaTime * MOVE_SPEED * moveDir);
	}

	cameraTransform->applyTransformation(translateMatrix * rotationMatrix);

	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_A:
			Window::rotateLeft = 1;
			break;
		case GLFW_KEY_D:
			Window::rotateLeft = -1;
			break;
		case GLFW_KEY_W:
			Window::moveForward = 1;
			break;
		case GLFW_KEY_S:
			Window::moveForward = -1;
			break;
		case GLFW_KEY_E:
			Window::strafeRight = 1;
			break;
		case GLFW_KEY_Q:
			Window::strafeRight = -1;
			break;
		case GLFW_KEY_SPACE:
			Window::moveUp = 1;
			break;
		case GLFW_KEY_LEFT_CONTROL:
			Window::moveUp = -1;
			break;
		default:
			break;
		}
	}
	else if (action == GLFW_RELEASE) {
		switch (key)
		{
		case GLFW_KEY_A:
		case GLFW_KEY_D:
			Window::rotateLeft = 0;
			break;
		case GLFW_KEY_W:
		case GLFW_KEY_S:
			Window::moveForward = 0;
			break;
		case GLFW_KEY_E:
		case GLFW_KEY_Q:
			Window::strafeRight = 0;
			break;
		case GLFW_KEY_SPACE:
		case GLFW_KEY_LEFT_CONTROL:
			Window::moveUp = 0;
		default:
			break;
		}
	}
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	GLfloat fov = mainCamera->getFov();
	GLfloat newFov = glm::min(glm::max(glm::radians(5.0), fov - glm::radians(yoffset)), glm::radians(60.0));
	mainCamera->setFov(newFov);
}

void Window::mouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
	// No longer track mouse movement outside of window
	if (std::isnan(xpos) || std::isnan(ypos)) {
		return;
	}
}

glm::vec3 Window::hexToRGB(int hex) {
	glm::vec3 rgb;
	rgb.r = ((hex >> 16) & 0xFF) / 255.0;
	rgb.g = ((hex >> 8) & 0xFF) / 255.0;
	rgb.b = ((hex) & 0xFF) / 255.0;

	return rgb;
}
