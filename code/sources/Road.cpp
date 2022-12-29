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



void Road::move(int nbOfRoads, int pos)
{
    
    glm::vec3 vector;
    if (pos == -1) {
        vector = glm::vec3(0.f, 0.f, -200 * nbOfRoads);
    }
    else { // init
        vector = glm::vec3(0, 0, -200 * pos);
    }

    this->translate(vector);
    this->translateBarriers(vector);
    this->moveLinkedCars(vector);
    this->moveLinkedObjects(vector);
}


void Road::moveLinkedObjects(glm::vec3 vector) // NOTE: simple objects without rigidbodies
{
    for (size_t t = 0; t < linkedObjects.size(); t++) {
        Object* obj = linkedObjects[t];
        obj->setModelMatrix(glm::translate(obj->getModelMatrix(), vector));
    }
}


void Road::moveLinkedCars(glm::vec3 vector)
{
    // as the obstacle cars move, we do not simly translate them
    // but position them at a certain position of the road
    // from which they will move forward

    /* ... */

    for (size_t t = 0; t < cars.size(); t++) {
        Car* car = cars[t];
        //car->resetStartZ();
        //car->translate();
    }
}


void Road::addCars(std::vector<Car*> cars) 
{
    this->cars = cars;
}

void Road::linkObject(Object* obj)
{
    this->linkedObjects.push_back(obj);
}

std::vector<Object*> Road::getLinkedObjects()
{
    return this->linkedObjects;
}



void Road::translateBarriers(glm::vec3 vector)
{
    for (size_t t = 0; t < otherBodies.size(); t++) {
        btRigidBody* body = this->otherBodies[t];
        btTransform transform = body->getWorldTransform();
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        transform.getOpenGLMatrix(glm::value_ptr(modelMatrix));
        modelMatrix = glm::translate(modelMatrix, vector);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0)); // TODO: TEMPORARY HEIGHT ADD
        transform.setFromOpenGLMatrix(glm::value_ptr(modelMatrix));
        body->setWorldTransform(transform);
    }
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