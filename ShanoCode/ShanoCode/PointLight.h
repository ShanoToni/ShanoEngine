#pragma once
#include "Light.h"
class PointLight :
	public Light
{
public:
	PointLight();
	PointLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat cons, GLfloat lin, GLfloat exp);

	void useLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

	glm::vec3 getPos() { return position; }
	~PointLight();

protected:
	glm::vec3 position;

	GLfloat constant, linear, exponent;
};

