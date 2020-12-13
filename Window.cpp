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
float Time::deltaTime = .01;
float Window::moveForward = 0;
float Window::moveBackward = 0;
float Window::strafeLeft = 0;
float Window::strafeRight = 0;

bool Window::mouseRotation = false;
bool Window::mouseRotationVertical = true;
double Window::mouseX = 0;

TrackBall* Window::trackBall;

Astronaut* Window::playerAstronaut;
ColliderTracker* Window::colliderTracker;
CollisionChecker* Window::collisionChecker;
CollisionPusher* Window::collisionPusher;

AstronautHandler* Window::astroHandler;

// Shader Program ID
GLuint Window::phongShader;
GLuint Window::toonShader;
std::vector<GLuint> Window::shaders;

// Models to Render
Geometry* Window::lobby;
Geometry* Window::astronaut_idle;
Geometry* Window::astronaut_movingOne;
Geometry* Window::astronaut_movingTwo;
Animation* Window::idle;
Animation* Window::walking;

// Materials
Materials* Window::lobbyMat;

// Textures 
Texture* Window::lobbyTexture;
Texture* Window::solidTexture;

// World
Transform* Window::world;
Transform* Window::toonWorld;

// Camera Matrices 
glm::mat4 Window::projection;
Transform* Window::cameraLobbyTransform;
Transform* Window::cameraTransform;
Camera* Window::mainCamera = NULL;
bool Window::firstPerson = false;
Transform* Window::playerCameraTransform;
Camera* Window::playerCamera = NULL;

// Lights
PointLight* Window::pointLight;
SpotLight* Window::spotLight;

bool Window::initializeProgram() {
	phongShader = LoadShaders("shaders/phong_illumination.vert", "shaders/phong_illumination.frag");
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

	std::vector<Geometry*> idleFrames;
	idleFrames.push_back(astronaut_idle);

	std::vector<Geometry*> walkingFrames;
	walkingFrames.push_back(astronaut_idle);
	walkingFrames.push_back(astronaut_movingOne);
	walkingFrames.push_back(astronaut_idle);
	walkingFrames.push_back(astronaut_movingTwo);

	//Place objects in world
	world = new Transform(glm::mat4(1), glm::mat4(1), glm::vec3(0));
	world->addChild(lobby);

	toonWorld = new Transform(glm::mat4(1), glm::mat4(1), glm::vec3(0));

	// Collider tracking
	colliderTracker = new ColliderTracker();
	collisionChecker = new CollisionChecker();
	collisionPusher = new CollisionPusher();

	// Boxes
	Transform* leftBox = new Transform(glm::mat4(1), glm::mat4(1), glm::vec3(-9.6, .4, 11.4));
	SphereCollider* leftBoxCollider = new SphereCollider(leftBox, glm::vec3(0, 1, 0), 3.1, true);
	leftBox->addChild(leftBoxCollider);
	toonWorld->addChild(leftBox);

	Transform* rightBox = new Transform(glm::mat4(1), glm::mat4(1), glm::vec3(10.7, .4, 8));
	SphereCollider* rightBoxCollider = new SphereCollider(rightBox, glm::vec3(0, 1, 0), 3.1, true);
	rightBox->addChild(rightBoxCollider);
	toonWorld->addChild(rightBox);

	colliderTracker->addCollider(leftBoxCollider);
	colliderTracker->addCollider(rightBoxCollider);

	// Walls
	PlaneCollider* leftWallCollider = new PlaneCollider(world, glm::vec3(-16.25, 0, 0), glm::vec3(1, 0, 0));
	world->addChild(leftWallCollider);
	PlaneCollider* rightWallCollider = new PlaneCollider(world, glm::vec3(16.25, 0, 0), glm::vec3(-1, 0, 0));
	world->addChild(rightWallCollider);
	PlaneCollider* topWallCollider = new PlaneCollider(world, glm::vec3(0, 0, 3), glm::vec3(0, 0, 1));
	world->addChild(topWallCollider);
	PlaneCollider* botWallCollider = new PlaneCollider(world, glm::vec3(0, 0, 21), glm::vec3(0, 0, -1));
	world->addChild(botWallCollider);
	PlaneCollider* leftCornerWallCollider = new PlaneCollider(world, glm::vec3(-14, 0, 18), glm::vec3(1, 0, -1));
	world->addChild(leftCornerWallCollider);
	PlaneCollider* rightCornerWallCollider = new PlaneCollider(world, glm::vec3(14, 0, 18), glm::vec3(-1, 0, -1));
	world->addChild(rightCornerWallCollider);

	colliderTracker->addCollider(leftWallCollider);
	colliderTracker->addCollider(rightWallCollider);
	colliderTracker->addCollider(topWallCollider);
	colliderTracker->addCollider(botWallCollider);
	colliderTracker->addCollider(leftCornerWallCollider);
	colliderTracker->addCollider(rightCornerWallCollider);

	// Player
	playerAstronaut = new Astronaut(glm::vec3(0, .5, 8), idleFrames, walkingFrames, hexToRGB(0xC51211));
	toonWorld->addChild(playerAstronaut->getTransform());
	colliderTracker->addCollider(playerAstronaut->getCollider());

	trackBall = new TrackBall();

	// NPCs
	astroHandler = new AstronautHandler(idleFrames, walkingFrames);
	for (auto astronaut : astroHandler->getAstronauts()) {
		toonWorld->addChild(astronaut->getTransform());
		colliderTracker->addCollider(astronaut->getCollider());
	}
	
	// Initialize camera
	cameraLobbyTransform = new Transform(glm::mat4(1), glm::mat4(1), glm::vec3(0, 0, 5));
	cameraTransform = new Transform(glm::mat4(1), glm::rotate((5.0f / 4.0f) * glm::quarter_pi<float>(), glm::vec3(-1.0, 0.0, 0.0)), glm::vec3(0, 40, 30));
	cameraLobbyTransform->addChild(cameraTransform);
	mainCamera = new Camera(shaders, glm::radians(60.0), double(Window::width) / (double)Window::height, 1, 1000, true);
	cameraTransform->addChild(mainCamera);

	playerCameraTransform = new Transform(glm::mat4(1), glm::rotate(2 * glm::half_pi<float>(), Window::UP), glm::vec3(0, 2.3, -.2));
	playerAstronaut->getTransform()->addChild(playerCameraTransform);
	playerCamera = new Camera(shaders, glm::radians(60.0), double(Window::width) / (double)Window::height, 1, 1000, false);
	playerCameraTransform->addChild(playerCamera);

	// Initialize lights
	Transform* pointLightTransform = new Transform(glm::scale(glm::vec3(1)), glm::mat4(1), glm::vec3(3, 5, 10));
	toonWorld->addChild(pointLightTransform);

	Geometry* texSphere = new TriangleGeometry("Objects/cube.objmodel");
	Materials* solidWhiteMat = new Materials(glm::vec3(1), glm::vec3(1), glm::vec3(1), 8);
	texSphere->setMaterials(solidWhiteMat);
	texSphere->setTexture(solidTexture);
	pointLightTransform->addChild(texSphere);

	pointLight = new PointLight(Window::shaders, glm::vec3(0), glm::vec3(1), glm::vec3(1, .005, 0.0));
	pointLightTransform->addChild(pointLight);

	Transform* flashLightTransform = new Transform(glm::scale(glm::vec3(.1)), glm::rotate(glm::half_pi<float>(), glm::vec3(-1, 0, 0)), glm::vec3(0, 1.5, 1));
	Geometry* cone = new TriangleGeometry("Objects/cone.objmodel");
	cone->setMaterials(solidWhiteMat);
	cone->setTexture(solidTexture);
	flashLightTransform->addChild(cone);
	playerAstronaut->getTransform()->addChild(flashLightTransform);

	spotLight = new SpotLight(Window::shaders, glm::vec3(1), glm::vec3(1, .005, 0.0), 0.3, 0);
	flashLightTransform->addChild(spotLight);

	float currentFrame = glfwGetTime();
	Time::deltaTime = currentFrame - time;
	time = currentFrame;

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
	delete toonWorld;

	delete trackBall;
	delete colliderTracker;
	delete collisionChecker;
	delete collisionPusher;

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

	if (mainCamera) {
		mainCamera->setAspect(double(Window::width) / (double)Window::height);
	}
	if (playerCamera) {
		playerCamera->setAspect(double(Window::width) / (double)Window::height);
	}
}

void Window::idleCallback()
{
	float currentFrame = glfwGetTime();
	Time::deltaTime = currentFrame - time;
	time = currentFrame;

	astroHandler->handleDeactivate();
	astroHandler->handleActivate();

	world->update(glm::mat4(1));
	toonWorld->update(glm::mat4(1));
	cameraLobbyTransform->update(glm::mat4(1));

	float forwardMovement = Window::moveForward - Window::moveBackward;
	float sidewaysMovement = Window::strafeRight - Window::strafeLeft;

	// Update movement
	if (forwardMovement != 0 || sidewaysMovement != 0) {
		glm::vec3 moveDir;

		// First person
		if (Window::firstPerson) {
			moveDir = glm::normalize(forwardMovement * playerAstronaut->getTransform()->getForward() + sidewaysMovement * playerAstronaut->getTransform()->getRight());
		}
		else {
			// Third Person
			glm::vec3 camForward = mainCamera->getForward();
			camForward.y = 0;
			if (glm::length(camForward) == 0) {
				camForward.y = 1;
			}

			glm::vec3 camRight = glm::cross(camForward, Window::UP);

			moveDir = glm::normalize(forwardMovement * camForward + sidewaysMovement * camRight);
		}

		playerAstronaut->move(moveDir, !Window::firstPerson);
	}
	else {
		playerAstronaut->stop();
	}

	astroHandler->updateActive();

	// Check for collisions
	std::vector<std::pair<Collider*, Collider*>> collidingColliders;
	for (int i = 0; i < colliderTracker->getNonKinematicColliders().size(); i++) {
		Collider* collider = colliderTracker->getNonKinematicColliders()[i];

		for (int j = i + 1; j < colliderTracker->getNonKinematicColliders().size(); j++) {
			Collider* otherNonKinematicCollider = colliderTracker->getNonKinematicColliders()[j];
			if (collider == otherNonKinematicCollider) {
				continue;
			}

			if (collisionChecker->collides(collider, otherNonKinematicCollider)) {
				std::pair<Collider*, Collider*> colliderPair;
				colliderPair.first = collider;
				colliderPair.second = otherNonKinematicCollider;
				collidingColliders.push_back(colliderPair);
			}
		}

		for (Collider* kinematicCollider : colliderTracker->getKinematicColliders()) {
			if (collisionChecker->collides(collider, kinematicCollider)) {
				std::pair<Collider*, Collider*> colliderPair;
				colliderPair.first = collider;
				colliderPair.second = kinematicCollider;
				collidingColliders.push_back(colliderPair);
			}
		}
	}

	// Update AI
	astroHandler->handleCollisions(collidingColliders);

	// Resolve collisions
	for (std::pair<Collider*, Collider*> colliderPair : collidingColliders) {
		collisionPusher->resolveCollision(colliderPair.first, colliderPair.second);
	}
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	world->draw(phongShader, glm::mat4(1));

	toonWorld->draw(toonShader, glm::mat4(1));

	cameraLobbyTransform->draw(toonShader, glm::mat4(1));

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();

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
			Window::strafeLeft = 1;
			break;
		case GLFW_KEY_D:
			Window::strafeRight = 1;
			break;
		case GLFW_KEY_W:
			Window::moveForward = 1;
			break;
		case GLFW_KEY_S:
			Window::moveBackward = 1;
			break;
		case GLFW_KEY_E:
			Window::mouseRotationVertical = !mouseRotationVertical;
			break;
		case GLFW_KEY_R:
			Window::firstPerson = !Window::firstPerson;

			Window::mainCamera->setActive(!Window::firstPerson);
			Window::playerCamera->setActive(Window::firstPerson);

			if (Window::firstPerson) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			else {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			break;
		default:
			break;
		}
	}
	else if (action == GLFW_RELEASE) {
		switch (key)
		{
		case GLFW_KEY_A:
			Window::strafeLeft = 0;
			break;
		case GLFW_KEY_D:
			Window::strafeRight = 0;
			break;
		case GLFW_KEY_W:
			Window::moveForward = 0;
			break;
		case GLFW_KEY_S:
			Window::moveBackward = 0;
			break;
		default:
			break;
		}
	}
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	// Zoom
	GLfloat fov = mainCamera->getFov();
	GLfloat newFov = glm::min(glm::max(glm::radians(5.0), fov - glm::radians(yoffset)), glm::radians(60.0));
	//mainCamera->setFov(newFov);

	cameraLobbyTransform->setScale(glm::scale(cameraLobbyTransform->getScale(), glm::vec3(1 - (.01 * glm::sign(yoffset)))));
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			Window::mouseRotation = true;
		}
		else if (action == GLFW_RELEASE) {
			Window::mouseRotation = false;
		}
	}
}

void Window::mouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
	// No longer track mouse movement outside of window
	if (std::isnan(xpos) || std::isnan(ypos)) {
		return;
	}

	// Wait until window is set up
	if (Window::width == 0 || Window::height == 0) {
		return;
	}

	float mouseXMovement = xpos - Window::mouseX;
	Window::mouseX = xpos;

	std::shared_ptr<TrackBallReturn> returnValues = trackBall->handleMove(xpos, Window::width, ypos, Window::height);
	if (firstPerson) {
		playerAstronaut->getTransform()->applyRotation(glm::rotate(glm::radians(mouseXMovement), -Window::UP));
	}
	else if (Window::mouseRotation) {
		glm::vec3 rotationAxis = returnValues.get()->rotationAxis;
		glm::vec3 horizontalRotation = glm::vec3(0, rotationAxis.y < 0 ? 1 : -1, 0);
		glm::vec3 verticalRotation = glm::vec3(rotationAxis.x < 0 ? 1 : -1, 0, 0);

		glm::vec3 axis = Window::mouseRotationVertical ? verticalRotation : horizontalRotation;

		if (Window::mouseRotationVertical) {
			cameraLobbyTransform->applyLocalRotation(glm::rotate(returnValues.get()->angle, axis));
		}
		else {
			cameraLobbyTransform->applyRotation(glm::rotate(returnValues.get()->angle, axis));
		}
	}
}

glm::vec3 Window::hexToRGB(int hex) {
	glm::vec3 rgb;
	rgb.r = ((hex >> 16) & 0xFF) / 255.0;
	rgb.g = ((hex >> 8) & 0xFF) / 255.0;
	rgb.b = ((hex) & 0xFF) / 255.0;

	return rgb;
}
