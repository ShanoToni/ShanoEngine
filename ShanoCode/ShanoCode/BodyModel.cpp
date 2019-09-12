#include "BodyModel.h"

using namespace Assimp;

BodyModel::BodyModel()
{
	rotateMat = glm::mat4(1);
}

void BodyModel::loadBodies()
{
	float minX, minY, minZ;
	float maxX, maxY, maxZ;

	
	for (int i = 0; i < meshlist.size(); i++)
	{
		Body * b = new Body();
		b->setMesh(meshlist[i]);


		for (int j = 0; j < b->getMesh()->getVertices().size(); j++)
		{

			if (i == 0 && j == 0)
			{
				//initialize X
				minX = b->getMesh()->getVertices()[j].getCoord().x;
				maxX = b->getMesh()->getVertices()[j].getCoord().x;

				//initialize y
				minY = b->getMesh()->getVertices()[j].getCoord().y;
				maxY = b->getMesh()->getVertices()[j].getCoord().y;

				//initialize z
				minZ = b->getMesh()->getVertices()[j].getCoord().z;
				maxZ = b->getMesh()->getVertices()[j].getCoord().z;
			}

			//create min and max X
			if (b->getMesh()->getVertices()[j].getCoord().x < minX)
			{
				minX = b->getMesh()->getVertices()[j].getCoord().x;
			}

			if (b->getMesh()->getVertices()[j].getCoord().x > maxX)
			{
				maxX = b->getMesh()->getVertices()[j].getCoord().x;
			}

			//create min and max Y
			if (b->getMesh()->getVertices()[j].getCoord().y < minY)
			{
				minY = b->getMesh()->getVertices()[j].getCoord().y;
			}

			if (b->getMesh()->getVertices()[j].getCoord().y > maxY)
			{
				maxY = b->getMesh()->getVertices()[j].getCoord().y;
			}

			//create min and max Z
			if (b->getMesh()->getVertices()[j].getCoord().z < minZ)
			{
				minZ = b->getMesh()->getVertices()[j].getCoord().z;
			}

			if (b->getMesh()->getVertices()[j].getCoord().z > maxZ)
			{
				maxZ = b->getMesh()->getVertices()[j].getCoord().z;
			}
		}
	}

	scaleVec = glm::vec3((maxX + minX) / 2.0, (maxY + minY) / 2.0, (maxZ + minZ) / 2.0);
	sizeVec = glm::vec3((maxX - minX) , (maxY - minY), (maxZ - minZ) );
	for (int i = 0; i < meshlist.size(); i++)
	{
		float minX, minY, minZ;
		float maxX, maxY, maxZ;
		

		Body * b = new Body();
		b->setMesh(meshlist[i]);
		b->setPos(meshlist[i]->getMeshPos());
		b->setVel(glm::vec3(0.0f));
		b->setAngVel(glm::vec3(0.0f));

		for (int j = 0; j < b->getMesh()->getVertices().size(); j++)
		{
			if (j == 0)
			{
				//initialize X
				minX = b->getMesh()->getVertices()[j].getCoord().x;
				maxX = b->getMesh()->getVertices()[j].getCoord().x;

				//initialize y
				minY = b->getMesh()->getVertices()[j].getCoord().y;
				maxY = b->getMesh()->getVertices()[j].getCoord().y;

				//initialize z
				minZ = b->getMesh()->getVertices()[j].getCoord().z;
				maxZ = b->getMesh()->getVertices()[j].getCoord().z;
			}

			//create min and max X
			if (b->getMesh()->getVertices()[j].getCoord().x < minX)
			{
				minX = b->getMesh()->getVertices()[j].getCoord().x;
			}

			if (b->getMesh()->getVertices()[j].getCoord().x > maxX)
			{
				maxX = b->getMesh()->getVertices()[j].getCoord().x;
			}

			//create min and max Y
			if (b->getMesh()->getVertices()[j].getCoord().y < minY)
			{
				minY = b->getMesh()->getVertices()[j].getCoord().y;
			}

			if (b->getMesh()->getVertices()[j].getCoord().y > maxY)
			{
				maxY = b->getMesh()->getVertices()[j].getCoord().y;
			}

			//create min and max Z
			if (b->getMesh()->getVertices()[j].getCoord().z < minZ)
			{
				minZ = b->getMesh()->getVertices()[j].getCoord().z;
			}

			if (b->getMesh()->getVertices()[j].getCoord().z > maxZ)
			{
				maxZ = b->getMesh()->getVertices()[j].getCoord().z;
			}
		}


		glm::vec3 bodyPos = glm::vec3((maxX + minX) / 2.0, (maxY + minY) / 2.0, (maxZ + minZ) / 2.0);
		b->setOffset(bodyPos - scaleVec);
		b->scale(glm::vec3(1.0f, 1.0f, 1.0f));
		b->setMass(1.0f);
		b->addCollider(TYPE::OBB);
		bodyList.push_back(b);
	}
}

void BodyModel::setShader(Shader shader)
{
	for (auto & b : bodyList)
	{
		b->getMesh()->setShader(shader);
	}
}

OBB BodyModel::getOrientedBoxCollider()
{
	auto halfEdgeLen = sizeVec / 2.0f;
	return OBB(pos, glm::mat3(meshlist[0]->getRotate()), halfEdgeLen);
}

void BodyModel::setAcc(const glm::vec3 & vect)
{
	acc = vect;
	for (auto & b : bodyList)
	{
		b->setAcc(vect);
	}
}

void BodyModel::setVel(const glm::vec3 & vect)
{
	vel = vect;
	for (auto & b : bodyList)
	{
		b->setVel(vect);
	}
}

void BodyModel::setPos(const glm::vec3 & vect)
{
	pos = vect;
	for (auto & b : bodyList)
	{
		b->setPos(vect + b->getOffset());
		b->getMesh()->setPos(vect + b->getOffset());
	}
}

void BodyModel::setPos(int i, float p)
{
	pos[i] = p;

	for (auto & b : bodyList)
	{
		b->setPos(i, p); 
		b->getMesh()->setPos(i, p);
	}
}

void BodyModel::setRotate(glm::mat4 rot)
{
	rotateMat = rot;
	for (auto &b : bodyList)
	{
		b->getMesh()->setRotate(rot);
		b->setPos(pos + glm::vec3(glm::vec4(b->getOffset(), 1.0) /rot));
	}
}

void BodyModel::translate(const glm::vec3 & vect)
{
	pos = pos + vect;
	for (auto & b : bodyList)
	{
		b->setPos(vect);
	}
}

void BodyModel::rotate(float angle, const glm::vec3 & vect)
{
	for (auto & b : bodyList)
	{
		b->getMesh()->rotate(angle, vect);
	}
}

void BodyModel::setAngVel(const glm::vec3 omega)
{
	angVel = omega;
	for (auto & b : bodyList)
	{
		b->setAngVel(omega);
	}
}

void BodyModel::setAngAcc(const glm::vec3 alpha)
{
	angAcc = alpha;
	for (auto & b : bodyList)
	{
		b->setAngAcc(alpha);
	}
}


void BodyModel::scale(glm::vec3 vect)
{
	scaleVec = scaleVec * vect;
	sizeVec = sizeVec * vect;

	for (auto & b : bodyList)
	{
		b->getMesh()->scale(vect);
		b->updateInvInertia();
	}
	updateInvInertia();
}

void BodyModel::updateInvInertia()
{
	float w = sizeVec.x ;
	float d = sizeVec.z ;
	float h = sizeVec.y ;

	glm::mat3 inertia = glm::mat3(0.0f);
	inertia[0][0] = getMass() / 12.0f * (h * h + d * d);
	inertia[1][1] = getMass() / 12.0f * (w * w + d * d);
	inertia[2][2] = getMass() / 12.0f * (w * w + h * h);

	invInertia = inverse(inertia);
}

void BodyModel::setStatic()
{
	moving = false;
	for (auto &b : bodyList)
	{
		b->setStatic();
	}
}

void BodyModel::setDynamic()
{
	if (!staticBody)
	{
		moving = true;
		for (auto &b : bodyList)
		{
			b->setDynamic();
		}
	}
}

void BodyModel::updateState()
{
	if (staticBody)
	{
		return;
	}

	float motion = glm::length(getVel()) + glm::length(getAngVel());

	motions.insert(motions.begin(), motion);

	if (motions.size() > counter)
	{
		motions.erase(motions.end() - 1);
	}

	float motionSum = 0.0f;

	for (int i = 0; i < motions.size(); i++)
	{
		motionSum = motionSum + (motions[i] / pow(2, i + 1));
	}


	if (motionSum < 0.9f)
	{
		this->setStatic();
	}
}

void BodyModel::becomeStatic()
{
	if (!moving)
	{
		setAcc(glm::vec3(0.0));
		setVel(glm::vec3(0.0));
		setAngVel(glm::vec3(0.0));
		for (auto &b : bodyList)
		{
			b->becomeStatic();
		}
	}
}

void BodyModel::setAlwaysStatic()
{
	staticBody = true;
	for (auto &b : bodyList)
	{
		b->setAlwaysStatic();
	}

}

IntersectData BodyModel::canCollide(Body * other)
{
	return collider.FindCollisionFeatures(getOrientedBoxCollider(), other->getOrientedBoxCollider());
}

BodyModel::~BodyModel()
{
}
