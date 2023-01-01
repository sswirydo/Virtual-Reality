#include "../headers/Player.hpp"

Player::Player(Model* model, Shader* shader, Physics* physics, LightSource* light) : Car(model, shader, physics, light)
{
    btRigidBody* body = this->getRigidBody();
    body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    ((BulletObject*)body->getUserPointer())->id = PLAYER;
    
    btTransform transform = this->getRigidBody()->getWorldTransform();
    transform.setOrigin(transform.getOrigin() + btVector3(0, 1, 0));
    this->getRigidBody()->setWorldTransform(transform);
}

bool Player::wasHit() 
{
    BulletObject* bo = (BulletObject*)this->getRigidBody()->getUserPointer();
    return bo->hit;
}

void Player::move(float deltaTime, glm::vec4 direction, int speedIncrease)
{
    btTransform transform;
    btVector3 position;
    btVector3 velocity = this->getRigidBody()->getLinearVelocity();;

    btVector3 acceleration(0, 0, 0);

    const int accelerationFactor = 5;
    const int steeringFactor = 2.5;
    const int brakingFactor =  4;

    if (direction.x) // FORWARD 
    {
        acceleration += btVector3(0, 0, -accelerationFactor);
    }
    if (direction.y) // BACKWARDS
    {
        acceleration += btVector3(0, 0, +accelerationFactor * brakingFactor);
    }
    if (direction.z) // LEFT
    {
        acceleration += btVector3(-accelerationFactor * steeringFactor, 0, 0);
    }
    if (direction.w) // RIGHT
    {
        acceleration += btVector3(+accelerationFactor * steeringFactor, 0, 0);
    }

    // Update the car's velocity based on the acceleration
    velocity += acceleration * deltaTime;

    // Velocity cap 
    float zSpeed = -velocity.getZ();
    int minSpeed = (20 + speedIncrease);
    if (zSpeed < (float)minSpeed)
    {
        velocity.setZ(-minSpeed);
    }

    this->getRigidBody()->setLinearVelocity(velocity);

    //Update the car's position based on the velocity
    transform = this->getRigidBody()->getWorldTransform();
    position = transform.getOrigin();
    position += velocity * deltaTime;
    transform.setOrigin(position);
    this->getRigidBody()->setWorldTransform(transform);

    this->getRigidBody()->activate(true);
}
