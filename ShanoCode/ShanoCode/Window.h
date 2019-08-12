#pragma once
#include<iostream>
#include<vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp"

#include "Mesh.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

constexpr int MAX_POINT_LIGHTS = 10;
constexpr int MAX_SPOT_LIGHTS = 10;
const float toRadians = 3.14159265f / 180.0f;

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int Initialise();
	void showFPS();

	void draw(Mesh & mesh);

	GLfloat GetBufferWidth() { return bufferWidth; }
	GLfloat GetGufferHeight() { return bufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* getKeys() { return keys; }

	GLfloat getXChange();
	GLfloat getYChange();

	void addLight(DirectionalLight *light) { directional.push_back(light); }
	void addPLight(PointLight * light) { if (pointLightCount < MAX_POINT_LIGHTS) { points.push_back(light); pointLightCount++; } }
	void addSLight(SpotLight * light) { if (spotLightCount < MAX_SPOT_LIGHTS) { spots.push_back(light); spotLightCount++; } }

	void useDirLight(Mesh & mesh);
	void usePLight(Mesh & mesh);
	void useSLight(Mesh & mesh);

	void updateFlashLight(float dt);

	void swapBuffers() {glfwSwapBuffers(mainWindow);}

	~Window();

	Camera camera;

private:
	GLFWwindow *mainWindow;
	std::vector<DirectionalLight*> directional;
	std::vector<PointLight*> points;
	int pointLightCount;

	std::vector<SpotLight*> spots;
	int spotLightCount;
	bool flash;
	float time;

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

