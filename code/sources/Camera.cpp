#include "../headers/Camera.hpp"

Camera::Camera(Window* window) { this->window = window; }


glm::mat4 Camera::getProjectionMatrix(float fov, float near, float far)
{
	//float screenRatio = (float)this->window->getWidth() / (float)this->window->getHeight();
	float aspectRatio = 4.0f/3.0f;//(float) SCR_HEIGHT / (float) SCR_WIDTH;
	return glm::perspective(fov, aspectRatio, near, far);
}
