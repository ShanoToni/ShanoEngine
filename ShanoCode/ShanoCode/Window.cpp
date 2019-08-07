#include "Window.h"


using namespace glm;
Window::Window()
{
	width = 800;
	height = 600;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

int Window::Initialise()
{
	//Initialize GLFW
	if (!glfwInit())
	{
		std::cout << "GLFW broke!" << std::endl;		glfwTerminate();
		return 1;
	}
	// setup GLFW win properties
	// OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core profile = no backwards compatability
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forward compatability
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainWindow = glfwCreateWindow(width, height, "ShanoEngine", NULL, NULL);
	if (!mainWindow)
	{
		std::cout << "Window broke!" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Get Buffer Size Information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	//handle key and mouse
	CreateCallbacks();

	//Kill mouse
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW broke!" << std::endl;		
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	//tell glfw which window im handling keys for
	glfwSetWindowUserPointer(mainWindow, this);

	// Initialize cammera
	camera = Camera(vec3(0.0f), vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.3f);
}

void Window::draw(const Mesh & mesh)
{
	mat4 app_view = camera.calculateViewMatrix();
	mat4 app_projection = glm::perspective(45.0f, (GLfloat)GetBufferWidth() / GetGufferHeight(), 0.1f, 5000.0f);
	mesh.getShader().UseShader();
	// view and projection matrices

	// Get the uniform locations for MVP
	GLint modelLoc = glGetUniformLocation(mesh.getShader().shaderID, "Model");
	GLint viewLoc = glGetUniformLocation(mesh.getShader().shaderID, "View");
	GLint projLoc = glGetUniformLocation(mesh.getShader().shaderID, "Projection");
	//GLint rotateLoc = glGetUniformLocation(mesh.getShader().shaderID, "rotate");


	// Pass the matrices to the shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mesh.getModel()));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(app_view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(app_projection));
	//glUniformMatrix4fv(rotateLoc, 1, GL_FALSE, glm::value_ptr(mesh.getRotate()));


	glBindVertexArray(mesh.getVertexArrayObject());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndexBuffer());
	
	glDrawElements(GL_TRIANGLES, mesh.getNumIndices(), GL_UNSIGNED_INT, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}



GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0;
	return theChange;
}

Window::~Window()
{
	glfwTerminate();
}

void Window::CreateCallbacks()
{
	glfwSetKeyCallback(mainWindow, HandleKeys);
	glfwSetCursorPosCallback(mainWindow, HandleMouse);
}

void Window::HandleKeys(GLFWwindow * window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
		} 
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
		}
	}
}

void Window::HandleMouse(GLFWwindow * window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;

}
