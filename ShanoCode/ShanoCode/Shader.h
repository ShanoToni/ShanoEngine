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
	void CreateFromFiles(const char* vertexLocation, const char* geometryLocation, const char* fragmentLocation);

	std::string ReadFile(const char* fileLocation);

	void addGeomShader(const char * fileLocation);
	
	void UseShader();
	void ClearShader();
	
	GLuint shaderID;
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

	~Shader();

private:

	void CompileShader(const char * vertexCode, const char * fragmentCode);
	void CompileShader(const char * vertexCode, const char * geometryCode, const char * fragmentCode);
};

