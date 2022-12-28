#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Settings.hpp"

// Defines several possible options for camera movement. 
// Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera // Interface
{
public:
	Camera();
	virtual glm::mat4 getViewMatrix() = 0;
	virtual glm::mat4 getProjectionMatrix(float fov = 45.0, float near = 0.1f, float far = 8000.0f) = 0;

	virtual void ProcessKeyboard(Camera_Movement direction, float deltaTime) = 0 ;
	virtual void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) = 0;
	virtual void ProcessMouseScroll(float yoffset) = 0;

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);;
	float yaw = YAW;
	float pitch = PITCH;

	float MovementSpeed = SPEED;
	float MouseSensitivity = SENSITIVITY;
	float Zoom = ZOOM;
	float ScreenRatio = SCR_WIDTH/SCR_HEIGHT;
};

#endif // !CAMERA_HPP
