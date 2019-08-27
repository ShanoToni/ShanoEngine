#include "Physics.h"



Physics::Physics()
{
	gravity = glm::vec3(0.0f, -9.8f, 0.0f);
}

void Physics::init()
{
	Particle * p = new Particle();

	for (int i = 0; i < 101; i++)
	{
		//first test particle
		p = new Particle();
		p->getMesh()->setShader(shader);
		p->getMesh()->setTexture("Textures/particle.jpg");
		p->getMesh()->loadTexture();

		p->setPos(glm::vec3(0.0, 11.0, 0.0));
		p->setVel(glm::vec3(getRandom() * 10, getRandom() * 3, 0.0f));

		particleList.push_back(p);
	}
	

	//hook
	ks = 5000.0f;
	kd = 5.0f;
	rest = 0.0f;
}


void Physics::update(float dt)
{
	
	for (auto p : particleList)
	{
		p->setVel(p->getVel() + dt * p->getAcc());
		p->setPos(p->getPos() + dt * p->getVel());
		p->setAcc(glm::vec3(0.0));
	}
	
}

void Physics::useForces()
{
	for (auto p : particleList)
	{
		p->setAcc(p->getAcc() + gravity);

		if (glm::length(p->getAcc()) < 0.01f && glm::length(p->getVel()) < 0.01f)
		{
			p->setAcc(glm::vec3(0.0f));
			p->setVel(glm::vec3(0.0f));
		}
	}
	
	//
}

void Physics::collide()
{
	for (auto p : particleList)
	{
		if (p->getPos().y < 0.65)
		{
			applySpring(p, glm::vec3(p->getPos().x, 0.55f - 0.1f, p->getPos().z));
			p->setVel(glm::vec3(p->getVel().x * 0.999f, p->getVel().y, p->getVel().z * 0.999f));
			//p->setPos(glm::vec3(p->getPos().x, 0.35f , p->getPos().z));
		}

		if (p->getPos().y > 12.55)
		{
			applySpring(p, glm::vec3(p->getPos().x, 12.55f - 0.1f, p->getPos().z));
			p->setVel(glm::vec3(p->getVel().x * 0.999f, p->getVel().y, p->getVel().z * 0.999f));
			//p->setPos(glm::vec3(p->getPos().x, 0.35f , p->getPos().z));
		}

		if (p->getPos().x < -5.0f)
		{
			applySpring(p, glm::vec3(-5.0f + 0.1f, p->getPos().y, p->getPos().z));
			p->setVel(glm::vec3(p->getVel().x , p->getVel().y* 0.999f, p->getVel().z * 0.999f));
			//p->setPos(glm::vec3(p->getPos().x, 0.35f , p->getPos().z));
		}

		if (p->getPos().x > 5.0f)
		{
			applySpring(p, glm::vec3(5.0f - 0.1f, p->getPos().y, p->getPos().z));
			p->setVel(glm::vec3(p->getVel().x, p->getVel().y* 0.999f, p->getVel().z * 0.999f));
			//p->setPos(glm::vec3(p->getPos().x, 0.35f , p->getPos().z));
		}
	}
}

void Physics::applySpring(Particle * p, glm::vec3 pos)
{
	
	//distance
	float l = glm::distance(p->getPos(), pos);
	//unit vector
	glm::vec3 e = (p->getPos() - pos ) / l;
	//1d vel
	float vel1D = glm::dot(e, p->getVel());
	float f = -ks * (rest - l) - kd * (-vel1D);

	p->setAcc(p->getAcc() - (f*e));
}

Physics::~Physics()
{
}

float Physics::getRandom()
{
	// Seed with real random number if available
	std::random_device r;
	// Create random number generator
	std::default_random_engine e(r());
	// Create a distribution - floats between -1.0 and 1.0
	std::uniform_real_distribution<float> distribution2(-1.0, 1.0);

	return distribution2(e);
}
