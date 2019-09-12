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

	glm::vec3 getCameraPos();
	glm::vec3 getCameraDir();

	glm::mat4 calculateViewMatrix();

	void updateLookPos(glm::vec3 pos) { lookPos = pos; }
	bool getFreeCam() { return freeCam; }
	glm::vec3 getActorCamPos() { return glm::vec3(lookPos.x + front.x * 20, lookPos.y + front.y * 20, lookPos.z + front.z * 20); }

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

	float timer;
	bool freeCam;
	glm::vec3 lookPos;

	void Update();
};

