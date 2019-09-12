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
#include "Particle.h"
#include "Physics.h"

// WINDOW DIMENTIONS
const GLint WIDTH = 800, HEIGHT = 600;

//Window
Window mainWindow;
//Mesh list
std::vector<Mesh*> meshList;
std::vector<Model*> modelList;
//Shader list
std::vector<Shader*> shaderList;
//Lights
DirectionalLight *  mainLight;
//Texture list
std::vector<Texture*> texList;

Model * mod1;

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

//normal map shader
static const char* nvShader = "Shaders/shadern.vert";
//Frag Shader
static const char* nfShader = "Shaders/shadern.frag";

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

//OMNI SHADOW SHADERS
//vertex Shader
static const char* oVShader = "Shaders/omni.vert";
//Frag Shader
static const char* oFShader = "Shaders/omni.frag";
//Geometry Shader
static const char* oGShader = "Shaders/omni.geom";

//BLUR SHADER
//Vert Shader
static const char* bVShader = "Shaders/blur.vert";
//Frag Shader
static const char* bFShader = "Shaders/blur.frag";

//PHYSICS

//Vert Shader
static const char* paVShader = "Shaders/particle.vert";
//Frag Shader
static const char* paFShader = "Shaders/particle.frag";

void CreateObjects()
{
	//PLANE
	float size = 20.0f;
	Texture * t = new Texture("Textures/wood.jpg");
	t->loadTexture();
	texList.push_back(t);

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			Mesh *obj1 = new Mesh(Mesh::CUBE);
	
			obj1->initTransform();
			obj1->setTexture(*texList[0]);
			obj1->loadTexture();

			obj1->scale(vec3(size , 1.0f, size));
			obj1->translate(vec3((-size * 5)  + i * size * 2, -2.0f, (-size * 5)  + j * size *2));

			obj1->setMaterial(Material(.1f, 4));

			obj1->setShader(*shaderList[0]);

			mainWindow.addMesh(obj1);
		}
	}

	//left wall
	Mesh *obj1 = new Mesh(Mesh::CUBE);

	obj1->initTransform();
	obj1->setTexture(*texList[0]);
	obj1->loadTexture();

	obj1->translate(vec3(-15.0,9.0,-20.0));
	obj1->scale(vec3(0.9, 12.9f, 20.9));

	obj1->setMaterial(Material(.01f, 4));

	obj1->setShader(*shaderList[0]);

	mainWindow.addMesh(obj1);

	//back wall

	obj1 = new Mesh(Mesh::CUBE);

	obj1->initTransform();
	obj1->setTexture(*texList[0]);
	obj1->loadTexture();

	obj1->translate(vec3(5.0, 9.0, -40.0));
	obj1->scale(vec3(20.9f, 12.9f, 0.9));

	obj1->setMaterial(Material(.01f, 4));

	obj1->setShader(*shaderList[0]);

	mainWindow.addMesh(obj1);

	//right wall
	obj1 = new Mesh(Mesh::CUBE);

	obj1->initTransform();
	obj1->setTexture(*texList[0]);
	obj1->loadTexture();

	obj1->translate(vec3(25.0, 9.0, -20.0));
	obj1->scale(vec3(0.9, 12.9f, 20.9));

	obj1->setMaterial(Material(.01f, 4));

	obj1->setShader(*shaderList[0]);

	mainWindow.addMesh(obj1);


	//Sphere
	Mesh *obj2 = new Mesh(Mesh::SPHERE3);

	obj2->initTransform();
	obj2->translate(vec3(-5.0f, 7.0f, -3.0f));
	obj2->scale(vec3(1.0f, 1.0f, 1.0f));


	obj2->setShader(*shaderList[0]);

	obj2->setTexture("Textures/plane.jpg");
	obj2->loadTexture();

	mainWindow.addMesh(obj2);

	//Sphere 2
	Mesh *obj3 = new Mesh(Mesh::SPHERE3);

	obj3->initTransform();
	obj3->translate(vec3(-3.0f, 14.3f, -12.0f));
	obj3->scale(vec3(2.0f, 2.0f, 2.0f));

	obj3->setMaterial(Material(1.0f, 32));

	obj3->setShader(*shaderList[0]);

	obj3->setTexture("Textures/mars.jpg");
	obj3->loadTexture();
	mainWindow.addMesh(obj3);

	// No normal map cube
	Mesh *obj4 = new Mesh(Mesh::CUBE);

	obj4->initTransform();
	obj4->translate(vec3(8.0f, 12.3f, -20.0f));
	obj4->scale(vec3(1.0f, 1.0f, 1.0f));

	obj4->setMaterial(Material(0.01f, 32));

	obj4->setShader(*shaderList[0]);

	obj4->setTexture("Textures/plane.jpg");

	obj4->loadTexture();
	mainWindow.addMesh(obj4);

	

	// Normal map cube
	Mesh *obj5 = new Mesh(Mesh::CUBE);

	obj5->initTransform();
	obj5->translate(vec3(13.0f, 8.3f, -15.0f));
	obj5->scale(vec3(2.0f, 2.0f, 2.0f));

	obj5->setMaterial(Material(0.05f, 32));

	obj5->setShader(*shaderList[1]);

	obj5->setTexture("Textures/brick.jpg");
	obj5->setNormalMap("Textures/bricknor.jpg");

	obj5->loadTexture();
	obj5->loadMap();
	mainWindow.addMesh(obj5);


		mod1 = new Model();
		mod1->loadModel("Models/lowpolytree.obj");
		mod1->translate(vec3(-5.0f, 5.0f, -17.0f ));
		mod1->scale(glm::vec3(2.0, 2.0, 2.0));
		mod1->addShader(*shaderList[0]);
		for (auto mesh : mod1->getMeshes())
		{
			mainWindow.addMesh(mesh);
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
	shaderList.push_back(shader1);

	Shader *shader11 = new Shader();
	shader11->CreateFromFiles(nvShader, nfShader);
	shaderList.push_back(shader11);

	Shader *shader2 = new Shader();
	shader2->CreateFromFiles(sVShader, sFShader);
	mainWindow.setScreenShader(shader2);

	Shader *shader3 = new Shader();
	shader3->CreateFromFiles(shVShader, shFShader);
	mainWindow.setShadowShader(shader3);
	
	Shader * shader4 = new Shader();
	shader4->CreateFromFiles(oVShader, oGShader, oFShader);
	mainWindow.setOmniShader(shader4);

	Shader * shader5 = new Shader();
	shader5->CreateFromFiles(bVShader, bFShader);
	mainWindow.setBlurShader(shader5);

	//Shader * shader7 = new Shader();
	//shader5->CreateFromFiles(paVShader, paFShader);
	//shaderList.push_back(shader7);
	
	
}

void createLights() 
{
	glm::vec3 dir = glm::normalize(glm::vec3(-50, -50, -50));
	mainLight = new DirectionalLight(1.0f, 1.0f, 1.0f, 0.01f,
		dir.x, dir.y, dir.z, 1.1f);

	mainWindow.addLight(mainLight);

//	mainWindow.addPLight(new PointLight(0.0f, 0.0f, 18.0f,
//										0.2f, 2.0f,
//										-5.0f, 7.0f, -3.0f,
//										0.3f, 0.2f, 0.1f));
//
//	mainWindow.addPLight(new PointLight(10.0f, 0.0f, 0.0f,		/* R G B*/
//										0.2f, 2.5f,			/*ambientIntensity diffuseIntensity */
//										8.0f, 12.3, -20.0f,		/* Location xyz*/
//										0.2f, 0.1f, 0.1f));		/* Const Lin Exp*/
//
////Flashlight MANDITORY same list as the rest of the spot lights easier to render
//
//	mainWindow.addSLight(new SpotLight(5.0f, 5.0f, 5.0f,
//		0.2f, 60.0f,
//		0.0f, 10.0f, 10.0f,
//		0.5f, 0.3f, 0.2f,
//		0.0f, -1.0f, 0.0f, 30.0f));  /* Direction xyz Edge */


}

int main()
{
	//window

	mainWindow = Window(1366, 768);
	mainWindow.Initialise();
	Mesh * screen = new Mesh();
	mainWindow.setScreenQuad(screen);

	//-------------------SCENE SETUP-------------------------
	CreateShaders();
	CreateSkybox();
	//CreateObjects();
	createLights();

	

	//Physics
	Physics phys = Physics();
	phys.setShader(*shaderList[0]);
	phys.init();

	for (auto & p : phys.getParticles())
	{
		mainWindow.addMesh(p->getMesh());
	}

	for (auto &b : phys.getBodies())
	{
		mainWindow.addMesh(b->getMesh());
	}
	

	// new time	
	const float dt = 0.007f;
	float accumulator = 0.0f;
	GLfloat currentTime = (GLfloat)glfwGetTime();

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
	
		//New frame time
		GLfloat newTime = (GLfloat)glfwGetTime();
		GLfloat frameTime = newTime - currentTime;

		//*******************************************************************************************************************
		frameTime *= 1.0;
		currentTime = newTime;
		accumulator += frameTime;

		//impuse
		//phys.applyImpulse(currentTime, 5, vec3(-1, -0.1, 0), vec3(1, 0, 0), phys.getBodies().at(0));
	
		while (accumulator >= dt)
		{
			phys.collide();
			phys.useForces();
			phys.update(dt);
			
			
			accumulator -= dt;

		}
		// get and handle user input events
		glfwPollEvents();
		mainWindow.showFPS();
		//mainWindow.update(dt);

		

		mainWindow.camera.keyControl(mainWindow.getKeys(), dt);
		mainWindow.camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		mainWindow.camera.updateLookPos(phys.getActorPos());
		phys.controlActor(mainWindow.getKeys(), mainWindow.camera.getFreeCam(), mainWindow.camera.getActorCamPos(),dt);
		phys.resetJumps();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Draw meshes
		//mainWindow.draw();
		mainWindow.draw();

		mainWindow.swapBuffers();
	}


}