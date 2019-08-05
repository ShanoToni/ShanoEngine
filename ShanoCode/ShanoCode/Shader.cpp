#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProj = 0;
}

void Shader::CreateFromString(const char * vertexCode, const char * fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char * vertexLocation, const char * fragmentLocation)
{

	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);

	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);

}

std::string Shader::ReadFile(const char * fileLocation)
{
	std::string content;
	std::ifstream filestream(fileLocation, std::ios::in);

	if (!filestream.is_open())
	{
		std::cout << "Failed to read ->" <<fileLocation<< std::endl;
		return "";
	}

	std::string line = "";
	while (!filestream.eof())
	{
		std::getline(filestream, line);
		content.append(line+"\n");
	}

	filestream.close();

	return content;
}

GLuint Shader::GetProjectionLocation()
{
	return uniformProj;
}

GLuint Shader::GetModelLocation()
{
	return uniformModel;
}


void Shader::UseShader()
{
	glUseProgram(shaderID);
}

void Shader::ClearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}
	uniformModel = 0;
	uniformProj = 0;
}

Shader::~Shader()
{
	ClearShader();
}

//privates
void Shader::CompileShader(const char * vertexCode, const char * fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID) {
		std::cout << "Error Creating Shader Program" << std::endl;
		return;
	}

	//add the shaders to the shader program
	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	//error handle
	GLint result = 0;
	GLchar elog[1024] = { 0 };

	glLinkProgram(shaderID);

	//validate linking of shader
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(elog), NULL, elog);
		std::cout << "Error linking Program " << elog << std::endl;
		return;
	}
	//validate program
	glValidateProgram(shaderID);
	// check validation
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(elog), NULL, elog);
		std::cout << "Error validating Program " << elog << std::endl;
		return;
	}

	uniformModel = glGetUniformLocation(shaderID, "Model");
	uniformProj = glGetUniformLocation(shaderID, "Projection");
}

void Shader::AddShader(GLuint theProgram, const char * shaderCode, GLenum shaderType)
{
	//create shader of desired type
	GLuint theShader = glCreateShader(shaderType);

	//get the code
	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	//get length of code
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	//modify the created shader with the code
	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	//error handle
	GLint result = 0;
	GLchar elog[1024] = { 0 };

	//validate linking of shader
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(shaderID, sizeof(elog), NULL, elog);
		std::cout << "Error compiling the " << shaderType << " shader " << elog << std::endl;
		return;
	}

	//attach compiled shader to program
	glAttachShader(theProgram, theShader);
}
