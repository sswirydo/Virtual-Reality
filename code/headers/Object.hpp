#ifndef OBJECT_HPP
#define OBJECT_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <btBulletDynamicsCommon.h>

#include "Shader.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include "Physics.hpp"
#include "LightSource.hpp"

#include <string>
#include <iostream>
#include <vector>


class Car;
class Camera;

class Object
{
public:
    Object();
    Object(Model &model, Shader &shader, Physics* physics, LightSource *light);
    void render(Camera* camera);
    void setModel(Model &model);
    void setShader(Shader &shader);
    void setModelMatrix(glm::mat4 model);
    glm::mat4 getModelMatrix();
    virtual void Draw();
    void setRigidBody(btRigidBody* rigidBody);
    btRigidBody* getRigidBody();
    btCollisionShape* getCollisionShape();
    glm::vec3 getWorldCoordinates();
    glm::vec3 getRotation();

    // Simple operations on the gl model matrix.
    void translateModel(glm::vec3 vector);
    void rotateModel(float degrees, glm::vec3 axis);

    // Updates the rigidbody transform based on the gl model matrix.
    void forceUpdatePhysics();
    void forceTranslate(glm::vec3 vector);
    void forceRotate(float degrees, glm::vec3 axis);

    // Upades the gl model matrix based on the rigidbody transform.
    void updateModelFromPhysics();
    void translatePhysics(glm::vec3 vector);
    void rotatePhysics(float degrees, glm::vec3 axis);

protected:
    Model model;
    glm::mat4 modelMatrix = glm::mat4(1.0f);;
    LightSource *light;
    Shader shader;
    Physics* physics;
    glm::vec3 cameraPos;
    btRigidBody* rigidBody = NULL;
    btCollisionShape* collisionShape = NULL;
};


#endif