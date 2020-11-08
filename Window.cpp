#include "Window.h"


// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "GLFW Starter Project";

// Objects to Render
TriangleFacedModelLoader* Window::triangleFacedModelLoader;
GLfloat Window::pointSize;
TriangleFacedModel* Window::bunnyTriangleFacedModel;
TriangleFacedModel* Window::sandalTriangleFacedModel;
TriangleFacedModel* Window::bearTriangleFacedModel;
TriangleFacedModel* currTriangleFacedModel;

// Materials
Materials* Window::shinyMat;
Materials* Window::diffuseMat;
Materials* Window::shinyAndDiffuseMat;

// Light
glm::vec3 Window::pointLightLocation;
PointLight* Window::pointLight;
TriangleFacedModel* Window::pointLightModel;
Materials* Window::lightMat;


// Camera Matrices 
// Projection matrix:
glm::mat4 Window::projection; 

// View Matrix:
glm::vec3 Window::eyePos(0, 0, 20);			// Camera position.
glm::vec3 Window::lookAtPoint(0, 0, 0);		// The point we are looking at.
glm::vec3 Window::upVector(0, 1, 0);		// The up direction of the camera.
glm::mat4 Window::view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);

// Shader Program ID
GLuint Window::normalShaderProgram;
GLuint Window::realisticShaderProgram;
GLuint currentShader;

// Default window state values
bool Window::mouseRotation = false;
glm::vec3 Window::lastPoint = glm::vec3(0.0);

bool Window::initializeProgram() {

	// Create a shader program with a vertex shader and a fragment shader.
	normalShaderProgram = LoadShaders("shaders/normal.vert", "shaders/normal.frag");
	realisticShaderProgram = LoadShaders("shaders/realistic.vert", "shaders/realistic.frag");
	currentShader = normalShaderProgram;

	// Check the shader program.
	if (!(normalShaderProgram && realisticShaderProgram))
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	return true;
}

bool Window::initializeObjects()
{
	// Create the point file reader.
	triangleFacedModelLoader = new TriangleFacedModelLoader();

	// Create 3d models of objects.
	bunnyTriangleFacedModel = triangleFacedModelLoader->loadTriangleFacedModel("Objects/bunny.objmodel");
	sandalTriangleFacedModel = triangleFacedModelLoader->loadTriangleFacedModel("Objects/sandal.objmodel");
	bearTriangleFacedModel = triangleFacedModelLoader->loadTriangleFacedModel("Objects/bear.objmodel");

	// Set bunny to be the first to display
	currTriangleFacedModel = bunnyTriangleFacedModel;

	// Create the materials
	shinyMat = new Materials(glm::vec3(0.3, 0.1, 0.1), glm::vec3(0.0), glm::vec3(1.0), 1024);
	diffuseMat = new Materials(glm::vec3(0.1, 0.3, 0.1), glm::vec3(0.9), glm::vec3(0.0), 1.0);
	shinyAndDiffuseMat = new Materials(glm::vec3(0.1, 0.1, 0.3), glm::vec3(0.9), glm::vec3(0.9), 1024);

	// Assign default materials
	bunnyTriangleFacedModel->setMaterials(shinyMat);
	sandalTriangleFacedModel->setMaterials(diffuseMat);
	bearTriangleFacedModel->setMaterials(shinyAndDiffuseMat);

	// Create the point light
	pointLightLocation = glm::vec3(3.0, 3.0, 6.0);
	glm::vec3 lightColor = glm::vec3(0.6, 0.6, 0.8);
	pointLight = new PointLight(pointLightLocation, lightColor, glm::vec3(0.0, 1.0, 0.0));
	pointLight->sendLightToShader(realisticShaderProgram);
	pointLightModel = triangleFacedModelLoader->loadTriangleFacedModel("Objects/sphere.objmodel");
	pointLightModel->setLocation(pointLightLocation);
	pointLightModel->scaleObject(-8);
	lightMat = new Materials(lightColor, glm::vec3(0), glm::vec3(0), 0);
	pointLightModel->setMaterials(lightMat);

	return true;
}

void Window::cleanUp()
{
	// Deallocate the objects.
	delete triangleFacedModelLoader;
	delete bunnyTriangleFacedModel;
	delete sandalTriangleFacedModel;
	delete bearTriangleFacedModel;

	// Deallocate materials
	delete shinyMat;
	delete diffuseMat;
	delete shinyAndDiffuseMat;

	// Deallocate light.
	delete pointLight;
	delete pointLightModel;
    delete lightMat;

	// Delete the shader program.
	glDeleteProgram(normalShaderProgram);
	glDeleteProgram(realisticShaderProgram);
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
	Window::projection = glm::perspective(glm::radians(60.0), 
								double(width) / (double)height, 1.0, 1000.0);
}

void Window::idleCallback()
{
	// Perform any necessary updates here 
	currTriangleFacedModel->update();
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Render the objects
	currTriangleFacedModel->draw(view, projection, currentShader);
	pointLightModel->draw(view, projection, currentShader);

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

			// switch between the shapes
		case GLFW_KEY_1:
			currTriangleFacedModel = bunnyTriangleFacedModel;
			std::cout << "Displaying bunny" << std::endl;
			break;
		case GLFW_KEY_2:
			currTriangleFacedModel = sandalTriangleFacedModel;
			std::cout << "Displaying sandal" << std::endl;
			break;
		case GLFW_KEY_3:
			currTriangleFacedModel = bearTriangleFacedModel;
			std::cout << "Displaying bear" << std::endl;
			break;
		case GLFW_KEY_N:
			std::cout << "Switching shaders" << std::endl;
			if (currentShader == normalShaderProgram) {
				currentShader = realisticShaderProgram;
			}
			else {
				currentShader = normalShaderProgram;
			}
			break;
		default:
			break;
		}

		if (currentShader == realisticShaderProgram) {
			currTriangleFacedModel->setUseMaterials(true);
			pointLightModel->setUseMaterials(true);
		}
		else {
			currTriangleFacedModel->setUseMaterials(false);
			pointLightModel->setUseMaterials(false);
		}
	}
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	// Scale the offset to allow for more granular control
	float scalingFactor = 0.05;
	currTriangleFacedModel->scaleObject(yoffset * scalingFactor);
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			std::cout << "PRESSED MOUSE LEFT" << std::endl;
			Window::mouseRotation = true;
			double mouseXPos;
			double mouseYPos;
			glfwGetCursorPos(window, &mouseXPos, &mouseYPos);
			Window::lastPoint = trackBallMapping(mouseXPos, mouseYPos);
		}
		else if (action == GLFW_RELEASE) {
			std::cout << "RELEASED MOUSE LEFT" << std::endl;
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

	if (Window::mouseRotation) {
		glm::vec3 currPoint = trackBallMapping(xpos, ypos);
		glm::vec3 direction = currPoint - Window::lastPoint;
		GLfloat velocity = glm::length(direction);
		if (velocity > .0001) {
			glm::vec3 rotationAxis = glm::cross(Window::lastPoint, currPoint);
			GLfloat angle = glm::acos(glm::dot(Window::lastPoint, currPoint));

			// Ensure valid angle and rotation axis
			if (!std::isnan(angle) && glm::length(rotationAxis) != 0) {
				currTriangleFacedModel->rotateObject(angle, rotationAxis);
			}
		}
		Window::lastPoint = currPoint;
	}
}

glm::vec3 Window::trackBallMapping(double mouseXPos, double mouseYPos)
{
	glm::vec3 mouseProjectedVector;
	GLfloat distanceFromOrigin;
	mouseProjectedVector.x = (2.0f * mouseXPos - Window::width) / Window::width;
	mouseProjectedVector.y = (Window::height - 2.0f * mouseYPos) / Window::height;
	mouseProjectedVector.z = 0.0f;
	distanceFromOrigin = glm::length(mouseProjectedVector);
	distanceFromOrigin = glm::min(distanceFromOrigin, 1.0f);
	mouseProjectedVector.z = glm::sqrt(1.001f - distanceFromOrigin * distanceFromOrigin);
	return glm::normalize(mouseProjectedVector);
}
