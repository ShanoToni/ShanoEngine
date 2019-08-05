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
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj2);

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

	CreateObjects();
	CreateShaders();

	mat4 proj = perspective(45.0f, mainWindow.GetBufferWidth()/mainWindow.GetBufferWidth(), 0.1f, 100.0f);
	
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;

	int degree = 0;
	float size = 0;

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		// get and handle user input events
		glfwPollEvents();

		//translation
		if (direction)
		{
			triOffSet += triIncrement;
		}
		else
		{
			triOffSet -= triIncrement;
		}

		if (abs(triOffSet) >= triMaxOffset)
		{
			direction = !direction;
		}
		//rotation
		

		if (degree < 360) 
		{
			degree+=1;
		}
		else
		{
			degree = 0;
		}

		if (size < 2) 
		{
			size+= 0.01f;
		}
		else
		{
			size = 0.4f;
		}

		// clear window
		glClearColor(0.0f,0.0f,0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Draw
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();

		mat4 model(1.0f);
		
		model = translate(model, vec3(0.0f, 0.0f, -3.0f));
		model = rotate(model, degree * toRadians, vec3(0.0f, 1.0f, 0.0f));
		model = scale(model, vec3(0.4, 0.4, 0.4));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, value_ptr(proj));
		
		//Draw meshes
		meshList[0]->RenderMesh();

		//new mesh
		model = mat4(1.0f);
		model = translate(model, vec3(0.0f, 1.0f, -3.0f));
		model = rotate(model, -degree * toRadians, vec3(0.0f, 1.0f, 0.0f));
		model = scale(model, vec3(0.4, 0.4, 0.4));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
		meshList[1]->RenderMesh();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}


}