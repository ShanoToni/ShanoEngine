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
#include "Framebuffer.h"

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

	//ADD MESHES
	void addMesh(Mesh * mesh) { meshes.push_back(mesh); }
	//DRAW MESHES
	void draw();

	GLfloat GetBufferWidth() { return bufferWidth; }
	GLfloat GetGufferHeight() { return bufferHeight; }

	//CONTROLS
	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* getKeys() { return keys; }

	GLfloat getXChange();
	GLfloat getYChange();

	//LIGHTS
	void addLight(DirectionalLight *light) { directional.push_back(light); }
	void addPLight(PointLight * light) { if (pointLightCount < MAX_POINT_LIGHTS) { points.push_back(light); pointLightCount++; } }
	void addSLight(SpotLight * light) { if (spotLightCount < MAX_SPOT_LIGHTS) { spots.push_back(light); spotLightCount++; } }

	void useDirLight(Mesh & mesh);
	void usePLight(Mesh & mesh);
	void useSLight(Mesh & mesh);

	void update(float dt);

	//FRAMEBUFFER
	void setScreenShader(Shader * screen) { frameshader = screen; }
	void setScreenQuad(Mesh * quad) { screenQuad = quad; }
	void drawScreenQuad();

	//SKYBOX
	void setSkyMesh(Mesh * mesh) { skycube = mesh; }
	void setSkyBoxShader(Shader * shade) { skyBoxShader = shade; }
	void drawSkyBox();

	void swapBuffers() {glfwSwapBuffers(mainWindow);}

	~Window();

	Camera camera;

	Framebuffer fb;
private:
	GLFWwindow *mainWindow;

	//Meshes
	std::vector<Mesh*> meshes;

	//directional light
	std::vector<DirectionalLight*> directional;

	//point light
	std::vector<PointLight*> points;
	int pointLightCount;

	//spotlight
	std::vector<SpotLight*> spots;
	int spotLightCount;
	bool flash;
	float flashTime;

	//Framebuffer
	Shader * frameshader;
	Mesh * screenQuad;
	int toggler;
	float toggleTime;

	//SKYBOX
	Mesh * skycube;
	Shader * skyBoxShader;

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

