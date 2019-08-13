#include "Window.h"
#include <sstream>

using namespace glm;
Window::Window()
{
	width = 800;
	height = 600;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}

	pointLightCount = 0;
	spotLightCount = 0;
	flash = false;
	
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
	pointLightCount = 0;
	spotLightCount = 0;
	flash = false;
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

	// Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	//tell glfw which window im handling keys for
	glfwSetWindowUserPointer(mainWindow, this);

	// Initialize cammera
	camera = Camera(vec3(0.0f), vec3(0.0f, 5.0f, 0.0f), -90.0f*toRadians, 1.0f * toRadians, 8.0f, 0.3f);
	flashTime = 0.5f;
	toggleTime = 0.5f;

	//Set framebuffer
	fb = Framebuffer();
	fb.initFB();
	toggler = 0;

}

void Window::showFPS()
{
	static double previousSeconds = 0.0;
	static int frameCount;
	double elapsedSeconds;
	double currentSeconds = glfwGetTime();

	elapsedSeconds = currentSeconds - previousSeconds;

	// limit FPS refresh rate to 4 times per second
	if (elapsedSeconds > 0.25) {
		previousSeconds = currentSeconds;
		double FPS = (double)frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / FPS;

		std::ostringstream outs;
		outs.precision(3);
		outs << std::fixed
			<< " ShanoEngine " << "  "
			<< "FPS: " << FPS << "  "
			<< "Frame time: " << msPerFrame << "ms" << std::endl;
		glfwSetWindowTitle(mainWindow, outs.str().c_str());

		frameCount = 0;
	}
	frameCount++;
}

void Window::draw()
{
	
	fb.drawToBuffer();

	//Draw skybox before rest of geometry
	drawSkyBox();

	for (auto mesh : meshes)
	{
		mat4 app_view = camera.calculateViewMatrix();
		mat4 app_projection = glm::perspective(45.0f, (GLfloat)GetBufferWidth() / GetGufferHeight(), 0.1f, 1000.0f);
		mesh->getShader().UseShader();
		// view and projection matrices

		// Get the uniform locations for MVP
		GLint modelLoc = glGetUniformLocation(mesh->getShader().shaderID, "Model");
		GLint viewLoc = glGetUniformLocation(mesh->getShader().shaderID, "View");
		GLint projLoc = glGetUniformLocation(mesh->getShader().shaderID, "Projection");
		GLint eyePosLoc = glGetUniformLocation(mesh->getShader().shaderID, "eyePos");

		// Pass the matrices to the shader
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mesh->getModel()));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(app_view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(app_projection));
		glUniform3f(eyePosLoc, camera.getCameraPos().x, camera.getCameraPos().y, camera.getCameraPos().z);

		//get the position of the light count
		GLint pointLightCountLoc = glGetUniformLocation(mesh->getShader().shaderID, "pointLightCount");
		GLint spotLightCountLoc = glGetUniformLocation(mesh->getShader().shaderID, "spotLightCount");
		// Pass the count to shader
		glUniform1i(pointLightCountLoc, pointLightCount);
		glUniform1i(spotLightCountLoc, spotLightCount);
	

		//PAss Materials
		GLint specIntensityLoc = glGetUniformLocation(mesh->getShader().shaderID, "material.specIntensity");
		GLint shininessLoc = glGetUniformLocation(mesh->getShader().shaderID, "material.shininess");
		mesh->getMaterial().useMaterial(specIntensityLoc, shininessLoc);
	
		//DIRECTIONAL LIGHT
		useDirLight(*mesh);
		//POINTLIGHTS
		usePLight(*mesh);
		//SPOTLIGHTS
		useSLight(*mesh);


		mesh->useTexture();
		glBindVertexArray(mesh->getVertexArrayObject());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBuffer());

		glDrawElements(GL_TRIANGLES, mesh->getNumIndices(), GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	drawScreenQuad();
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

void Window::useDirLight(Mesh & mesh)
{
	for (auto dir : directional)
	{
		//lights
		GLint ambientColLoc = glGetUniformLocation(mesh.getShader().shaderID, "directionalLight.base.color");
		GLint ambientColInensityLoc = glGetUniformLocation(mesh.getShader().shaderID, "directionalLight.base.ambientIntesity");
		GLint diffuseDirLoc = glGetUniformLocation(mesh.getShader().shaderID, "directionalLight.direction");
		GLint diffuseIntensityLoc = glGetUniformLocation(mesh.getShader().shaderID, "directionalLight.base.diffuseIntensity");
		dir->useLight(ambientColInensityLoc, ambientColLoc,
			diffuseIntensityLoc, diffuseDirLoc);
	}
}

void Window::usePLight(Mesh & mesh)
{
	for (int i = 0; i < pointLightCount; i++)
	{
		char locBuff[100] = ("\n");
		snprintf(locBuff, sizeof(locBuff), "PointLights[%d].base.color", i);
		GLint ambientColLoc = glGetUniformLocation(mesh.getShader().shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "PointLights[%d].base.ambientIntesity", i);
		GLint ambientColInensityLoc = glGetUniformLocation(mesh.getShader().shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "PointLights[%d].base.diffuseIntensity", i);
		GLint diffuseIntensityLoc = glGetUniformLocation(mesh.getShader().shaderID, locBuff);


		snprintf(locBuff, sizeof(locBuff), "PointLights[%d].position", i);
		GLint posLoc = glGetUniformLocation(mesh.getShader().shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "PointLights[%d].constant", i);
		GLint constLoc = glGetUniformLocation(mesh.getShader().shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "PointLights[%d].linear", i);
		GLint linearLoc = glGetUniformLocation(mesh.getShader().shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "PointLights[%d].exponent", i);
		GLint exponentLoc = glGetUniformLocation(mesh.getShader().shaderID, locBuff);
		points.at(i)->useLight(ambientColInensityLoc, ambientColLoc, diffuseIntensityLoc, posLoc, constLoc, linearLoc, exponentLoc);
	}
}

void Window::useSLight(Mesh & mesh)
{
	for (int i = 0; i < spotLightCount; i++)
	{
		char locBuff[100] = ("\n");
		snprintf(locBuff, sizeof(locBuff), "SpotLights[%d].base.base.color", i);
		GLint ambientColLoc = glGetUniformLocation(mesh.getShader().shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "SpotLights[%d].base.base.ambientIntesity", i);
		GLint ambientColInensityLoc = glGetUniformLocation(mesh.getShader().shaderID, locBuff);
		snprintf(locBuff, sizeof(locBuff), "SpotLights[%d].base.base.diffuseIntensity", i);
		GLint diffuseIntensityLoc = glGetUniformLocation(mesh.getShader().shaderID, locBuff);


		snprintf(locBuff, sizeof(locBuff), "SpotLights[%d].base.position", i);
		GLint posLoc = glGetUniformLocation(mesh.getShader().shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "SpotLights[%d].base.constant", i);
		GLint constLoc = glGetUniformLocation(mesh.getShader().shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "SpotLights[%d].base.linear", i);
		GLint linearLoc = glGetUniformLocation(mesh.getShader().shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "SpotLights[%d].base.exponent", i);
		GLint exponentLoc = glGetUniformLocation(mesh.getShader().shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "SpotLights[%d].direction", i);
		GLint dirLoc = glGetUniformLocation(mesh.getShader().shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "SpotLights[%d].edge", i);
		GLint edgeLoc = glGetUniformLocation(mesh.getShader().shaderID, locBuff);

		spots[i]->useLight(ambientColInensityLoc, ambientColLoc, diffuseIntensityLoc,
							posLoc, constLoc, linearLoc, exponentLoc,
							dirLoc, edgeLoc);
	}
}

void Window::update(float dt)
{
	//FLASHLIGHT CONTROLS
	if (flashTime > .5f) 
	{
		if (keys[GLFW_KEY_F])
		{
			flash = !flash;
			flashTime = 0.0f;
		}
	}
	else 
	{
		flashTime += dt;
		if (flashTime > 100)
		{
			flashTime = 100;
		}
	}
	if (flash)
	{
		glm::vec3 lowerLight = camera.getCameraPos();
		lowerLight.y -= 0.3f;
		spots[0]->setFlash(lowerLight, camera.getCameraDir());
		spots[0]->turnOn();
	}
	else
	{
		spots[0]->turnOff();
	}

	if (toggleTime > .5f)
	{
		if (keys[GLFW_KEY_Q])
		{
			toggler++;
			toggleTime = 0.0f;
			if (toggler > 5)
			{
				toggler = 0;
			}
		}
	}
	else
	{
		toggleTime += dt;
		if (toggleTime > 100)
		{
			toggleTime = 100;
		}
	}
	
}

void Window::drawScreenQuad()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	frameshader->UseShader();

	GLuint toggleLoc = glGetUniformLocation(frameshader->shaderID, "toggle");
	glUniform1i(toggleLoc, toggler);
	//get the position of the light count
	glBindVertexArray(screenQuad->getVertexArrayObject());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screenQuad->getIndexBuffer());
	fb.useBuffer();
	glDrawElements(GL_TRIANGLES, screenQuad->getNumIndices(), GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Window::drawSkyBox()
{
	mat4 app_view = glm::mat4(glm::mat3(camera.calculateViewMatrix()));
	mat4 app_projection = glm::perspective(45.0f, (GLfloat)GetBufferWidth() / GetGufferHeight(), 0.1f, 1000.0f);

	glDepthMask(GL_FALSE);
	skyBoxShader->UseShader();

	GLint viewLoc = glGetUniformLocation(skyBoxShader->shaderID, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(app_view));

	GLint projLoc = glGetUniformLocation(skyBoxShader->shaderID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(app_projection));

	glBindVertexArray(skycube->getVertexArrayObject());

	glBindTexture(GL_TEXTURE_CUBE_MAP, skycube->getTexture().getSkyBoxID());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skycube->getIndexBuffer());

	glDrawElements(GL_TRIANGLES, skycube->getNumIndices(), GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
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
