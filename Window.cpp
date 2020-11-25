#include "Window.h"

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "CSE 167";

const float Window::MOVE_SPEED = 15;
const float Window::ROTATION_SPEED = glm::half_pi<float>();
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
GLuint Window::normalShaderProgram;
GLuint Window::skyboxShaderProgram;
GLuint Window::environmentMapShaderProgram;
std::vector<GLuint> Window::shaders;

// Textures 
CubeMapTexture* Window::skyboxTexture;

// Models to Render
Geometry* Window::bunnyModel;
Geometry* Window::sandalModel;
Geometry* Window::bearModel;

// Skybox 
Transform* Window::skyboxWorld;
Transform* Window::skyboxTransform;
Geometry* Window::skybox;

// World
Transform* Window::world;

// Environment Mapped World
Transform* Window::environmentMappedWorld;

// Camera Matrices 
glm::mat4 Window::projection; 
Transform* Window::cameraTransform;
Camera* Window::mainCamera;

bool Window::initializeProgram() {

	// Create a shader program with a vertex shader and a fragment shader.
	normalShaderProgram = LoadShaders("shaders/normal.vert", "shaders/normal.frag");
	skyboxShaderProgram = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");
	environmentMapShaderProgram = LoadShaders("shaders/environment_map.vert", "shaders/environment_map.frag");
	shaders.push_back(normalShaderProgram);
	shaders.push_back(skyboxShaderProgram);
	shaders.push_back(environmentMapShaderProgram);
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

	// Call update after the shaders are loaded
	updateProjection(glm::perspective(glm::radians(60.0), double(width) / (double)height, 1.0, 1000.0));

	// Enable face culling to only show "front" of faces.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	return true;
}

bool Window::initializeObjects()
{
	// Create skybox texture
	std::vector<std::string> skyboxTextures
	{
		"CubeMapImages/Day_Right.jpg",
		"CubeMapImages/Day_Left.jpg",
		"CubeMapImages/Day_Top.jpg",
		"CubeMapImages/Day_Bottom.jpg",
		"CubeMapImages/Day_Front.jpg",
		"CubeMapImages/Day_Back.jpg"
	};
	skyboxTexture = new CubeMapTexture(skyboxTextures);
	skyboxTexture->sendTextureToShader(Window::skyboxShaderProgram);
	skyboxTexture->sendTextureToShader(Window::environmentMapShaderProgram);

	// Create 3d models of objects.
	bunnyModel = new TriangleGeometry("Objects/bunny.objmodel");
	sandalModel = new TriangleGeometry("Objects/sandal.objmodel");
	bearModel = new TriangleGeometry("Objects/bear.objmodel");

	// Initialize skybox
	skyboxWorld = new Transform(glm::mat4(1), glm::mat4(1), glm::vec3(0));
	skyboxTransform = new Transform(glm::scale(glm::vec3(500)), glm::mat4(1), glm::vec3(0, 0, 0));
	skyboxWorld->addChild(skyboxTransform);
	skybox = new Skybox();
	skyboxTransform->addChild(skybox);

	//Place objects in world
	world = new Transform(glm::mat4(1), glm::mat4(1), glm::vec3(0));

	Transform* bunnyTransform = new Transform(glm::scale(glm::vec3(6)), glm::mat4(1), glm::vec3(0, 0, 0));
	bunnyTransform->addChild(bunnyModel);
	world->addChild(bunnyTransform);

	Transform* sandalTransform = new Transform(glm::scale(glm::vec3(1)), glm::mat4(1), glm::vec3(0, 0, 5));
	sandalTransform->addChild(sandalModel);
	bunnyTransform->addChild(sandalTransform);

	Transform* bearTransform = new Transform(glm::scale(glm::vec3(1)), glm::mat4(1), glm::vec3(0, 5, 0));
	bearTransform->addChild(bearModel);
	sandalTransform->addChild(bearTransform);

	// Create Environment Mapped world
	environmentMappedWorld = new Transform(glm::mat4(1), glm::mat4(1), glm::vec3(0));

	Transform* discoBallTransform = new Transform(glm::scale(glm::vec3(5)), glm::rotate(glm::half_pi<float>(), glm::vec3(1, 0, 0)), glm::vec3(0, 5, 0));
	Geometry* discoBall = new DiscoBall(100, 200);
	discoBallTransform->addChild(discoBall);
	environmentMappedWorld->addChild(discoBallTransform);

	// Initialize camera
	cameraTransform = new Transform(glm::mat4(1), glm::mat4(1), glm::vec3(0, 0, 0));
	world->addChild(cameraTransform);
	environmentMappedWorld->addChild(cameraTransform);
	skyboxWorld->addChild(cameraTransform);
	mainCamera = new Camera();
	cameraTransform->addChild(mainCamera);

	return true;
}

void Window::cleanUp()
{
	// Delete textures
	delete skyboxTexture;

	// Deallocate the objects.
	delete bunnyModel;
	delete sandalModel;
	delete bearModel;

	// Deallocate skybox.
	delete skyboxTransform;
	delete skybox;

	// Deallocate worlds.
	delete world;
	delete environmentMappedWorld;

	// Delete the shader program.
	glDeleteProgram(normalShaderProgram);
	glDeleteProgram(skyboxShaderProgram);
	glDeleteProgram(environmentMapShaderProgram);
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

	// Set the projection matrix.
	updateProjection(glm::perspective(glm::radians(60.0), double(width) / (double)height, 1.0, 1000.0));
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

	// Render the world
	world->draw(normalShaderProgram, glm::mat4(1));

	// Render environment mapped world
	skyboxTexture->sendTextureToShader(Window::environmentMapShaderProgram);
	environmentMappedWorld->draw(environmentMapShaderProgram, glm::mat4(1));

	// Render skybox last as an optimization
	skyboxTexture->sendTextureToShader(Window::skyboxShaderProgram);
	skyboxWorld->draw(skyboxShaderProgram, glm::mat4(1));

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

	// Update camera
	glm::mat4 rotationMatrix = glm::rotate(Window::deltaTime * Window::rotateLeft * Window::ROTATION_SPEED, Window::UP);

	glm::mat4 translateMatrix = glm::mat4(1);
	if (moveForward != 0 || strafeRight != 0 || moveUp != 0) {
		glm::vec3 moveDir = glm::normalize(moveForward * Window::FORWARD + strafeRight * Window::RIGHT + moveUp * Window::UP);
		translateMatrix = glm::translate(glm::mat4(1), Window::deltaTime * moveDir);
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

void Window::mouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
	// No longer track mouse movement outside of window
	if (std::isnan(xpos) || std::isnan(ypos)) {
		return;
	}
}

void Window::updateProjection(const glm::mat4& newProjection) {
	Window::projection = newProjection;

	for (GLuint shader : shaders) {
		glUseProgram(shader);
		glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(Window::projection));
		glUseProgram(0);
	}
}
