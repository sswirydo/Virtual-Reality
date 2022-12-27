#include "../headers/Car.hpp"

Car::Car(Model model, Shader &shader, Camera * camera, Physics* physics) : Object(model, shader, camera, physics) 
{
    // TODO: below is provisory (testing)
    carShape = new btBoxShape(btVector3(1, 1, 2));
    btDefaultMotionState* carMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
    btScalar carMass = 1000;
    btVector3 carInertia(0, 0, 0);
    carShape->calculateLocalInertia(carMass, carInertia);
    btRigidBody::btRigidBodyConstructionInfo carRigidBodyCI(carMass, carMotionState, carShape, carInertia);
    carBody = new btRigidBody(carRigidBodyCI);

    physics->getWorld()->addRigidBody(carBody);
}

void Car::move() 
{
    //btVector3 acceleration(0, 0, 0);
    //acceleration += btVector3(0, 0, -1); // forward movement test
    //// Update the car's velocity based on the acceleration
    //btVector3 velocity = car.getCarBody()->getLinearVelocity();
    //velocity += acceleration * deltaTime; // deltaTime is the time elapsed since the last frame
    //car.getCarBody()->setLinearVelocity(velocity);

    // Update the car's position based on the velocity
    //btTransform transform = car.getCarBody()->getWorldTransform();
    //btVector3 position = transform.getOrigin();
    //position += velocity * deltaTime;
    //transform.setOrigin(position);
    //car.getCarBody()->setWorldTransform(transform);

    btTransform transform = this->getCarBody()->getWorldTransform();
    // Create a GLM model matrix from the world transform
    glm::mat4 modelMatrix;
    transform.getOpenGLMatrix(glm::value_ptr(modelMatrix));
    this->setModelMatrix(modelMatrix);
}

btRigidBody* Car::getCarBody() 
{
    return carBody;
}

btCollisionShape* Car::getCarShape() 
{
    return carShape;
}

void Car::render(LightSource &light)
{
    glm::mat4 projection = camera->GetProjectionMatrix();
    glm::mat4 view = camera->GetViewMatrix();

    this->shader.use();
    this->shader.setVec3("lightPos", light.getPosition());
    this->shader.setVec4("lightColor", light.getColor());
    this->shader.setVec3("viewPos", this->camera->Position); 
    this->shader.setMat4("projection", projection);
    this->shader.setMat4("view", view);
    this->shader.setMat4("model", this->modelMatrix);
    this->model.Draw(this->shader);
}

void Car::renderShapeBox(Shader &shader)
{
    const btBoxShape* boxShape = static_cast<const btBoxShape*>(this->getCarShape());
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
    shader.setMat4("projection", this->camera->GetProjectionMatrix());
    shader.setMat4("view", this->camera->GetViewMatrix());
    shader.setMat4("model", modelMatrix);
    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
