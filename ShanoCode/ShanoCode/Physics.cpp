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
	
	//Plane 0
	Body * body = new Body();
	body->setStatic();
	body->setAlwaysStatic();
	body->setMesh(new Mesh(Mesh::CUBE));
	body->getMesh()->setShader(shader);
	body->getMesh()->setTexture("Textures/plane.jpg");
	body->getMesh()->loadTexture();

	body->setPos(glm::vec3(0.0f, 0.0f, 0.0f));
	body->setVel(glm::vec3(0.0f));
	body->setAngVel(glm::vec3(0.0f));

	body->scale(glm::vec3(20.0f, 0.5, 20.0f));
	body->setMass(10.0f);
	body->addCollider(TYPE::OBB);

	bodyList.push_back(body);

	//Small Base Cube 1
	body = new Body();
	body->setDynamic();
	body->setMesh(new Mesh(Mesh::CUBE));
	body->getMesh()->setShader(shader);
	body->getMesh()->setTexture("Textures/solid.jpg");
	body->getMesh()->loadTexture();

	body->setPos(glm::vec3(-5.0f, 6.0f, 0.0f));
	body->setVel(glm::vec3(15.0f, 0.0f, 0.0f));
	body->setAngVel(glm::vec3(0.0f, 0.0f, 0.0f));

	body->scale(glm::vec3(0.5));
	body->setMass(10.0f);
	body->addCollider(TYPE::OBB);

	bodyList.push_back(body);

	//Rectangular cube on top 2
	body = new Body();
	body->setDynamic();
	body->setMesh(new Mesh(Mesh::CUBE));
	body->getMesh()->setShader(shader);
	body->getMesh()->setTexture("Textures/solid.jpg");
	body->getMesh()->loadTexture();

	body->setPos(glm::vec3(5.0f, 5.0f, 0.0f));
	body->setVel(glm::vec3(0.0f, -1.0f, 0.0f));
	body->setAngVel(glm::vec3(0.0f, 0.0f, 0.0f));

	body->scale(glm::vec3(0.5, 2.5, 2.0));
	body->setMass(1.0f);
	body->addCollider(TYPE::OBB);

	bodyList.push_back(body);

	//Next one after 3
	body = new Body();
	body->setDynamic();
	body->setMesh(new Mesh(Mesh::CUBE));
	body->getMesh()->setShader(shader);
	body->getMesh()->setTexture("Textures/solid.jpg");
	body->getMesh()->loadTexture();

	body->setPos(glm::vec3(7.0f, 5.0f, 0.0f));
	body->setVel(glm::vec3(0.0f, -1.0f, 0.0f));
	body->setAngVel(glm::vec3(0.0f, 0.0f, 0.0f));

	body->scale(glm::vec3(0.5, 2.5, 2.0));
	body->setMass(1.0f);
	body->addCollider(TYPE::OBB);

	bodyList.push_back(body);

	//rotating cube 4
	body = new Body();
	body->setDynamic();
	body->setMesh(new Mesh(Mesh::CUBE));
	body->getMesh()->setShader(shader);
	body->getMesh()->setTexture("Textures/solid.jpg");
	body->getMesh()->loadTexture();

	body->setPos(glm::vec3(9.0f, 5.0f, 0.0f));
	body->setVel(glm::vec3(0.0f, -1.0f, 0.0f));
	body->setAngVel(glm::vec3(0.0f, 0.0f, 0.0f));

	body->scale(glm::vec3(0.5, 2.5, 2.0));
	body->setMass(1.0f);
	body->addCollider(TYPE::OBB);

	bodyList.push_back(body);

	//Big cube on side 5
	body = new Body();
	body->setDynamic();
	body->setMesh(new Mesh(Mesh::CUBE));
	body->getMesh()->setShader(shader);
	body->getMesh()->setTexture("Textures/solid.jpg");
	body->getMesh()->loadTexture();

	body->setPos(glm::vec3(-5.0f, 35.0f, 0.0f));
	body->setVel(glm::vec3(0.0f, -1.0f, 0.0f));
	body->setAngVel(glm::vec3(0.0f, 0.0f, 0.0f));

	body->scale(glm::vec3(1.5, 1.5, 1.5));
	body->setMass(1.0f);
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


	

	for (int i = 0; i < bodyList.size(); i++)
	{
		
		//integration position
		bodyList[i]->getVel() = bodyList[i]->getVel() + dt * bodyList[i]->getAcc();
		bodyList[i]->setPos(bodyList[i]->getPos() + dt * (bodyList[i]->getVel()));

		// integration  rotation
		bodyList[i]->setAngVel(bodyList[i]->getAngVel() + dt * bodyList[i]->getAngAcc());
		glm::mat3 angVelSkew = glm::matrixCross3(bodyList[i]->getAngVel());
		glm::mat3 R = glm::mat3(bodyList[i]->getRotate());
		R += dt * angVelSkew * R;
		R = glm::orthonormalize(R);
		bodyList[i]->getMesh()->setRotate(glm::mat4(R));
		//reset acceleration
		bodyList[i]->setAcc(glm::vec3(0.0));
		bodyList[i]->setAngAcc(glm::vec3(0.0));

		//check if body has moved
		
		
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

	for (auto b : bodyList)
	{
		if (b->getState())
		{
			b->setAcc(b->getAcc() + gravity);
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
			if(b1->getState() || b2->getState())
			{
				auto collisionData = b1->canCollide(b2);
				if (collisionData.getHasIntersection())
				{
					bodyList[i]->updateState();
					bodyList[i]->becomeStatic();
					bodyList[j]->updateState();
					bodyList[j]->becomeStatic();
					if (bodyList[i]->getState() || bodyList[j]->getState())
					{
						resolveColision(collisionData, b1, b2);
					}
				}
			}
		}

		if (i == 4)
		{
			glm::vec3 vel = bodyList[i]->getVel();
			glm::vec3 angVel = bodyList[i]->getAngVel();
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

void Physics::applySpring(float ks, float kd, float depth, glm::vec3 normal, glm::vec3 pos, glm::vec3 relVel, Body * b, glm::vec3 appPoint)
{
	//distance
	float l = glm::distance(b->getPos(), pos);
	////unit vector
	glm::vec3 e = (b->getPos() - pos) / l;
	////1d vel
	float vel1D = glm::dot(e, relVel);
	//float vel1D2 = glm::dot(e, vel);
	float f = (-ks * depth - (kd * vel1D));
	glm::vec3 imp = glm::vec3((-f*normal));
	//std::cout << "Impulse : " << glm::to_string(imp) << std::endl;
	//std::cout << "RelVel : " << glm::to_string(relVel) << std::endl;

	
	applyImpulse(appPoint, imp , b);
}

void Physics::resolveColision(IntersectData data, Body * b, Body * b2)
{
	//get mass
	float invMass1 = 1.0f / b->getMass();
	float invMass2 = 1.0f / b2->getMass();
	float invMassSum = invMass1 + invMass2;
	if (invMassSum == 0.0f)
	{
		return;
	}

	glm::vec3 sumPoints = glm::vec3(0.0f);
	for (auto point : data.getContacts())
	{
		sumPoints += point;
	}
	if (data.getContacts().size() == 0)
	{
		return;
	}
	int simpleContactX = 1000 * sumPoints.x;
	int simpleContactY = 1000 * sumPoints.y;
	int simpleContactZ = 1000 * sumPoints.z;

	sumPoints = glm::vec3(simpleContactX / 1000.0f, simpleContactY / 1000.0f, simpleContactZ / 1000.0f);
	sumPoints = sumPoints / data.getContacts().size();

	//local point of collision for each body
	glm::vec3 r1 = sumPoints - b->getPos();
	glm::vec3 r2 = sumPoints - b2->getPos();

	
	glm::mat4 i1 = b->getInvInertia();
	glm::mat4 i2 = b2->getInvInertia();

	//relative velocity
	glm::vec3 relativeVel1 = (b->getVel() + glm::cross(b->getAngVel(), r1));
	glm::vec3 relativeVel2 = (b2->getVel() + glm::cross(b2->getAngVel(), r2));

	glm::vec3 relativeVel = relativeVel2 - relativeVel1;
							
	glm::vec3 normal = data.getNormal();
	
	
	
	//already sepparating dont bother
	if (glm::dot(relativeVel, normal) > 0.0f )
	{
		return;
	}

	float e = 0.3f;

	if (glm::length(relativeVel) < 1.0)
	{
		e = 0.1f;
	}

	float numerator= (-(1.0f + e) * glm::dot(relativeVel, normal));

	float d1 = invMassSum;

	glm::vec3 d2 = glm::cross((glm::vec3(glm::vec4(glm::cross(r1, normal), 1.0f) * i1)), r1);
	glm::vec3 d3 = glm::cross((glm::vec3(glm::vec4(glm::cross(r2, normal), 1.0f) * i2)), r2);
	float denominator = d1 + glm::dot(normal, d2 + d3);
	float j = (denominator == 0.0f) ? 0.0f : numerator / denominator;

	glm::vec3 impulse = normal * j;

	if (!b->isAlwaysStatic())
	{
		applyImpulse(r1, -impulse, b);
	}
	if (glm::length(b->getVel()) + glm::length(b->getAngVel()) > 2.0f)
	{
		b->setDynamic();
	}

	if (!b2->isAlwaysStatic())
	{
		applyImpulse(r2, impulse, b2);
	}
	if (glm::length(b2->getVel()) + glm::length(b2->getAngVel()) > 2.0f)
	{
		b2->setDynamic();
	}
	
	if (b->isAlwaysStatic())
	{
		auto halfDepth = data.getDepth() / 2.0f;
		b2->setPos(b2->getPos() + halfDepth * normal);
	}
	else if (b2->isAlwaysStatic())
	{
		auto halfDepth = data.getDepth() / 2.0f;
		b->setPos(b->getPos() - halfDepth * normal);
	}
	else if (!b->getState())
	{
		auto halfDepth = data.getDepth() / 2.0f;
		b2->setPos(b2->getPos() + halfDepth * normal);
	}
	else if (!b2->getState())
	{
		auto halfDepth = data.getDepth() / 2.0f;
		b->setPos(b->getPos() - halfDepth * normal);
	}
	else 
	{
		auto halfDepth = data.getDepth() / 4.0f;
		b2->setPos(b2->getPos() + halfDepth * normal);
		b->setPos(b->getPos() - halfDepth * normal);
	}

	//FRICTION

	//tangental dir
	glm::vec3 t = relativeVel - (normal * glm::dot(relativeVel, normal));

	//does it exist
	if (CMP(glm::length2(t), 0.0f))
	{
		return;
	}
	t = glm::normalize(t);

	numerator = -glm::dot(relativeVel, t);
	d1 = invMassSum;
	d2 = glm::cross((glm::vec3(glm::vec4(glm::cross(r1, t), 1.0f) * i1)), r1);
	d3 = glm::cross((glm::vec3(glm::vec4(glm::cross(r2, t), 1.0f) * i2)), r2);
	denominator = d1 + glm::dot(t, d2 + d3);

	if (denominator == 0.0f)
	{
		return;
	}

	//friction impulse
	float jt = numerator / denominator;

	if (CMP(jt, 0.0f))
	{
		return;
	}
	//friction coefficient
	float friction = 0.8;

	//clamp values of friction
	if (jt > j * friction) 
	{
		jt = j * friction;
	}
	else if (jt < -j * friction) 
	{
		jt = -j * friction;
	}

	glm::vec3 tangentImpulse = t * jt;

	glm::vec3 vt = relativeVel - glm::dot(relativeVel, normal) * normal;
	float mu = 0.8f;
	glm::vec3 jFriction = -mu * glm::length(j) * glm::normalize(vt);

	if (glm::length(tangentImpulse) > 0.0f)
	{
		if (!b->isAlwaysStatic())
		{
			b->setVel(b->getVel() - (tangentImpulse / b->getMass()));
			b->setAngVel(b->getAngVel() - (b->getInvInertia() * glm::cross(r1, tangentImpulse)));
		}
		if (!b2->isAlwaysStatic())
		{
			b2->setVel(b2->getVel() + (tangentImpulse / b2->getMass()));
			b2->setAngVel(b2->getAngVel() + (b2->getInvInertia() * glm::cross(r2, tangentImpulse)));
		}
	}
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

void Physics::applyImpulse(glm::vec3 applicationPoint, glm::vec3 impuse, Body * b)
{
	b->setVel(b->getVel() + impuse / b->getMass());

	glm::vec3 appPoint = b->getPos() + applicationPoint;


	b->setAngVel(b->getAngVel() + b->getInvInertia() * glm::cross(applicationPoint, impuse));
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
