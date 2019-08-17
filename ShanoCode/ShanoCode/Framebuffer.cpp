#include "Framebuffer.h"



Framebuffer::Framebuffer()
{
	FBO = 0;
	sceneTex = 0;
	w = 1366;
	h = 768;
	
}

void Framebuffer::initFB()
{
	unsigned int framebuffer;
	unsigned int texColorBuffer;
	//create Framebuffer on the GPU
	glGenFramebuffers(1, &framebuffer);
	//bind to use
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "FRAMEBUFFER ERROR" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	//gen texture
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1366, 768);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	FBO = framebuffer;
	sceneTex = texColorBuffer;
}

void Framebuffer::useBuffer()
{
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sceneTex);
}

void Framebuffer::drawToBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glDisable(GL_FRAMEBUFFER_SRGB);
	glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	//Depth Buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);


}

void Framebuffer::useShadow(int i, GLint shadowLoc)
{
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, sceneTex);
}

void Framebuffer::drawShadow()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_DEPTH_BUFFER_BIT);
}


void Framebuffer::initDB()
{
	unsigned int depthMapFBO;
	unsigned int depthMap;
	//create Framebuffer on the GPU
	glGenFramebuffers(1, &depthMapFBO);
	const unsigned int SHADOW_WIDTH = 4048, SHADOW_HEIGHT = 4048;

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);


	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	FBO = depthMapFBO;
	sceneTex = depthMap;
}

Framebuffer::~Framebuffer()
{

}
