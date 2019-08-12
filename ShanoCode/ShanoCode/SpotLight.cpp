#include "SpotLight.h"



SpotLight::SpotLight() : PointLight()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	edge = 0.0f;
	procEdge = cosf(glm::radians(edge));

	aInt = 0;
	dInt = 0;
}

SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue,
	GLfloat aIntensity, GLfloat dIntensity,
	GLfloat xPos, GLfloat yPos, GLfloat zPos,
	GLfloat cons, GLfloat lin, GLfloat exp,
	GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat edg) : PointLight(red, green, blue, aIntensity, dIntensity, xPos, yPos, zPos, cons, lin, exp)
{
	direction = glm::normalize(glm::vec3(xDir, yDir, zDir));

	edge = edg;
	procEdge = cosf(glm::radians(edge));
	aInt = ambientIntensity;
	dInt = diffuseIntensity;
}


void SpotLight::useLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat positionLocation, GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation, GLfloat directionLocation, GLfloat edgeLocation)
{
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(edgeLocation, procEdge);
	
}

void SpotLight::setFlash(glm::vec3 pos, glm::vec3 dir)
{
	position = pos;
	direction = dir;
}

void SpotLight::turnOff()
{
	ambientIntensity = 0.0f;
	diffuseIntensity = 0.0f;
}

void SpotLight::turnOn()
{
	ambientIntensity = aInt;
	diffuseIntensity = dInt;
}

SpotLight::~SpotLight()
{
}
