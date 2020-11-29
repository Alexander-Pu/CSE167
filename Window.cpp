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
GLuint Window::environmentMapShaderProgram;
GLuint Window::skyboxShaderProgram;
std::vector<GLuint> Window::shaders;

// Textures 
CubeMapTexture* Window::skyboxTexture;
Texture* Window::grass;

// Materials
Materials* Window::shinyAndDiffuseMat;

// Models to Render
Geometry* Window::plane;
Geometry* Window::cube;
Geometry* Window::cone;
Geometry* Window::cylinder;

// Skybox 
Transform* Window::skyboxWorld;
Transform* Window::skyboxTransform;
Geometry* Window::skybox;

// World
Transform* Window::world;
Transform* Window::basePlate;
Transform* Window::spinner;
Transform* Window::chair;

// Environment Mapped World
Transform* Window::environmentMappedWorld;

// Camera Matrices 
glm::mat4 Window::projection; 
Transform* Window::cameraTransform;
Camera* Window::mainCamera;

// Lights
PointLight* Window::pointLight;
SpotLight* Window::spotLight;

bool Window::initializeProgram() {

	// Create a shader program with a vertex shader and a fragment shader.
	phongShader = LoadShaders("shaders/realistic.vert", "shaders/realistic.frag");
	std::cout << "Phong shader id: " << phongShader;
	environmentMapShaderProgram = LoadShaders("shaders/environment_map.vert", "shaders/environment_map.frag");
	skyboxShaderProgram = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");
	shaders.push_back(phongShader);
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
	// Create textures
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

	grass = new Texture("Textures/grass.jpg");
	//grass->sendTextureToShader(phongShader);

	// Materials
	shinyAndDiffuseMat = new Materials(glm::vec3(1, 0.5, 0.5), glm::vec3(0.9), glm::vec3(0.9), 64.0);
	shinyAndDiffuseMat->sendMatToShader(phongShader);

	// Create 3d models of objects.
	plane = new TriangleGeometry("Objects/plane.objmodel");
	cube = new TriangleGeometry("Objects/cube.objmodel");
	cone = new TriangleGeometry("Objects/cone.objmodel");
	cylinder = new TriangleGeometry("Objects/cylinder.objmodel");

	// Initialize skybox
	skyboxWorld = new Transform(glm::mat4(1), glm::mat4(1), glm::vec3(0));
	skyboxTransform = new Transform(glm::scale(glm::vec3(50)), glm::mat4(1), glm::vec3(0, 0, 0));
	skyboxWorld->addChild(skyboxTransform);
	skybox = new Skybox();
	skyboxTransform->addChild(skybox);

	//Place objects in world
	world = new Transform(glm::mat4(1), glm::mat4(1), glm::vec3(0));

	Transform* groundTransform = new Transform(glm::scale(glm::vec3(1)), glm::mat4(1), glm::vec3(0, 0, 0));
	groundTransform->addChild(plane);
	world->addChild(groundTransform);

	// Ride
	// Base Plate
	basePlate = new Transform(glm::scale(glm::vec3(1)), glm::rotate(glm::pi<float>(), Window::UP), glm::vec3(0, 0, 0));
	world->addChild(basePlate);
	Transform* basePlateCylinder = new Transform(glm::scale(glm::vec3(5, 1, 5)), glm::mat4(1), glm::vec3(0, 0, 0));
	basePlateCylinder->addChild(cylinder);
	basePlate->addChild(basePlateCylinder);
	Transform* basePlateCube = new Transform(glm::scale(glm::vec3(10, 1, 10)), glm::mat4(1), glm::vec3(0, 2, 0));
	basePlateCube->addChild(cube);
	basePlate->addChild(basePlateCube);
	Transform* basePlatePillar = new Transform(glm::scale(glm::vec3(1, 20, 1)), glm::mat4(1), glm::vec3(0, 10, -5));
	basePlatePillar->addChild(cube);
	basePlate->addChild(basePlatePillar);
	Transform* basePlateArm = new Transform(glm::scale(glm::vec3(1, 1, 5)), glm::mat4(1), glm::vec3(0, 20, -3));
	basePlateArm->addChild(cube);
	basePlate->addChild(basePlateArm);

	// Spinner
	spinner = new Transform(glm::scale(glm::vec3(1)), glm::rotate(glm::half_pi<float>(), glm::vec3(1, 0, 0)), glm::vec3(0, 20, 0));
	basePlate->addChild(spinner);
	Transform* spinnerTube = new Transform(glm::scale(glm::vec3(15, 1, 1)), glm::mat4(1), glm::vec3(0, 0, 0));
	spinnerTube->addChild(cube);
	spinner->addChild(spinnerTube);
	Transform* pivot = new Transform(glm::scale(glm::vec3(.15)), glm::mat4(1), glm::vec3(0, .5, 0));
	pivot->addChild(cone);
	spinner->addChild(pivot);
	Transform* counterWeight = new Transform(glm::scale(glm::vec3(2)), glm::mat4(1), glm::vec3(-8, 0, 0));
	counterWeight->addChild(cube);
	spinner->addChild(counterWeight);

	// Chair
	chair = new Transform(glm::scale(glm::vec3(1)), glm::rotate(-glm::half_pi<float>(), glm::vec3(1, 0, 0)), glm::vec3(8, 1.5, 0));
	spinner->addChild(chair);
	Transform* chairSeat = new Transform(glm::scale(glm::vec3(8, 3, 2)), glm::mat4(1), glm::vec3(0, 0, 0));
	chairSeat->addChild(cube);
	chair->addChild(chairSeat);

	// Create Environment Mapped world
	environmentMappedWorld = new Transform(glm::mat4(1), glm::mat4(1), glm::vec3(0));

	Transform* discoBallTransform = new Transform(glm::scale(glm::vec3(5)), glm::rotate(glm::half_pi<float>(), glm::vec3(1, 0, 0)), glm::vec3(0, 5, 0));
	Geometry* discoBall = new DiscoBall(100, 200);
	discoBallTransform->addChild(discoBall);
	environmentMappedWorld->addChild(discoBallTransform);

	// Initialize camera
	cameraTransform = new Transform(glm::mat4(1), glm::mat4(1), glm::vec3(0, 0, 20));
	chair->addChild(cameraTransform);
	environmentMappedWorld->addChild(cameraTransform);
	skyboxWorld->addChild(cameraTransform);
	mainCamera = new Camera();
	cameraTransform->addChild(mainCamera);

	// Initialize lights
	Transform* pointLightTransform = new Transform(glm::scale(glm::vec3(1)), glm::mat4(1), glm::vec3(3.0, 10, 6.0));
	pointLightTransform->addChild(cube);
	world->addChild(pointLightTransform);

	Transform* spotLightTransform = new Transform(glm::scale(glm::vec3(1)), glm::mat4(1), glm::vec3(-3.0, 10, 6.0));
	spotLightTransform->addChild(cube);
	world->addChild(spotLightTransform);

	pointLight = new PointLight(glm::vec3(3.0, 10.0, 6.0), glm::vec3(0.6, 0.6, 0.8), glm::vec3(1, 0, 0.0));
	pointLight->sendLightToShader(phongShader);
	spotLight = new SpotLight(glm::vec3(-3.0, 10.0, 6.0), glm::vec3(0.8, 0.6, 0.6), glm::vec3(1, 0, 0.0), glm::normalize(glm::vec3(0,0,0) - glm::vec3(-3.0, 3.0, 6.0)), 0.5, 20);
	spotLight->sendLightToShader(phongShader);

	return true;
}

void Window::cleanUp()
{
	// Delete textures
	delete skyboxTexture;
	delete grass;

	// Deallocate the objects.
	delete cube;
	delete cone;
	delete cylinder;

	// Deallocate skybox.
	delete skyboxTransform;
	delete skybox;

	// Deallocate worlds.
	delete world;
	delete environmentMappedWorld;

	// Delete the shader program.
	glDeleteProgram(phongShader);
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
	world->draw(phongShader, glm::mat4(1));

	// Render environment mapped world
	skyboxTexture->sendTextureToShader(Window::environmentMapShaderProgram);
	//environmentMappedWorld->draw(environmentMapShaderProgram, glm::mat4(1));

	// Render skybox last as an optimization
	skyboxTexture->sendTextureToShader(Window::skyboxShaderProgram);
	//skyboxWorld->draw(skyboxShaderProgram, glm::mat4(1));

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

	// Rotate baseplate
	rotationMatrix = glm::rotate(Window::deltaTime * .5f * Window::ROTATION_SPEED, Window::UP);
	basePlate->applyTransformation(rotationMatrix);
	spinner->applyTransformation(rotationMatrix);
	chair->applyTransformation(glm::rotate(Window::deltaTime * .5f * Window::ROTATION_SPEED, glm::vec3(0, 0, -1)));

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
