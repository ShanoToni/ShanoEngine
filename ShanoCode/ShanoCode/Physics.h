#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_cross_product.hpp>
#include <glm/gtx/orthonormalize.hpp>
#include "Particle.h"
#include "Body.h"
#include "BodyModel.h"
#include <random>
#include <thread>
#include <GLFW/glfw3.h>

class Physics
{
public:
	Physics();

	void init();
	void setShader(Shader s) { shader = s; }

	std::vector<Particle*> getParticles() { return particleList; }
	std::vector<Body*> getBodies();

	//integrate
	void update(float dt);
	void useForces();

	
	//simple collision
	void collide();

	void applySpring(float ks, float kd, float rest, Particle * p, glm::vec3 pos, glm::vec3 vel);
	void applySpring(float ks, float kd, float depth, glm::vec3 normal, glm::vec3 pos, glm::vec3 relVel, Body * b, glm::vec3 appPoint);
	void resolveColision(IntersectData data, Body * b,  Body * b2);
	void resolveColision(IntersectData data, BodyModel * b, Body * b2);
	
	//impuse check what happens
	void applyImpulse(float currentTime, float delay, glm::vec3 applicationPoint, glm::vec3 impuse, Body * b);
	void applyImpulse(glm::vec3 applicationPoint, glm::vec3 impuse, Body * b);
	void applyImpulse(glm::vec3 applicationPoint, glm::vec3 impuse, BodyModel * b);

	glm::vec3 getActorPos() { return bodyList[1]->getPos(); }
	void controlActor(bool * keys, bool freeCam, glm::vec3 camPos, float dt);
	void resetJumps();


	~Physics();
private:
	float getRandom();
	std::vector<Particle*> particleList;
	std::vector<Body*> bodyList;
	std::vector<BodyModel*> modelList;
	Shader shader;
	//gravity
	glm::vec3 gravity;
	//spring variables
	bool applied;
	float countW, countA, countS, countD, countSpace;
};

