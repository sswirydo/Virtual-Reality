#include "../headers/Player.hpp"

Player::Player(Model& model, Shader& shader, Physics* physics,LightSource *light) : Car(model, shader, physics, light)
{
    btRigidBody* body = this->getRigidBody();
    body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    ((BulletObject*)body->getUserPointer())->id = PLAYER;
}


void Player::checkCollision() {
    this->physics->getWorld();
}




void Player::move(float deltaTime, glm::vec4 direction)
{
    btTransform transform;
    btVector3 position;
    btVector3 velocity;

    btVector3 acceleration(0, 0, 0);

    const int accelerationFactor = 5;

    if (direction.x) // FORWARD 
    {
        acceleration += btVector3(0, 0, -accelerationFactor);
    }
    if (direction.y) // BACKWARDS
    {
        acceleration += btVector3(0, 0, +accelerationFactor);
    }
    if (direction.z) // LEFT
    {
        acceleration += btVector3(-accelerationFactor, 0, 0);
    }
    if (direction.w) // RIGHT
    {
        acceleration += btVector3(+accelerationFactor, 0, 0);
    }

    // TODO : TEMPORARY FIX: making sure the car do not drift due to collision with the ground
    //transform = rigidBody->getWorldTransform();
    //transform.setRotation(btQuaternion(0, 0, 0, 1)); // set rotation to identity quaternion
    //this->getRigidBody()->setWorldTransform(transform);

    // Update the car's velocity based on the acceleration
    velocity = this->getRigidBody()->getLinearVelocity();
    velocity += acceleration * deltaTime; // deltaTime is the time elapsed since the last frame
    this->getRigidBody()->setLinearVelocity(velocity);

    //Update the car's position based on the velocity
    transform = this->getRigidBody()->getWorldTransform();
    position = transform.getOrigin();
    position += velocity * deltaTime;
    transform.setOrigin(position);
    this->getRigidBody()->setWorldTransform(transform);

    this->getRigidBody()->activate(true);
}
