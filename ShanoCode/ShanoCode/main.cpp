#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//class includes
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"



// WINDOW DIMENTIONS
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;


GLfloat dt = 0.0f;
GLfloat lastTime = 0.0f;


bool direction = true;
float triOffSet = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.005f;

using namespace::std;
using namespace::glm;

//vertex Shader
static const char* vShader = "Shaders/shader.vert";

static const char* fShader = "Shaders/shader.frag";

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};
	
	//Plane
	Mesh *obj1 = new Mesh(Mesh::CUBE);
	obj1->initTransform();
	obj1->translate(vec3(0.0f, -0.3f, 15.0f));
	obj1->scale(vec3(100.0f, 0.2f, 100.0f));
	obj1->setShader(shaderList[0]);
	meshList.push_back(obj1);

	//Sphere
	Mesh *obj2 = new Mesh(Mesh::SPHERE);
	obj2->initTransform();
	obj2->translate(vec3(0.0f, 10.3f, 1.0f));
	obj2->scale(vec3(1.0f, 1.0f, 1.0f));
	obj2->setShader(shaderList[0]);
	meshList.push_back(obj2);

	//CUBE
	Mesh *obj3 = new Mesh(Mesh::CUBE);
	obj3->initTransform();
	obj3->translate(vec3(4.0f, 10.3f, 1.0f));
	obj3->scale(vec3(1.0f, 1.0f, 1.0f));
	obj3->setShader(shaderList[0]);
	meshList.push_back(obj3);
}

void CreateShaders() 
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
	
}
int main()
{
	//window

	mainWindow = Window(800, 600);
	mainWindow.Initialise();

	//Create triangle

	CreateShaders();
	CreateObjects();
	



	mat4 proj = perspective(45.0f, mainWindow.GetBufferWidth()/mainWindow.GetBufferWidth(), 0.1f, 100.0f);
	
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;

	int degree = 0;
	float size = 0;

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		//handle time in seconds
		GLfloat now = glfwGetTime();
		dt = now - lastTime;
		lastTime = now;
		// get and handle user input events
		glfwPollEvents();

		mainWindow.camera.keyControl(mainWindow.getKeys(), dt);
		mainWindow.camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());


		// clear window
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Draw
		for (auto mesh : meshList)
		{
			mainWindow.draw(*mesh);
		}

		mainWindow.swapBuffers();
	}


}