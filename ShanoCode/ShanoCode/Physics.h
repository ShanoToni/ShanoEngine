#pragma once

#include "Particle.h"
#include <random>

class Physics
{
public:
	Physics();

	void init();
	void setShader(Shader s) { shader = s; }

	std::vector<Particle*> getParticles() { return particleList; }

	//integrate
	void update(float dt);
	void useForces();

	
	//simple collision
	void collide();

	void setSpringVars(float s, float d, float r) { ks = s; kd = d; rest = r; }
	void applySpring(Particle * p, glm::vec3 pos);

	~Physics();
private:
	float getRandom();
	std::vector<Particle*> particleList;
	Shader shader;
	//gravity
	glm::vec3 gravity;
	//spring variables
	float ks, kd, rest;
};

