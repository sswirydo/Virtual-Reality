#include "../headers/Car.hpp"

Car::Car(Model* model, Shader* shader, Physics* physics, LightSource* light) : Object(model, shader, physics,light)
{
    // TODO: below is provisory (testing)
    this->collisionShape = new btBoxShape(btVector3(1, 1, (btScalar)1.8));
    btDefaultMotionState* carMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 1, 0)));
    btScalar carMass = 1000;
    btVector3 carInertia(0, 0, -10);
    this->collisionShape->calculateLocalInertia(carMass, carInertia);
    btRigidBody::btRigidBodyConstructionInfo carRigidBodyCI(carMass, carMotionState, this->collisionShape, carInertia);
    this->rigidBody = new btRigidBody(carRigidBodyCI);

    //this->rigidBody->setFriction(0.9);
    this->rigidBody->setAngularFactor(btVector3(1, 0, 1)); // disables Y-axis rotation

    physics->addBody(rigidBody, CAR);

    // small initial motions
    btVector3 velocity = btVector3(0, 0, -10);
    this->getRigidBody()->setLinearVelocity(velocity);
    
}

void Car::move(float deltaTime) 
{   
    btVector3 velocity = btVector3(0, 0, -15);
    // this->getRigidBody()->setLinearVelocity(velocity);
    btTransform transform;
    btVector3 position;
    /*this->getRigidBody()->setLinearVelocity(velocity);*/
    //Update the car's position based on the velocity
    transform = this->getRigidBody()->getWorldTransform();
    position = transform.getOrigin();
    position += velocity * deltaTime;
    transform.setOrigin(position);
    this->getRigidBody()->setWorldTransform(transform);
    this->getRigidBody()->activate(true);
}





void Car::Draw()
{
    /** Draw la carosserie*/
    std::vector<Mesh> carosserie = this->getCarosserieMesh();
    for(unsigned int i = 0; i < carosserie.size(); i++)
        carosserie[i].Draw(this->shader);

    /** Draw wheels*/
    std::vector<Mesh> wheels = this->getWheelsMesh();
    for(unsigned int i = 0; i < wheels.size(); i++)
        wheels[i].Draw(this->shader);

    /** Draw The windows*/
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    std::vector<Mesh> windows = this->getWindowsMesh();
    for(unsigned int i = 0; i < windows.size(); i++)
        windows[i].Draw(this->shader);
    //
    // Uncomment this if you want to load in order
    //
    // std::map<float, Mesh*> sorted;
    // for (unsigned int i = 0; i < windows.size(); i++)
    // {
    //     float distance = glm::distance(this->cameraPos, windows[i].positionTriangles[0]);
    //     sorted[distance] = &windows[i];
    // }
    // for(std::map<float,Mesh*>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) 
    //     it->second->Draw(this->shader);
    glDisable(GL_BLEND);

}

std::vector<Mesh> Car::getWheelsMesh()
{
    std::vector<Mesh> wheels;
    wheels.push_back(this->model->getMeshes()[7]);
    wheels.push_back(this->model->getMeshes()[6]);
    wheels.push_back(this->model->getMeshes()[5]);
    wheels.push_back(this->model->getMeshes()[4]);
    return wheels;
}

std::vector<Mesh> Car::getWindowsMesh()
{
    std::vector<Mesh> windows;
    windows.push_back(this->model->getMeshes()[8]);
    windows.push_back(this->model->getMeshes()[9]);
    windows.push_back(this->model->getMeshes()[10]);
    return windows;
}

std::vector<Mesh> Car::getCarosserieMesh()
{
    std::vector<Mesh> carosserie;
    carosserie.push_back(this->model->getMeshes()[0]);
    carosserie.push_back(this->model->getMeshes()[1]);
    carosserie.push_back(this->model->getMeshes()[2]);
    carosserie.push_back(this->model->getMeshes()[3]);
    carosserie.push_back(this->model->getMeshes()[11]);
    return carosserie;
}

void Car::render(Camera* camera) {
    this->updateModelFromPhysics();
    this->translateModel(glm::vec3(0, -1, 0));
    this->rotateModel(180.0f, glm::vec3(0, 1, 0));
    this->Object::render(camera);
}
 
// void Car::render(Camera* camera, LightSource &light)
// {
//     glm::mat4 projection = camera->getProjectionMatrix();
//     glm::mat4 view = camera->getViewMatrix();

//     this->shader.use();
//     this->shader.setVec3("lightPos", light.getPosition());
//     this->shader.setVec4("lightColor", light.getColor());
//     this->shader.setVec3("viewPos", camera->position); 
//     this->shader.setMat4("projection", projection);
//     this->shader.setMat4("view", view);
//     this->shader.setMat4("model", this->modelMatrix);
//     this->Draw();
// }
