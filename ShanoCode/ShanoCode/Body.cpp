#include "Body.h"



Body::Body()
{
}

OBB Body::getOrientedBoxCollider()
{
	auto halfEdgeLen = getScaleVec();
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
	float w = getMesh()->getScale()[0][0] * 2.0f;
	float d = getMesh()->getScale()[2][2] * 2.0f;
	float h = getMesh()->getScale()[1][1] * 2.0f;

	glm::mat3 inertia = glm::mat3(0.0f);
	inertia[0][0] = getMass() / 12.0f * (h * h + d * d);
	inertia[1][1] = getMass() / 12.0f * (w * w + d * d);
	inertia[2][2] = getMass() / 12.0f * (w * w + h * h);

	invInertia = inverse(inertia);
}

IntersectData Body::canCollide(Body * other)
{
		return collider.intersect(getOrientedBoxCollider(), other->getOrientedBoxCollider());	
}

Body::~Body()
{
}
