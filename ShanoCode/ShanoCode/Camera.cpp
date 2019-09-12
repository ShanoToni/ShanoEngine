#include "Camera.h"


Camera::Camera()
{
	position = glm::vec3(0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	freeCam = true;
	timer = 0.8f;
}

Camera::Camera(glm::vec3 startPosition,
	glm::vec3 startUp,
	GLfloat startYaw,
	GLfloat startPitch,
	GLfloat startMoveSpeed,
	GLfloat startTurnSpeed )
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	right = glm::vec3(1.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);

	movementSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;
	
	Update();

	freeCam = true;
	timer = 0.8f;
}

void Camera::keyControl(bool * keys, GLfloat dt)
{
	GLfloat vel = movementSpeed * dt;
	if (keys[GLFW_KEY_W])
	{
		position += front * vel;
	}
	if (keys[GLFW_KEY_A])
	{
		position -= right * vel;
	}
	if (keys[GLFW_KEY_S])
	{
		position -= front * vel;
	}
	if (keys[GLFW_KEY_D])
	{
		position += right * vel;
	}
	if (keys[GLFW_KEY_SPACE])
	{
		position += up * vel;
	}
	if (keys[GLFW_KEY_C])
	{
		position -= up * vel;
	}

	timer -= dt;
	if (keys[GLFW_KEY_TAB] && timer< 0.0f)
	{
		timer = 0.8f;
		freeCam = !freeCam;
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	//limit the pitch 
	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f) 
	{
		pitch = -89.0f;
	}

	Update();


}

glm::vec3 Camera::getCameraPos()
{
	return position;
}

glm::vec3 Camera::getCameraDir()
{
	return glm::normalize(front);
}

glm::mat4 Camera::calculateViewMatrix()
{
	//calculate the view matrix with lookat 
	if(freeCam)
		return glm::lookAt(position, position + front, worldUp);
	if(!freeCam)
		return glm::lookAt(glm::vec3(lookPos.x + front.x * 20, lookPos.y + front.y * 20, lookPos.z + front.z* 20), lookPos, worldUp);
}

Camera::~Camera()
{

}

void Camera::Update()
{
	//Determine the front of the camera using the angles of rotation
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	//front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
