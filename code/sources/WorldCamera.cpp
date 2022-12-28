#include "../headers/WorldCamera.hpp"

WorldCamera::WorldCamera(glm::vec3 position, glm::vec3 up)
{
    this->position = position;
    WorldUp = up;
    updateCameraVectors();
}

glm::mat4 WorldCamera::getViewMatrix()
{
    return glm::lookAt(this->position, this->position + Front, Up);
}

glm::mat4 WorldCamera::getProjectionMatrix(float fov, float near, float far)
{
    return glm::perspective(fov, this->ScreenRatio, near, far);
}

void WorldCamera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime * 2;
    if (direction == FORWARD)
        this->position += Front * velocity;
    if (direction == BACKWARD)
        this->position -= Front * velocity;
    if (direction == LEFT)
        this->position -= Right * velocity;
    if (direction == RIGHT)
        this->position += Right * velocity;
}

void WorldCamera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    this->yaw += xoffset;
    this->pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (this->pitch > 89.0f)
            this->pitch = 89.0f;
        if (this->pitch < -89.0f)
            this->pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void WorldCamera::ProcessMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

// calculates the front vector from the Camera's (updated) Euler Angles
void WorldCamera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    front.y = sin(glm::radians(this->pitch));
    front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}
