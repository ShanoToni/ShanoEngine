#pragma once
#include<iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp"

#include "Mesh.h"
#include "Camera.h"

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int Initialise();

	void draw(const Mesh & mesh);

	GLfloat GetBufferWidth() { return bufferWidth; }
	GLfloat GetGufferHeight() { return bufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* getKeys() { return keys; }

	GLfloat getXChange();
	GLfloat getYChange();

	
	void swapBuffers() {glfwSwapBuffers(mainWindow);}

	~Window();

	Camera camera;

private:
	GLFWwindow *mainWindow;
	

	GLint width, height;
	GLint bufferWidth, bufferHeight;
	bool keys[1024];

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;

	void CreateCallbacks();
	static void HandleKeys(GLFWwindow* window, int key, int code, int action,int mode);
	static void HandleMouse(GLFWwindow* window, double xPos, double yPos );
};

