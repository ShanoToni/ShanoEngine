#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// WINDOW DIMENTIONS
const GLint WIDTH = 800, HEIGHT = 600;

int main()
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
	
	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "ShanoEngine", NULL, NULL);
	if (!mainWindow)
	{
		std::cout << "Window broke!" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Get Buffer Size Information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	//Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW broke!" << std::endl;		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	// Loop until window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		// get and handle user input events
		glfwPollEvents();

		// clear window
		glClearColor(1.0f,1.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(mainWindow);
	}


}