#ifndef WORLDCAMERA_H
#define WORLDCAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "Object.hpp"
#include "Camera.hpp"

class Object;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};



// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class WorldCamera : public Camera
{
public:
    WorldCamera(float screenRatio,
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = YAW,
        float pitch = PITCH);
    WorldCamera(float screenRatio, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
    // void LookAt(Object* object);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix(float fov = 45.0, float near = 0.1f, float far = 8000.0f);
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

    // camera Attributes
    //glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    //float Yaw;
    //float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    float ScreenRatio;

protected:
    void updateCameraVectors();

};
#endif

