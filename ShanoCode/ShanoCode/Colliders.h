#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>

typedef	struct	Line {
	glm::vec3 start;
	glm::vec3 end;
	inline	Line() {}
	inline	Line(const glm::vec3& s, const glm::vec3&	e) : start(s), end(e) {	}
}	Line;

class Plane
{
public:
	Plane() :normal(glm::vec3(0.0f, 1.0f, 0.0f)), distance(0.0f) { }
	Plane(const glm::vec3 normal, float distance) :
		normal(normal),
		distance(distance)
	{ }

	Plane normalized() { return Plane(normal / glm::length(normal), distance / glm::length(normal)); }

	inline const glm::vec3& getNormal() { return normal; }
	inline float getDistance() { return distance; }

private:
	glm::vec3 normal;
	float distance;
};

class OBB
{
public:
	OBB(const glm::vec3& position, const glm::mat3& orientation, const glm::vec3& size) :
		position(position), orientation(orientation), size(size)
	{}

	const glm::vec3& getPosition() { return position; }
	const glm::mat3& getOrientation() { return orientation; }
	const glm::vec3& getSize() { return size; }

private:
	glm::vec3 position;
	glm::mat3 orientation;
	glm::vec3 size;
};