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
Texture* Window::white;
Texture* Window::grass;

// Materials
Materials* Window::diffuseMat;
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

// Grass world
Transform* Window::grassWorld;

// World
Transform* Window::world;
Transform* Window::basePlate;
Transform* Window::spinner;
Transform* Window::chair;
Transform* Window::chairSeat;

float Window::chairRotationAngle = 0;
float Window::chairRotationDirection = 1;
float Window::chairRotationAngleMin = -.75*glm::quarter_pi<float>();
float Window::chairRotationAngleMax = .75 * glm::quarter_pi<float>();

bool Window::rotateSpinner = false;
float Window::rotateChair = 0;
bool Window::rotateChairSeat = false;

// Environment Mapped World
Transform* Window::environmentMappedWorld;
Transform* Window::discoBallTransform;

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

	white = new Texture("Textures/white.jpg");
	grass = new Texture("Textures/grass.png");

	// Materials
	diffuseMat = new Materials(glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.9), glm::vec3(0), 64.0);
	shinyAndDiffuseMat = new Materials(glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.9), glm::vec3(0.9), 64.0);

	// Create 3d models of objects.
	plane = new TriangleGeometry("Objects/plane.objmodel");
	cube = new TriangleGeometry("Objects/cube.objmodel");
	cone = new TriangleGeometry("Objects/cone.objmodel");
	cylinder = new TriangleGeometry("Objects/cylinder.objmodel");

	// Initialize skybox
	skyboxWorld = new Transform(glm::mat4(1), glm::mat4(1), glm::vec3(0));
	skyboxTransform = new Transform(glm::scale(glm::vec3(500)), glm::mat4(1), glm::vec3(0, 0, 0));
	skyboxWorld->addChild(skyboxTransform);
	skybox = new Skybox();
	skyboxTransform->addChild(skybox);

	grassWorld = new Transform(glm::mat4(1), glm::mat4(1), glm::vec3(0));

	Transform* groundTransform = new Transform(glm::scale(glm::vec3(1)), glm::mat4(1), glm::vec3(0, 0, 0));
	groundTransform->addChild(plane);
	grassWorld->addChild(groundTransform);

	//Place objects in world
	world = new Transform(glm::mat4(1), glm::mat4(1), glm::vec3(0));

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
	Transform* pivot = new Transform(glm::scale(glm::vec3(.25)), glm::mat4(1), glm::vec3(0, .5, 0));
	pivot->addChild(cone);
	spinner->addChild(pivot);
	Transform* counterWeight = new Transform(glm::scale(glm::vec3(2)), glm::mat4(1), glm::vec3(-8, 0, 0));
	counterWeight->addChild(cube);
	spinner->addChild(counterWeight);

	// Chair
	chair = new Transform(glm::scale(glm::vec3(1)), glm::rotate(-glm::half_pi<float>(), glm::vec3(1, 0, 0)), glm::vec3(8, 1.5, 0));
	spinner->addChild(chair);
	chairSeat = new Transform(glm::scale(glm::vec3(8, 3, 2)), glm::mat4(1), glm::vec3(0, 0, 0));
	chairSeat->addChild(cube);
	chair->addChild(chairSeat);
	Transform* chairSeatHat = new Transform(glm::scale(glm::vec3(.15)), glm::mat4(1), glm::vec3(0, .5, 0));
	chairSeatHat->addChild(cone);
	chairSeat->addChild(chairSeatHat);

	// Create Environment Mapped world
	environmentMappedWorld = new Transform(glm::mat4(1), glm::mat4(1), glm::vec3(0));

	discoBallTransform = new Transform(glm::scale(glm::vec3(3)), glm::rotate(glm::half_pi<float>(), glm::vec3(1, 0, 0)), glm::vec3(5, 5, 20));
	Geometry* discoBall = new DiscoBall(10, 20);
	discoBallTransform->addChild(discoBall);
	environmentMappedWorld->addChild(discoBallTransform);

	// Initialize camera
	cameraTransform = new Transform(glm::mat4(1), glm::rotate(glm::pi<float>(), Window::UP), glm::vec3(0, 5, -20));
	world->addChild(cameraTransform);
	environmentMappedWorld->addChild(cameraTransform);
	skyboxWorld->addChild(cameraTransform);
	mainCamera = new Camera();
	cameraTransform->addChild(mainCamera);

	// Initialize lights
	Transform* pointLightTransform = new Transform(glm::scale(glm::vec3(1)), glm::mat4(1), glm::vec3(3.0, 10, 6.0));
	pointLightTransform->addChild(cube);
	world->addChild(pointLightTransform);

	pointLight = new PointLight(glm::vec3(3.0, 10.0, 6.0), glm::vec3(0.6, 0.6, 0.8), glm::vec3(1, .005, 0.0));
	pointLight->sendLightToShader(phongShader);

	Transform* spotLightTransform = new Transform(glm::scale(glm::vec3(.5)), glm::rotate(glm::quarter_pi<float>()/2, glm::vec3(-1, 0, 0)), glm::vec3(0, 20, -6.0));
	spotLightTransform->addChild(cone);
	world->addChild(spotLightTransform);

	spotLight = new SpotLight(spotLightTransform->getRelativeLocation(), glm::vec3(0.8, 0.6, 0.6), glm::vec3(1, .005, 0.0), glm::normalize(glm::vec3(0,0,0) - spotLightTransform->getRelativeLocation()), 0.3, 20);
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

	glm::mat4 rotationMatrix = glm::rotate(Window::deltaTime * .5f * Window::ROTATION_SPEED, Window::UP);
	if (Window::rotateSpinner) {
		spinner->applyTransformation(rotationMatrix);
	}
	if (Window::rotateChair != 0) {
		chair->applyTransformation(glm::rotate(Window::deltaTime * .5f * Window::ROTATION_SPEED, glm::vec3(0, 0, Window::rotateChair)));
	}

	// Chair rotation
	if (Window::rotateChairSeat) {
		float rotationDiff = Window::deltaTime * Window::chairRotationDirection;
		chairRotationAngle += rotationDiff;
		if ((chairRotationAngle > chairRotationAngleMax && chairRotationDirection > 0) || ((chairRotationAngle < chairRotationAngleMin && chairRotationDirection < 0))) {
			chairRotationDirection *= -1;
		}
		chairSeat->applyTransformation(glm::rotate(rotationDiff, glm::vec3(1, 0, 0)));
	}

	// Disco rotation
	discoBallTransform->applyTransformation(glm::rotate(Window::deltaTime * .5f * Window::ROTATION_SPEED, glm::vec3(0, 0, 1)));
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Render grass
	diffuseMat->sendMatToShader(phongShader);
	grass->sendTextureToShader(phongShader);
	grassWorld->draw(phongShader, glm::mat4(1));

	// Render the world
	shinyAndDiffuseMat->sendMatToShader(phongShader);
	white->sendTextureToShader(phongShader);
	world->draw(phongShader, glm::mat4(1));

	// Render environment mapped world
	skyboxTexture->sendTextureToShader(environmentMapShaderProgram);
	environmentMappedWorld->draw(environmentMapShaderProgram, glm::mat4(1));

	// Render skybox last as an optimization
	skyboxTexture->sendTextureToShader(skyboxShaderProgram);
	skyboxWorld->draw(skyboxShaderProgram, glm::mat4(1));

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
		case GLFW_KEY_1:
			std::cout << "Toggling spinner" << std::endl;
			Window::rotateSpinner = !Window::rotateSpinner;
			break;
		case GLFW_KEY_2:
			std::cout << "Toggling chair rotation" << std::endl;
			Window::rotateChair++;
			if (Window::rotateChair >= 2) {
				Window::rotateChair = -1;
			}
			break;
		case GLFW_KEY_3:
			std::cout << "Toggling chair seat angle rotation" << std::endl;
			Window::rotateChairSeat = !Window::rotateChairSeat;
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
