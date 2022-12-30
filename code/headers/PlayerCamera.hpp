#ifndef PLAYERCAMERA_HPP
#define PLAYERCAMERA_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"
#include "Object.hpp"

class Camera;

class PlayerCamera : public Camera {

public:
	PlayerCamera(Object* player);
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix(float fov = 45.0, float near = 0.1f, float far = 8000.0f);
	void move();
	void computeCameraPosition(float horizontalDistance, float verticalDistance);
	void computeZoom();
	void computePitch();
	void computeAngleAroundPlayer();
	float computeHorizontalDistance();
	float computeVerticalDistance();

	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(float yoffset);

	Object* player = nullptr;

	float distanceFromPlayer = 8;
	float angleAroundPlayer = 180;

protected:
private:

};


#endif // !PLAYERCAMERA_HPP
