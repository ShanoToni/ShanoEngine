#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "ColliderTester.h"

class Body
{
public:
	Body();

	Mesh * getMesh() { return mesh; }

	OBB getOrientedBoxCollider();

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
	float getCOR() const { return cor; }

	//SETS
	void setMesh(Mesh * m) { mesh = m; }
	void setAcc(const glm::vec3 & vect) { acc = vect; }
	void setVel(const glm::vec3 & vect) { vel = vect; }
	void setPos(const glm::vec3 & vect) { pos = vect; mesh->setPos(vect); }
	void setPos(int i, float p) { pos[i] = p; mesh->setPos(i, p); }

	void setCOR(float c) { cor = c; }

	void translate(const glm::vec3 & vect);
	void rotate(float angle, const glm::vec3 & vect);
	

	void setAngVel(const glm::vec3 omega) { angVel = omega; }
	void setAngAcc(const glm::vec3 alpha) { angAcc = alpha; }
	void setInvInertia(const glm::mat3 & invInert) { invInertia = invInert; }
	void setMass(float m) { mass = m; updateInvInertia(); }

	glm::vec3 getAngVel() { return angVel; }
	glm::vec3 getAngAcc() { return angAcc; }
	glm::mat3 getInvInertia() { return invInertia; }
	void scale(glm::vec3 vect) { mesh->scale(vect); updateInvInertia(); }
	inline const glm::vec3& getScaleVec() const { return glm::vec3(getScale()[0].x, getScale()[1].y, getScale()[2].z); }


	void updateInvInertia();

	//static or dynamic body
	void setStatic() { moving = false; }
	void setDynamic();
	bool getState() { return moving; }
	void updateState();
	void becomeStatic();
	void setAlwaysStatic() { staticBody = true; }
	bool isAlwaysStatic() { return staticBody; }
	int getCounter() { return counter; }


	ColliderTester getCollider() { return collider; }
	void addCollider(TYPE t) { collider.setType(t); }

	//collision detection
	IntersectData canCollide(Body * other);

	~Body();


private:
	Mesh * mesh;

	float mass;
	float cor;

	glm::vec3 acc;
	glm::vec3 vel;
	glm::vec3 pos;

	float density;
	glm::mat3 invInertia;
	glm::vec3 angVel;
	glm::vec3 angAcc;

	ColliderTester collider;
	bool moving;
	bool staticBody;
	const int counter = 10;
	std::vector<float> motions;
};

