#pragma once
#pragma once
#include<glm/glm.hpp>
#include "glm/ext.hpp"
#include <iostream>
#include <vector>

typedef struct Interval
{
	float min;
	float max;
} Interval;

class IntersectData
{
public:
	IntersectData() :
		hasIntersection(false),
		distance(0.0f),
		normal(glm::vec3(0.0f, 0.0f, 1.0f)),
		depth(FLT_MAX),
		contacts(std::vector<glm::vec3>())
	{};

	IntersectData(const bool hasInter, const float dist) :
		hasIntersection(hasInter),
		distance(dist) {}

	IntersectData(const bool hasInter, glm::vec3 n, const float d, const std::vector<glm::vec3>& cont) :
		hasIntersection(hasInter),
		normal(n),
		depth(d),
		contacts(cont) {}

	inline bool getHasIntersection() { return hasIntersection; }
	inline void setHasIntersection(bool hasInter) { hasIntersection = hasInter; }
	inline float getDistance() { return distance; }
	inline float getDepth() { return depth; }
	inline void setDepth(float d) { depth = d; }
	inline std::vector<glm::vec3> getContacts() { return contacts; }
	inline void insertContact(std::vector<glm::vec3> c) { contacts = c; }
	inline void setNormal(glm::vec3 n) { normal = n; }
	inline glm::vec3 getNormal() { return normal; }

private:
	bool hasIntersection;
	float distance;

	glm::vec3 normal;
	float depth;
	std::vector<glm::vec3> contacts;
};