#ifndef WORLDCAMERA_H
#define WORLDCAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Object.hpp"
#include "Camera.hpp"

class Object;

class WorldCamera : public Camera
{
public:
    WorldCamera(Window* window, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 getViewMatrix();

    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

    glm::vec3 Front = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

protected:
    void updateCameraVectors();

};
#endif

