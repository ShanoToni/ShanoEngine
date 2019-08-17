#pragma once
#include "Light.h"
class DirectionalLight :
	public Light
{
public:
	DirectionalLight();
	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
		GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity);

	void useLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation,
		GLfloat diffuseIntensityLocation, GLfloat directionLocation);

	inline glm::vec3 getPos() { return glm::vec3(direction * glm::vec3(-100)); }
	~DirectionalLight();

private:
	glm::vec3 direction;
};

