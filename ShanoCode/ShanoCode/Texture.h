#pragma once
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <vector>

#include "stb_image.h"

class Texture
{
public:
	Texture();
	//regular tex construtor
	Texture(const char* fileLoc);
	//skybox constructor
	Texture(std::vector<std::string> faces);

	bool loadTexture();
	const char* getFileLoc() { return fileLocation; }
	void useTexture();
	void clearTexture();
	//get the skybox id
	GLuint getSkyBoxID() { return textureID; }

	~Texture();

private:
	GLuint textureID;
	int width, height, bitDepth;

	const char* fileLocation;
};

