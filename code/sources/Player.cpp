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

const float roadXRange = 6.0f;
bool Player::wasHit() 
{
    // If discrete collision check with barriers failed e.g. due to a player lagging
    // we'll just check if its position is outside the road range (from -5.5f to 5.5f)
    bool out = false;
    float x = this->getWorldCoordinates().x;
    if (abs(x) > roadXRange) out = true;
    
    BulletObject* bo = (BulletObject*)this->getRigidBody()->getUserPointer();
    return out || bo->hit;
}

void Player::move(float deltaTime, glm::vec4 direction, int speedIncrease, bool enableSpeed)
{
    btTransform transform;
    btVector3 position;
    btVector3 velocity = this->getRigidBody()->getLinearVelocity();;

    btVector3 acceleration(0, 0, 0);

    const int accelerationFactor = 5;
    const int forwardFactor = 1.5;
    const int steeringFactor = 2.5;
    const int brakingFactor =  4;

    if (direction.x) // FORWARD 
    {
        acceleration += btVector3(0, 0, -accelerationFactor * forwardFactor);
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

    // Velocity cap  (i.e. game difficulty)
    if (enableSpeed) 
    {
        float zSpeed = -velocity.getZ();
        int minSpeed = (20 + speedIncrease);
        if (zSpeed < (float)minSpeed)
        {
            velocity.setZ(-minSpeed);
        }
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
