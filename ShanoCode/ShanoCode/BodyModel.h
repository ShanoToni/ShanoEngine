#pragma once
#include "Model.h"
#include "Body.h"
#include "ColliderTester.h"
#include "Shader.h"

class BodyModel :
	public Model
{
public:
	BodyModel();

	void loadBodies();

	void setShader(Shader shader);

	OBB getOrientedBoxCollider();

	//variables dynamics
	glm::vec3 & getAcc() { return acc; }
	glm::vec3 & getVel() { return vel; }
	glm::vec3 & getPos() { return pos; }
	glm::mat4 getRotate() { return rotateMat; }

	//physical properties
	float getMass() const { return mass; }

	//SETS
	void setAcc(const glm::vec3 & vect);
	void setVel(const glm::vec3 & vect);
	void setPos(const glm::vec3 & vect);
	void setPos(int i, float p);// { pos[i] = p; mesh->setPos(i, p); }
	void setRotate(glm::mat4 rot);

	void translate(const glm::vec3 & vect);
	void rotate(float angle, const glm::vec3 & vect);


	void setAngVel(const glm::vec3 omega);
	void setAngAcc(const glm::vec3 alpha);
	void setInvInertia(const glm::mat3 & invInert) { invInertia = invInert; }
	void setMass(float m) { mass = m; updateInvInertia(); }

	glm::vec3 getAngVel() { return angVel; }
	glm::vec3 getAngAcc() { return angAcc; }
	glm::mat3 getInvInertia() { return invInertia; }
	void scale(glm::vec3 vect);
	inline const glm::vec3& getScaleVec() const { return scaleVec;}

	void updateInvInertia();

	//static or dynamic body
	void setStatic();
	void setDynamic();
	bool getState() { return moving; }
	void updateState();
	void becomeStatic();
	void setAlwaysStatic();
	bool isAlwaysStatic() { return staticBody; }
	int getCounter() { return counter; }

	//collision detection
	IntersectData canCollide(Body * other);

	ColliderTester getCollider() { return collider; }
	void addCollider(TYPE t) { collider.setType(t); }

	std::vector<Body*> getBodies() { return bodyList; }

	~BodyModel();

private:

	std::vector<Body*> bodyList;
	float mass;
	glm::vec3 acc;
	glm::vec3 vel;
	glm::vec3 pos;
	glm::mat3 invInertia;
	glm::vec3 angAcc;
	glm::vec3 angVel;
	glm::vec3 scaleVec;
	glm::vec3 sizeVec;

	ColliderTester collider;

	bool moving;
	bool staticBody;
	const int counter = 10;
	std::vector<float> motions;

	
	glm::mat4 rotateMat; // rotation matrix
	

};

