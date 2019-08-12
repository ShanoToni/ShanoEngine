#include "Material.h"



Material::Material()
{
	specularIntensity = 0.0f;
	shininess = 0.0f;
}

Material::Material(GLfloat sintensity, GLfloat shine)
{
	specularIntensity = sintensity;
	shininess = shine;
}

void Material::useMaterial(GLuint specularIntensityLoc, GLuint shininessLoc)
{
	glUniform1f(specularIntensityLoc, specularIntensity);
	glUniform1f(shininessLoc, shininess);
}


Material::~Material()
{
}
