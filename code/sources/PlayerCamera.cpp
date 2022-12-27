#include "../headers/PlayerCamera.hpp"

PlayerCamera::PlayerCamera(Object* player) 
{
	this->player = player;
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

glm::mat4 PlayerCamera::getProjectionMatrix(float fov, float near, float far)
{
	return glm::perspective(fov, 4.0f/3.0f, near, far);
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
	this->position = glm::vec3(positionX, positionY, positionZ);
}

void PlayerCamera::computeZoom()
{
	//float zoomLevel = Zoom * 0.1f; // mouse wheel
	//distanceFromPlayer -= zoomLevel;
	distanceFromPlayer = 10;
}

void PlayerCamera::computePitch()
{
	//float pitchChange = Pitch * 0.1f; // mouse.getDY()
	//Pitch -= pitchChange;
	pitch = 20;
}

void PlayerCamera::computeAngleAroundPlayer()
{
	//float angleChange = 0 * 0.3f; // mouse.getDX()
	//angleAroundPlayer -= angleChange;
	angleAroundPlayer = -180;
}

float PlayerCamera::computeHorizontalDistance()
{
	return (float)(distanceFromPlayer * cos(glm::radians(pitch)));
}

float PlayerCamera::computeVerticalDistance()
{
	return (float)(distanceFromPlayer * sin(glm::radians(pitch)));
}