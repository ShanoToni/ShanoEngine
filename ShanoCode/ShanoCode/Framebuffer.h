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
	void useBuffer(int i, GLint Loc);
	void drawToBuffer();
	GLuint getTex(int idx) { return sceneTex[idx]; }
	//shadows
	void useShadow(int i, GLint shadowLoc);
	void useOmniShadow(int i, GLint shadowLoc);
	void drawShadow();
	//depth buffer 2D
	void initDB();
	inline void setWidthHeight(GLuint width, GLuint height) { w = width; h = height; }
	//depth Buffer 3D
	void init3DDB();
	//blur buffer
	void useBlurBuffer(int i, GLint Loc);
	void initBB();
	GLuint getPingPongFBO(int idx) { return pingPongFBO[idx]; }
	GLuint getPingPongTex(int idx) { return pingPongTex[idx]; }

	~Framebuffer();


private:
	//framebuffer id on GPU
	GLuint FBO;
	GLuint w, h;
	//texture id on GPU
	GLuint sceneTex[3];
	//Gaussian blur fb
	GLuint pingPongFBO[2];
	GLuint pingPongTex[2];
};

