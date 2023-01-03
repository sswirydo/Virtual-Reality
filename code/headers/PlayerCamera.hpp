#ifndef PLAYERCAMERA_HPP
#define PLAYERCAMERA_HPP

/*
	Implementation based on the following OpenGL 3D Game Tutorial in Java series
	https://www.youtube.com/playlist?list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP
*/

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"
#include "Object.hpp"

class Camera;

class PlayerCamera : public Camera {

public:
	PlayerCamera(Window* window, Object* player);
	glm::mat4 getViewMatrix();
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
