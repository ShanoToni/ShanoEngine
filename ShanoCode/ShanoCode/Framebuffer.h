#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>

class Framebuffer
{
public:
	Framebuffer();

	//screen quad
	void initFB();
	void useBuffer();
	void drawToBuffer();
	void useShadow(int i, GLint shadowLoc);
	void useOmniShadow(int i, GLint shadowLoc);
	void drawShadow();
	//depth buffer 2D
	inline void setWidthHeight(GLuint width, GLuint height) { w = width; h = height; }
	void initDB();
	//depth Buffer 3D
	void init3DDB();

	~Framebuffer();


private:
	//framebuffer id on GPU
	GLuint FBO;
	GLuint w, h;
	//texture id on GPU
	GLuint sceneTex;
};

