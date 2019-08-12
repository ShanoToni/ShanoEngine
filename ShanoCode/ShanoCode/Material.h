#pragma once

#include <GL/glew.h>

class Material
{
public:
	Material();
	Material(GLfloat sintensity, GLfloat shine);

	void useMaterial(GLuint specularIntensityLoc, GLuint shininessLoc);

	~Material();

private:
	GLfloat specularIntensity;
	GLfloat shininess;
};

