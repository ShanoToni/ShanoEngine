#pragma once
#include<iostream>
#include<string>
#include <fstream>

#include <GL/glew.h>

class Shader
{
public:
	Shader();

	void CreateFromString(const char * vertexCode, const char * fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();
	GLuint GetViewLocation();

	void UseShader();
	void ClearShader();
	
	GLuint shaderID;

	~Shader();

private:
	GLuint uniformProj, uniformModel, uniformView;

	void CompileShader(const char * vertexCode, const char * fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

