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

	//glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f);
	//float pitch;
	//float yaw;
	//float roll = 45;
	Object* player = NULL;

	float distanceFromPlayer = 50;
	float angleAroundPlayer = 0;

protected:
private:

};


#endif // !PLAYERCAMERA_HPP
