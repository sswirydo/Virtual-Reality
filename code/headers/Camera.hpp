#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);;
	float yaw = YAW;
	float pitch = PITCH;
};

#endif // !CAMERA_HPP
