#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//class includes
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Window.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "assimp/Importer.hpp"
#include "Model.h"

// WINDOW DIMENTIONS
const GLint WIDTH = 800, HEIGHT = 600;

//Window
Window mainWindow;
//Mesh list
std::vector<Mesh*> meshList;
std::vector<Model*> modelList;
//Shader list
std::vector<Shader> shaderList;
//Lights
DirectionalLight mainLight;
//THreads


//Time
GLfloat dt = 0.0f;
GLfloat lastTime = 0.0f;

using namespace::std;
using namespace::glm;

//SCENE SHADER
//vertex Shader
static const char* vShader = "Shaders/shader.vert";
//Frag Shader
static const char* fShader = "Shaders/shader.frag";

//SCREENQUAD SHADER
//vertex Shader
static const char* sVShader = "Shaders/screen.vert";
//Frag Shader
static const char* sFShader = "Shaders/screen.frag";

//SKYBOX SHADERS
//vertex Shader
static const char* sbVShader = "Shaders/skybox.vert";
//Frag Shader
static const char* sbFShader = "Shaders/skybox.frag";

//SHADOW SHADERS
//vertex Shader
static const char* shVShader = "Shaders/shadow.vert";
//Frag Shader
static const char* shFShader = "Shaders/shadow.frag";


void CreateObjects()
{
	Mesh * screen = new Mesh();
	mainWindow.setScreenQuad(screen);

	//Plane
	Mesh *obj1 = new Mesh(Mesh::CUBE);
	
	obj1->initTransform();
	obj1->setTexture("Textures/brick.jpg");
	obj1->loadTexture();

	obj1->translate(vec3(0.0f, -0.3f, 15.0f));
	obj1->scale(vec3(100.0f, 0.2f, 100.0f));

	obj1->setMaterial(Material(.1f, 4));

	obj1->setShader(shaderList[0]);

	mainWindow.addMesh(obj1);


	//Sphere
	Mesh *obj2 = new Mesh(Mesh::SPHERE3);

	obj2->initTransform();
	obj2->translate(vec3(0.0f, 5.3f, 0.0f));
	obj2->scale(vec3(1.0f, 1.0f, 1.0f));

	obj2->setMaterial(Material(1.0f, 32));

	obj2->setShader(shaderList[0]);

	obj2->setTexture("Textures/mars.jpg");
	obj2->loadTexture();

	mainWindow.addMesh(obj2);

	//Sphere 2
	Mesh *obj3 = new Mesh(Mesh::SPHERE3);

	obj3->initTransform();
	obj3->translate(vec3(-8.0f, 8.3f, -12.0f));
	obj3->scale(vec3(2.0f, 2.0f, 2.0f));

	obj3->setMaterial(Material(1.0f, 32));

	obj3->setShader(shaderList[0]);

	obj3->setTexture("Textures/mars.jpg");
	obj3->loadTexture();

	mainWindow.addMesh(obj3);

	for (int i = 1; i < 10; i++)
	{
		Model * mod1 = new Model();
		mod1->loadModel("Models/lowpolytree.obj");
		mod1->translate(vec3(-5.0f, 5.0f, -10.0f * i));
		mod1->scale(glm::vec3(2.0, 2.0, 2.0));
		mod1->addShader(shaderList[0]);
		for (auto mesh : mod1->getMeshes())
		{
			mainWindow.addMesh(mesh);
		}
	}

	
}

void CreateSkybox() 
{

	std::vector<std::string> faces;
	faces.push_back("Textures/skybox/front.tga");
	faces.push_back("Textures/skybox/back.tga");
	faces.push_back("Textures/skybox/top.tga");
	faces.push_back("Textures/skybox/bottom.tga");
	faces.push_back("Textures/skybox/right.tga");
	faces.push_back("Textures/skybox/left.tga");

	Mesh * skycube = new Mesh(Mesh::SKYBOX);
	skycube->setTexture(faces);

	mainWindow.setSkyMesh(skycube);

	Shader * skyboxShader = new Shader();
	skyboxShader->CreateFromFiles(sbVShader,sbFShader);
	mainWindow.setSkyBoxShader(skyboxShader);


}

void CreateShaders() 
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();
	shader2->CreateFromFiles(sVShader, sFShader);
	mainWindow.setScreenShader(shader2);

	Shader *shader3 = new Shader();
	shader3->CreateFromFiles(shVShader, shFShader);
	mainWindow.setShadowShader(shader3);
	
	
}
int main()
{
	//window

	mainWindow = Window(1366, 768);
	mainWindow.Initialise();

	//-------------------SCENE SETUP-------------------------
	CreateShaders();
	CreateSkybox();
	CreateObjects();

	
	glm::vec3 dir = glm::normalize(glm::vec3(-50, -50, -50));
	mainLight = DirectionalLight(1.0f,1.0f,1.0f,0.001f,
					dir.x, dir.y, dir.z, 0.5f);

	mainWindow.addLight(&mainLight);

	mainWindow.addPLight(&PointLight(1.0f, 0.0f, 0.0f,		/* R G B*/
									0.02f, 3.0f,				/*ambientIntensity diffuseIntensity */
									-10.0f, 10.0f, 0.0f,	/* Location xyz*/
									0.2f, 0.1f, 0.1f));		/* Const Lin Exp*/

	mainWindow.addPLight(&PointLight(0.0f, 1.0f, 0.0f,
								0.02f, 4.0f,
								10.0f, 10.0f, 0.0f,
								0.3f, 0.2f, 0.1f));

	//Flashlight MANDITORY same list as the rest of the spot lights easier to render
	
	mainWindow.addSLight(&SpotLight(1.0f, 1.0f, 1.0f,
									0.2f, 20.0f,
									0.0f, 10.0f, 10.0f,
									0.5f, 0.3f, 0.2f,
									0.0f, -1.0f, 0.0f, 30.0f));  /* Direction xyz Edge */


	

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		//handle time in seconds
		GLfloat now = glfwGetTime();
		dt = now - lastTime;
		lastTime = now;
		// get and handle user input events
		glfwPollEvents();
		mainWindow.showFPS();
		mainWindow.update(dt);
		

		mainWindow.camera.keyControl(mainWindow.getKeys(), dt);
		mainWindow.camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
	

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Draw meshes
		mainWindow.draw();

		mainWindow.swapBuffers();
	}


}