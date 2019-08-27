#pragma once
#include "Mesh.h"
class Particle
{
public:
	Particle();
	//GETS
	Mesh * getMesh() { return mesh; }

	//transform
	glm::mat4 getTranslate() const { return mesh->getTranslate(); }
	glm::mat4 getRotate() const { return mesh->getRotate(); }
	glm::mat4 getScale() const { return mesh->getScale(); }

	//variables dynamics
	glm::vec3 & getAcc() { return acc; }
	glm::vec3 & getVel() { return vel; }
	glm::vec3 & getPos() { return pos; }

	//physical properties
	float getMass() const { return mass; }

	//SETS
	void setMesh(Mesh * m) { mesh = m; }
	void setAcc(const glm::vec3 & vect) { acc = vect; }
	void setVel(const glm::vec3 & vect) { vel = vect; }
	void setPos(const glm::vec3 & vect) { pos = vect; mesh->setPos(vect); }
	void setPost(int i, float p) { pos[i] = p; mesh->setPos(i, p); }
	
	void setMass(float m) { mass = m; }

	void translate(const glm::vec3 & vect);
	void rotate(float angle, const glm::vec3 & vect);
	void scale(const glm::vec3 & vect);

	~Particle();
private:
	Mesh * mesh;
	float mass;
	
	glm::vec3 acc;
	glm::vec3 vel;
	glm::vec3 pos;
};

