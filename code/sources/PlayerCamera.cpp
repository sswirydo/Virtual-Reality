#include "../headers/PlayerCamera.hpp"

PlayerCamera::PlayerCamera(Window* window, Object* player) : Camera(window)
{
	this->player = player;
	this->pitch = 20;
}

glm::mat4 PlayerCamera::getViewMatrix() 
{
	this->move();
	glm::mat4 viewMatrix = glm::mat4(1.0f);
	viewMatrix = glm::rotate(viewMatrix, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
	viewMatrix = glm::rotate(viewMatrix, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	//viewMatrix = glm::rotate(viewMatrix, glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::vec3 cameraPos = glm::vec3(-this->position.x, -this->position.y, -this->position.z);
	viewMatrix = glm::translate(viewMatrix, cameraPos);
	return viewMatrix;
}

void PlayerCamera::move()
{
	computeZoom();
	computePitch();
	computeAngleAroundPlayer();
	float horizontalDistance = computeHorizontalDistance();
	float verticalDistance = computeVerticalDistance();
	computeCameraPosition(horizontalDistance, verticalDistance);
	this->yaw = 180 - (this->player->getRotation().y + angleAroundPlayer);
}

void PlayerCamera::computeCameraPosition(float horizontalDistance, float verticalDistance)
{
	glm::vec3 playerPosition = this->player->getWorldCoordinates();
	glm::vec3 playerRotation = this->player->getRotation();
	float theta = playerRotation.y + angleAroundPlayer;
	float offsetX = horizontalDistance * sin(glm::radians(theta));
	float offsetZ = horizontalDistance * cos(glm::radians(theta));
	float positionX = playerPosition.x - offsetX;
	float positionZ = playerPosition.z - offsetZ;
	float positionY = playerPosition.y + verticalDistance;

	if (positionY < 0.1f)
		positionY = 0.1f;

	this->position = glm::vec3(positionX, positionY, positionZ);
}

void PlayerCamera::computeZoom()
{
	//float zoomLevel = Zoom * 0.1f; // mouse wheel
	//distanceFromPlayer -= zoomLevel;
	//distanceFromPlayer = 10;
}

void PlayerCamera::computePitch()
{
	//float pitchChange = Pitch * 0.1f; // mouse.getDY()
	//Pitch -= pitchChange;
	//pitch = 20;
}

void PlayerCamera::computeAngleAroundPlayer()
{
	//float angleChange = 0 * 0.3f; // mouse.getDX()
	//angleAroundPlayer -= angleChange;
	//angleAroundPlayer = -180;
}

float PlayerCamera::computeHorizontalDistance()
{
	return (float)(distanceFromPlayer * cos(glm::radians(pitch)));
}

float PlayerCamera::computeVerticalDistance()
{
	return (float)(distanceFromPlayer * sin(glm::radians(pitch)));
}



// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void PlayerCamera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	/* ... */
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void PlayerCamera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	this->angleAroundPlayer -= xoffset;
	this->pitch -= yoffset;

	//// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (this->pitch > 89.0f)
			this->pitch = 89.0f;
		if (this->pitch < -89.0f)
			this->pitch = -89.0f;
	}
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void PlayerCamera::ProcessMouseScroll(float yoffset)
{
	distanceFromPlayer -= (float)yoffset;
	if (distanceFromPlayer < 3.0f)
		distanceFromPlayer = 3.0f;
	if (distanceFromPlayer > 45.0f)
		distanceFromPlayer = 45.0f;
}