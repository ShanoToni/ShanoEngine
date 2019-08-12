#pragma once
#include "PointLight.h"
class SpotLight :
	public PointLight
{
public:
	SpotLight();
	SpotLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat cons, GLfloat lin, GLfloat exp, 
		GLfloat xDir, GLfloat yDir, GLfloat zDir,
		GLfloat edge);

	void useLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation,
		GLfloat diffuseIntensityLocation, GLfloat positionLocation,
		GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation,
		GLfloat directionLocation, GLfloat edgeLocation);

	void setFlash(glm::vec3 pos, glm::vec3 dir);
	void turnOff();
	void turnOn();

	~SpotLight();

private:
	glm::vec3 direction;

	GLfloat aInt, dInt;
	GLfloat edge, procEdge;
};

