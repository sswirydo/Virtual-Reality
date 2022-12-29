#include "../headers/Car.hpp"

Car::Car(Model &model, Shader &shader, Physics* physics,LightSource &light) : Object(model, shader, physics,light, false) 
{
    // TODO: below is provisory (testing)
    this->collisionShape = new btBoxShape(btVector3(1, 1, 2));
    btDefaultMotionState* carMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 2, 0)));
    btScalar carMass = 1000;
    btVector3 carInertia(0, 0, 0);
    this->collisionShape->calculateLocalInertia(carMass, carInertia);
    btRigidBody::btRigidBodyConstructionInfo carRigidBodyCI(carMass, carMotionState, this->collisionShape, carInertia);
    this->rigidBody = new btRigidBody(carRigidBodyCI);

    physics->getWorld()->addRigidBody(this->rigidBody);
}

void Car::move(float deltaTime, glm::vec4 direction) 
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

    transform = this->getRigidBody()->getWorldTransform();
    // Create a GLM model matrix from the world transform
    
    glm::mat4 modelMatrix;
    transform.getOpenGLMatrix(glm::value_ptr(modelMatrix));
    this->setModelMatrix(modelMatrix);
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
    wheels.push_back(this->model.getMeshes()[7]);
    wheels.push_back(this->model.getMeshes()[6]);
    wheels.push_back(this->model.getMeshes()[5]);
    wheels.push_back(this->model.getMeshes()[4]);
    return wheels;
}

std::vector<Mesh> Car::getWindowsMesh()
{
    std::vector<Mesh> windows;
    windows.push_back(this->model.getMeshes()[8]);
    windows.push_back(this->model.getMeshes()[9]);
    windows.push_back(this->model.getMeshes()[10]);
    return windows;
}

std::vector<Mesh> Car::getCarosserieMesh()
{
    std::vector<Mesh> carosserie;
    carosserie.push_back(this->model.getMeshes()[0]);
    carosserie.push_back(this->model.getMeshes()[1]);
    carosserie.push_back(this->model.getMeshes()[2]);
    carosserie.push_back(this->model.getMeshes()[3]);
    carosserie.push_back(this->model.getMeshes()[11]);
    return carosserie;
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

void Car::renderShapeBox(Camera* camera, Shader &shader)
{
    const btBoxShape* boxShape = static_cast<const btBoxShape*>(this->getCollisionShape());
    btVector3 halfExtents = boxShape->getHalfExtentsWithMargin();
    std::vector<glm::vec3> positions;
    positions.emplace_back(halfExtents.x(), halfExtents.y(), halfExtents.z());       // 0
    positions.emplace_back(-halfExtents.x(), halfExtents.y(), halfExtents.z());      // 1
    positions.emplace_back(halfExtents.x(), -halfExtents.y(), halfExtents.z());      // 2
    positions.emplace_back(-halfExtents.x(), -halfExtents.y(), halfExtents.z());     // 3
    positions.emplace_back(halfExtents.x(), halfExtents.y(), -halfExtents.z());      // 4
    positions.emplace_back(-halfExtents.x(), halfExtents.y(), -halfExtents.z());     // 5
    positions.emplace_back(halfExtents.x(), -halfExtents.y(), -halfExtents.z());     // 6
    positions.emplace_back(-halfExtents.x(), -halfExtents.y(), -halfExtents.z());    // 7
    std::vector<unsigned int> indices;
    indices.push_back(0); indices.push_back(1);
    indices.push_back(1); indices.push_back(3);
    indices.push_back(3); indices.push_back(2);
    indices.push_back(2); indices.push_back(0);
    indices.push_back(0); indices.push_back(4);
    indices.push_back(1); indices.push_back(5);
    indices.push_back(3); indices.push_back(7);
    indices.push_back(2); indices.push_back(6);
    indices.push_back(4); indices.push_back(5);
    indices.push_back(5); indices.push_back(7);
    indices.push_back(7); indices.push_back(6);
    indices.push_back(6); indices.push_back(4);
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &positions[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    shader.use();
    shader.setMat4("projection", camera->getProjectionMatrix());
    shader.setMat4("view", camera->getViewMatrix());
    shader.setMat4("model", modelMatrix);
    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
