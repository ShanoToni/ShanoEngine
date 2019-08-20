#include "Framebuffer.h"



Framebuffer::Framebuffer()
{
	FBO = 0;
	sceneTex[0] = 0;
	sceneTex[1] = 0;
	w = 1366;
	h = 768;
	
}

void Framebuffer::initFB()
{
	unsigned int framebuffer;
	unsigned int texColorBuffer[2];
	//create Framebuffer on the GPU
	glGenFramebuffers(1, &framebuffer);
	//bind to use
	
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	//gen texture
	glGenTextures(2, texColorBuffer);
	for (int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, texColorBuffer[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);

		// attach it to currently bound framebuffer object
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texColorBuffer[i], 0);
	}
	//attach rbo
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1366, 768);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	//tell opengl which color attachments are being used for rendering
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "FRAMEBUFFER ERROR" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	FBO = framebuffer;
	sceneTex[0] = texColorBuffer[0];
	sceneTex[1] = texColorBuffer[1];
}

void Framebuffer::useBuffer(int i, GLint Loc)
{
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0 + i);
	//glBindTexture(GL_TEXTURE_2D, sceneTex[0]);
	glBindTexture(GL_TEXTURE_2D, sceneTex[0]);
}

void Framebuffer::useBlurBuffer(int i, GLint Loc)
{
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0 + i);
	//glBindTexture(GL_TEXTURE_2D, sceneTex[0]);
	glBindTexture(GL_TEXTURE_2D, sceneTex[1]);
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
	glBindTexture(GL_TEXTURE_2D, sceneTex[2]);
}

void Framebuffer::useOmniShadow(int i, GLint shadowLoc)
{
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_CUBE_MAP, sceneTex[2]);
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
	sceneTex[2] = depthMap;
}

void Framebuffer::init3DDB()
{
	unsigned int depthMapFBO;
	//create Framebuffer on the GPU
	glGenFramebuffers(1, &depthMapFBO);
	const unsigned int SHADOW_WIDTH = 4048, SHADOW_HEIGHT = 4048;
	
	//create a texture 
	unsigned int depthCubemap;
	glGenTextures(1, &depthCubemap);
	//bind it
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	//set parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	//Bind texture to FB
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	FBO = depthMapFBO;
	sceneTex[2] = depthCubemap;

}

void Framebuffer::initBB()
{
	glGenFramebuffers(2, pingPongFBO);
	glGenTextures(2, pingPongTex);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingPongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingPongTex[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingPongTex[i], 0
		);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "FRAMEBUFFER ERROR" << std::endl;
		}
	}

}


Framebuffer::~Framebuffer()
{

}
