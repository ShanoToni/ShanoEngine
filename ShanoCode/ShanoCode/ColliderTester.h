#pragma once
#include "Colliders.h"
#include "IntersectData.h"

#include <algorithm>
#include <cmath>
#include <array>
#include <limits>
#include <glm/gtx/norm.hpp>


static bool CMP(const float x, const float y)
{
	return (std::fabs((x)-(y)) <= std::numeric_limits<float>::epsilon() * std::fmax(1.0f, fmax(fabs(x), fabs(y))));
}

typedef struct SAT
{
	const glm::vec3& getMin(OBB obb);
	const glm::vec3& getMax(OBB obb);
	Interval getInterval(OBB obb, const glm::vec3 axis);
	bool overlapOnAxis(OBB obb1, OBB obb2, const glm::vec3 axis);
} SAT;


enum class TYPE
{
	SPHERE,
	OBB,
};

class ColliderTester
{
public:
	ColliderTester();
	ColliderTester(const TYPE & t) : type(t) {}

	inline void setType(TYPE t) { type = t; }
	inline  TYPE & getType()  { return type; }
	IntersectData intersect(OBB A, OBB B);
	glm::vec3 closestPoint(OBB obb, const glm::vec3 & point);
	bool PointInOBB(glm::vec3 point, OBB obb);
	
	Interval getInterval(OBB obb, glm::vec3 axis);

	std::vector<glm::vec3> getVertices(OBB obb);

	~ColliderTester();


private:
	TYPE type;

	std::vector<Line> getEdges(OBB obb);
	std::vector<Plane> getPlanes(OBB obb);
	bool clipToPlane(Plane plane, const Line& line, glm::vec3* outPoint);
	std::vector<glm::vec3> clipEdgesToOBB(const std::vector<Line>& edges, OBB obb);
	float penetrationDepth(OBB o1, OBB o2, const glm::vec3& axis, bool* outShouldFlip);
};

