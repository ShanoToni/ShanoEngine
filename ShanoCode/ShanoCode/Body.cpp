#include "Body.h"



Body::Body()
{
	moving = false;
	staticBody = false;
	jump = false;
}

OBB Body::getOrientedBoxCollider()
{
	auto halfEdgeLen = getScaleVec() * mesh->getMeshScale() / 2.0f;
	return OBB(getPos(), glm::mat3(getRotate()), halfEdgeLen);
}


void Body::translate(const glm::vec3 & vect)
{
	pos = pos + vect;
	mesh->translate(vect);
}

void Body::rotate(float angle, const glm::vec3 & vect)
{
	mesh->rotate(angle, vect);
}



void Body::updateInvInertia()
{
	float w = getMesh()->getScale()[0][0] * mesh->getMeshScale().x;
	float d = getMesh()->getScale()[2][2] * mesh->getMeshScale().z;
	float h = getMesh()->getScale()[1][1] * mesh->getMeshScale().y;

	glm::mat3 inertia = glm::mat3(0.0f);
	inertia[0][0] = getMass() / 12.0f * (h * h + d * d);
	inertia[1][1] = getMass() / 12.0f * (w * w + d * d);
	inertia[2][2] = getMass() / 12.0f * (w * w + h * h);

	invInertia = inverse(inertia);
}

void Body::setDynamic()
{
	if (!staticBody)
	{
		moving = true;
	}
}

void Body::updateState()
{
	if (isAlwaysStatic())
	{
		return;
	}

	float motion = glm::length(getVel()) + glm::length(getAngVel());
	
	motions.insert(motions.begin(), motion);

	if (motions.size() > counter)
	{
		motions.erase(motions.end()-1);
	}

	float motionSum = 0.0f;

	for (int i = 0; i < motions.size(); i++)
	{
		motionSum = motionSum + (motions[i] / pow(2, i+1));
	}


	if (motionSum < 0.5f)
	{
		this->setStatic();
	}
}

void Body::becomeStatic()
{
	if (!moving)
	{
		setAcc(glm::vec3(0.0));
		setVel(glm::vec3(0.0));
		setAngVel(glm::vec3(0.0));
	}
}

IntersectData Body::canCollide(Body * other)
{
		return collider.FindCollisionFeatures(getOrientedBoxCollider(), other->getOrientedBoxCollider());
}

Body::~Body()
{
}
