#include "Physics.h"



Physics::Physics()
{
	gravity = glm::vec3(0.0f, -9.8f, 0.0f);
	applied = false;
}

void Physics::init()
{
	Particle * p = new Particle();
	int count = 10;
	for (int i = 0; i < count + 1; i++)
	{
		//first test particle
		p = new Particle();
		p->getMesh()->setShader(shader);
		p->getMesh()->setTexture("Textures/particle.jpg");
		p->getMesh()->loadTexture();

		p->setPos(glm::vec3(0.0, 11.0  + getRandom(), 0.0));
		p->setVel(glm::vec3(getRandom() * 10, getRandom() * 3, 0.0f));

		particleList.push_back(p);
	}
	

	

	Body * body = new Body();
	body->setMesh(new Mesh(Mesh::CUBE));
	body->getMesh()->setShader(shader);
	body->getMesh()->setTexture("Textures/solid.jpg");
	body->getMesh()->loadTexture();

	body->setPos(glm::vec3(0.0f, 15.0f, 0.0f));
	body->setVel(glm::vec3(0.0f));
	body->setAngVel(glm::vec3(1.0f));

	body->scale(glm::vec3(0.5));
	body->setMass(1.0f);
	body->addCollider(TYPE::OBB);

	bodyList.push_back(body);

	body = new Body();
	body->setMesh(new Mesh(Mesh::CUBE));
	body->getMesh()->setShader(shader);
	body->getMesh()->setTexture("Textures/plane.jpg");
	body->getMesh()->loadTexture();

	body->setPos(glm::vec3(0.0f, 0.0f, 0.0f));
	body->setVel(glm::vec3(0.0f));
	body->setAngVel(glm::vec3(0.10f));

	body->scale(glm::vec3(20.0f, 0.05, 20.0f));
	body->setMass(10000000.0f);
	body->addCollider(TYPE::OBB);

	bodyList.push_back(body);
}


void Physics::update(float dt)
{
	//hook
	float ks = 10.0f;
	float kd = 5.0f;
	float rest = .01f;
	applySpring(ks*8, kd, rest, particleList[0], glm::vec3(0.0f, 10.0f, 0.0), glm::vec3(0.0));
	for (int i = 1; i < particleList.size(); i++)
	{
		applySpring(ks * 8/i, kd, rest, particleList[i-1], particleList[i]->getPos(), particleList[i]->getVel());
		applySpring(ks * 8/i, kd, rest, particleList[i], particleList[i-1]->getPos(), particleList[i-1]->getVel());
	}
	
	
	for (auto p : particleList)
	{
		p->setVel(p->getVel() + dt * p->getAcc());
		p->setPos(p->getPos() + dt * p->getVel());
		p->setAcc(glm::vec3(0.0));
	}

	for (auto b : bodyList)
	{
		//integration position
		b->getVel() = b->getVel() + dt * b->getAcc();
		b->setPos(b->getPos() + dt * (b->getVel()));

		// integration  rotation
		b->setAngVel(b->getAngVel() + dt * b->getAngAcc());
		glm::mat3 angVelSkew = glm::matrixCross3(b->getAngVel());
		glm::mat3 R = glm::mat3(b->getRotate());
		R += dt * angVelSkew * R;
		R = glm::orthonormalize(R);
		b->getMesh()->setRotate(glm::mat4(R));
		b->setAcc(glm::vec3(0.0));
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

	bodyList[0]->setAcc(bodyList[0]->getAcc() + gravity);
	for (auto b : bodyList)
	{
		b->setAcc(b->getAcc());

		if (glm::length(b->getAcc()) < 0.01f && glm::length(b->getVel()) < 0.01f)
		{
			b->setAcc(glm::vec3(0.0f));
			b->setVel(glm::vec3(0.0f));
		}
	}
	
	
}

void Physics::collide()
{
	for (auto p : particleList)
	{
		if (p->getPos().y < 0.1)
		{
			applySpring(7000, 150, 0.0, p, glm::vec3(p->getPos().x, 0.1f + 0.01f, p->getPos().z) , glm::vec3(0.0));
			p->setVel(glm::vec3(p->getVel().x * 0.99f, p->getVel().y, p->getVel().z * 0.99f));
			//p->setPos(glm::vec3(p->getPos().x, 0.35f , p->getPos().z));
		}

		if (p->getPos().y > 12.55)
		{
			applySpring(7000, 150, 0.0, p, glm::vec3(p->getPos().x, 12.55f - 0.01f, p->getPos().z), glm::vec3(0.0));
			p->setVel(glm::vec3(p->getVel().x * 0.99f, p->getVel().y, p->getVel().z * 0.99f));
			//p->setPos(glm::vec3(p->getPos().x, 0.35f , p->getPos().z));
		}

		if (p->getPos().x < -5.0f)
		{
			applySpring(7000, 150, 0.0, p, glm::vec3(-5.0f + 0.01f, p->getPos().y, p->getPos().z), glm::vec3(0.0));
			p->setVel(glm::vec3(p->getVel().x , p->getVel().y* 0.99f, p->getVel().z * 0.99f));
			//p->setPos(glm::vec3(p->getPos().x, 0.35f , p->getPos().z));
		}

		if (p->getPos().x > 5.0f)
		{
			applySpring(7000, 150, 0.0, p, glm::vec3(5.0f - 0.01f, p->getPos().y, p->getPos().z), glm::vec3(0.0));
			p->setVel(glm::vec3(p->getVel().x, p->getVel().y* 0.99f, p->getVel().z * 0.99f));
			//p->setPos(glm::vec3(p->getPos().x, 0.35f , p->getPos().z));
		}
	}

	for (int i = 0; i < bodyList.size(); i++)
	{
		auto& b1 = bodyList[i];

		for (int j = i + 1; j < bodyList.size(); j++)
		{
			auto& b2 = bodyList[j];

			auto collisionData = b1->canCollide(b2);
			
			if (collisionData.getHasIntersection())
			{
				b1->setVel(glm::vec3(0.0f));
				b1->setAcc(glm::vec3(0.0f));
				b1->setAngVel(glm::vec3(0.0f));
				b1->setPos( b1->getPos() - collisionData.getDepth() * collisionData.getNormal());
			}
		}
	}
}

void Physics::applySpring(float ks, float kd, float rest, Particle * p, glm::vec3 pos, glm::vec3 vel)
{	
	//distance
	float l = glm::distance(p->getPos(), pos);
	//unit vector
	glm::vec3 e = (p->getPos() - pos ) / l;
	//1d vel
	float vel1D = glm::dot(e, p->getVel());
	float vel1D2 = glm::dot(e, vel);
	float f = -ks * (rest - l) - kd * (-vel1D + vel1D2);

	p->setAcc(p->getAcc() - (f*e));
}

void Physics::applyImpulse(float currentTime, float delay, glm::vec3 applicationPoint, glm::vec3 impuse, Body * b)
{
	if (currentTime > delay && !applied)
	{
		b->getVel() += impuse / b->getMass();

		glm::vec3 appPoint = b->getPos() + applicationPoint;

		b->setAngVel(b->getAngVel() + b->getInvInertia() * glm::cross(appPoint - b->getPos(), impuse));

		applied = true;
	}
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
