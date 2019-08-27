#include "Particle.h"



Particle::Particle()
{
	setMesh(new Mesh(Mesh::CUBE));
	mesh->initTransform();
	scale(glm::vec3(0.05f));
	setAcc(glm::vec3(0.0f));
	setVel(glm::vec3(0.0f));
	setMass(1.0f);
	
}
void Particle::translate(const glm::vec3 & vect) {
	pos = pos + vect;
	mesh->translate(vect);

}

void Particle::rotate(float angle, const glm::vec3 & vect) {
	mesh->rotate(angle, vect);

}

void Particle::scale(const glm::vec3 & vect) {
	mesh->scale(vect);

}

Particle::~Particle()
{

}
