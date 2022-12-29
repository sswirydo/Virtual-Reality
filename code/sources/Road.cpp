#include "../headers/Road.hpp"

Road::Road(Model& model, Shader& shader, Physics* physics, LightSource* light) : Object(model, shader, physics, light) 
{
    //Creates the ground shape
    btCollisionShape* groundShape = new btBoxShape(btVector3(107, 1, 100));
    btCollisionShape* borneShape_l = new btBoxShape(btVector3(0.5, 1, 100));
    btCollisionShape* borneShape_r = new btBoxShape(btVector3(0.5, 1, 100));

    btDefaultMotionState* borneShape_l_ms = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-7, 0, 0)));
    btDefaultMotionState* borneShape_r_ms = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(7, 0, 0)));

    btRigidBody::btRigidBodyConstructionInfo borne_l_RigidBodyCI(0, borneShape_l_ms, borneShape_l);
    btRigidBody::btRigidBodyConstructionInfo borne_r_RigidBodyCI(0, borneShape_r_ms, borneShape_r);
    btRigidBody* rigidBody_l = new btRigidBody(borne_l_RigidBodyCI);
    btRigidBody* rigidBody_r = new btRigidBody(borne_r_RigidBodyCI);
    
    //Stores on an array for reusing
    this->physics->addShape(groundShape);
    this->physics->addShape(borneShape_l);
    this->physics->addShape(borneShape_r);
    //Creates the ground rigidbody
    btRigidBody* groundRigidBody = createGroundRigidBodyFromShape(groundShape);

    this->rigidBody = groundRigidBody;
    this->otherBodies.push_back(rigidBody_l);
    this->otherBodies.push_back(rigidBody_r);

    //Adds it to the world
    physics->getWorld()->addRigidBody(this->rigidBody);
    physics->getWorld()->addRigidBody(rigidBody_l);
    physics->getWorld()->addRigidBody(rigidBody_r);

}

void Road::move(int pos) 
{
    this->translateFrom(glm::vec3(0, 0, -200 * pos), glm::mat4(1.0f));
    this->translateBarriers(pos);
}

void Road::translateBarriers(int pos)
{
    btTransform transformL;
    btTransform transformR;
    btRigidBody* barrierL = this->otherBodies[0];
    btRigidBody* barrierR = this->otherBodies[1];
    glm::mat4 transfL = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -200 * pos));
    glm::mat4 transfR = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -200 * pos));
    transfL = glm::translate(transfL, glm::vec3(-7, 1, 0)); // TODO: fix translate
    transfR = glm::translate(transfR, glm::vec3(7, 1, 0));
    transformL.setFromOpenGLMatrix((btScalar*)glm::value_ptr(transfL));
    transformR.setFromOpenGLMatrix((btScalar*)glm::value_ptr(transfR));
    barrierL->setWorldTransform(transformL);
    barrierR->setWorldTransform(transformR);
}

btRigidBody* Road::createGroundRigidBodyFromShape(btCollisionShape* groundShape)
{
    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(0, -1, 0));

    {
        //The ground is not dynamic, we set its mass to 0
        btScalar mass(0);

        //No need for calculating the inertia on a static object
        btVector3 localInertia(0, 0, 0);

        //using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, groundMotionState, groundShape, localInertia);

        return new btRigidBody(rbInfo);
    }
}