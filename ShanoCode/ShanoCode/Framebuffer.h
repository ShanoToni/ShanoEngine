#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>

class Framebuffer
{
public:
	Framebuffer();

	void initFB();
	void useBuffer();
	void drawToBuffer();

	~Framebuffer();

private:
	//framebuffer id on GPU
	GLuint FBO;
	//texture id on GPU
	GLuint sceneTex;
};

