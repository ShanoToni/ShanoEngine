#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition,
		glm::vec3 startUp,
		GLfloat startYaw,
		GLfloat startPitch,
		GLfloat startMoveSpeed,
		GLfloat startTurnSpeed);

	glm::vec3 getPos() { return position; }
	void keyControl(bool * keys, GLfloat dt);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::mat4 calculateViewMatrix();

	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	//left - right
	GLfloat yaw;
	// up - down
	GLfloat pitch;

	GLfloat movementSpeed;
	GLfloat turnSpeed;

	void Update();
};

