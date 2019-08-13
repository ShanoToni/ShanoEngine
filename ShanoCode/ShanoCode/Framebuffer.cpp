#include "Framebuffer.h"



Framebuffer::Framebuffer()
{
	FBO = 0;
	sceneTex = 0;
	
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1366, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
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
	glBindTexture(GL_TEXTURE_2D, sceneTex);
}

void Framebuffer::drawToBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	//Depth Buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);


}


Framebuffer::~Framebuffer()
{

}
