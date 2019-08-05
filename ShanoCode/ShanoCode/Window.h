#pragma once
#include<iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int Initialise();

	GLfloat GetBufferWidth() { return bufferWidth; }
	GLfloat GetGufferHeight() { return bufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	void swapBuffers() {glfwSwapBuffers(mainWindow);}

	~Window();

private:
	GLFWwindow *mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;
};

