#include "../headers/WorldCamera.hpp"

// constructor with vectors
WorldCamera::WorldCamera(float screenRatio, glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, 0.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    ScreenRatio = screenRatio;
    this->position = position;
    WorldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    updateCameraVectors();
}
// constructor with scalar values
WorldCamera::WorldCamera(float screenRatio, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, 0.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    ScreenRatio = screenRatio;
    this->position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    this->yaw = yaw;
    this->pitch = pitch;
    updateCameraVectors();
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 WorldCamera::getViewMatrix()
{   //std::cout << "("<< Position.x << "," << Position.y << ","  << Position.z << ")"  << std::endl;
    return glm::lookAt(this->position, this->position + Front, Up);
}

glm::mat4 WorldCamera::getProjectionMatrix(float fov, float near, float far)
{
    return glm::perspective(fov, this->ScreenRatio, near, far);
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
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

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
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

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
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
