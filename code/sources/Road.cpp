#include "../headers/Road.hpp"

Road::Road(Model* model, Shader* shader, Physics* physics, LightSource* light) : Object(model, shader, physics, light) 
{
    srand(time(nullptr));

    //Creates the ground shape
    btCollisionShape* groundShape = new btBoxShape(btVector3(107, 0, 100)); // TODO: set Y to 0 or 1?
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
    this->physics->addBody(this->rigidBody, ROAD);
    this->physics->addBody(rigidBody_l, BARRIER);
    this->physics->addBody(rigidBody_r, BARRIER);
}

// Note: Road rigidbody should be destroyed in the Object.
Road::~Road() 
{
    for (size_t t = 0; t < otherBodies.size(); t++)
    {
        if (otherBodies[t] != nullptr) 
        {
            this->physics->getWorld()->removeRigidBody(otherBodies[t]);
            delete otherBodies[t];
            otherBodies[t] = nullptr;
        }
        
    }
    for (size_t t = 0; t < linkedObjects.size(); t++)
    {
        if (linkedObjects[t] != nullptr) 
        {
            delete linkedObjects[t];
            linkedObjects[t] = nullptr;
        }
    }
    for (size_t t = 0; t < cars.size(); t++)
    {
        if (cars[t] != nullptr) 
        {
            delete cars[t];
            cars[t] = nullptr;
        }
    }
}



void Road::move(int nbOfRoads, int pos, bool spawnCars)
{
    if (pos == 0) { return; }

    glm::vec3 vector;
    if (pos == -1) {
        vector = glm::vec3(0.f, 0.f, -200 * nbOfRoads);
    }
    else { // init
        vector = glm::vec3(0, 0, -200 * pos);
    }

    this->checkCarsState();
    this->forceTranslate(vector);

    btTransform transf = this->getRigidBody()->getWorldTransform();
    btVector3 btPos = transf.getOrigin();
    glm::vec3 vectorFromOrigin(btPos.getX(), btPos.getY(), btPos.getZ());

    if (spawnCars) {
        this->addNewCars(vectorFromOrigin);
    }
    this->translateBarriers(vector);
    this->moveLinkedObjects(vector);
}


void Road::moveLinkedObjects(glm::vec3 vector) // NOTE: simple objects without rigidbodies
{
    for (size_t t = 0; t < linkedObjects.size(); t++) {
        Object* obj = linkedObjects[t];
        obj->translateModel(vector);
    }
}

void Road::addCarInfo(Model* model, Shader* shader, LightSource* light) {
    this->carModel = model;
    this->carShader = shader;
    this->carLight = light;
}
void Road::addTreeInfo(Model* model, Shader* shader, LightSource* light) {
    this->treeModel = model;
    this->treeShader = shader;
    this->treeLight = light;
}
void Road::addLampInfo(Model* model, Model* reversedModel, Shader* shader, LightSource* light) {
    this->lampModel = model;
    this->lampReversedModel = reversedModel;
    this->lampShader = shader;
    this->lampLight = light;
}



constexpr int NUMBER_OF_CARS = 4;
void Road::addNewCars(glm::vec3 currentRoadTranslate)
{
    int min = 0;
    int max = 3;
    int range = max - min + 1;
    int value = rand() % range + min;

    for (int i = 0; i < NUMBER_OF_CARS; i++) {

        if (i == value) continue;

        Car* car = new Car(this->carModel, this->carShader, this->physics, this->carLight);
        glm::vec3 carVector = glm::vec3(0, 1, 75) + currentRoadTranslate;

        if (i % NUMBER_OF_CARS == 0) { carVector = carVector + glm::vec3(-5.25, 0, 0); }
        else if (i % NUMBER_OF_CARS == 1) { carVector = carVector + glm::vec3(5.25, 0, 0); }
        else if (i % NUMBER_OF_CARS == 2) { carVector = carVector + glm::vec3(-1.75, 0, 0); }
        else if (i % NUMBER_OF_CARS == 3) { carVector = carVector + glm::vec3(1.75, 0, 0); }

        car->translatePhysics(carVector);
        cars.push_back(car);
    }
}

// Checks if cars can be considered out of the game and if yes deletes them.
void Road::checkCarsState()
{ 
    for (size_t t = 0; t < this->cars.size(); t++) {
        Car* car = this->cars[t];
        if (car->getWorldCoordinates().y < -1) {
            delete car;
            this->cars.at(t) = nullptr;
        }
    }
    this->cars.erase( remove(this->cars.begin(), this->cars.end(), nullptr ), this->cars.end() );
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
std::vector<Car*> Road::getCars() {
    return this->cars;
}

constexpr int NUMBER_OF_LAMPS = 5;
void Road::generateLamps() {
    for (int i = 0; i < NUMBER_OF_LAMPS; i++) {
        Object* lamp = new StreetLamp(lampModel, lampShader, this->physics, lampLight, false, i);
        linkedObjects.push_back(lamp);
        lamps.push_back((StreetLamp*)lamp);
    }
    for (int i = 0; i < NUMBER_OF_LAMPS; i++) {
        Object* lamp = new StreetLamp(lampReversedModel, lampShader, this->physics, lampLight, true, i);
        linkedObjects.push_back(lamp);
        lamps.push_back((StreetLamp*)lamp);
    }
}

const int NUMBER_OF_TREES = 2000; // per road segment
void Road::generateTrees() {
    std::vector<glm::vec3> translations;
    for (int i = 0; i < NUMBER_OF_TREES; i++) {
        int minZ = 0; int maxZ = 99;
        int minX = 9; int maxX = 99;
        int rangeZ = maxZ - minZ + 1;
        int rangeX = maxX - minX + 1;
        int numZ = rand() % rangeZ + minZ;
        int numX = rand() % rangeX + minX;

        if (i % 4 == 0) { numZ = -numZ; }
        if (i % 4 == 1) { numX = -numX; }
        if (i % 4 == 2) { numZ = -numZ; numX = -numX; }
        //glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(numX, 0, numZ));
        glm::vec3 translation = glm::vec3(numX, 0, numZ);
        translations.push_back(translation);
    }
    InstancedObject* tree = new InstancedObject(treeModel, treeShader, this->physics, treeLight, translations);
    this->linkObject(tree);
}


void Road::translateBarriers(glm::vec3 vector)
{
    for (size_t t = 0; t < otherBodies.size(); t++) {
        btRigidBody* body = this->otherBodies[t];
        body->translate(btVector3(vector.x, vector.y, vector.z));
    }
}

btRigidBody* Road::createGroundRigidBodyFromShape(btCollisionShape* groundShape)
{
    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(0, 0, 0));

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

std::vector<StreetLamp *> Road::getLamps()
{
    return this->lamps;
}