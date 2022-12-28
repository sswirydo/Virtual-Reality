#include "../headers/Object.hpp"

Object::Object() {}

Object::Object(Model &model,Shader &shader, Physics* physics, bool createRigidBody)
{
    this->physics = physics;
    this->setModel(model);
    this->setShader(shader);
    this->modelMatrix = glm::mat4(1.0f);

    if (createRigidBody) 
    {
        std::cout << "HELLO WORLD " << model.directory << std::endl;
        
        btTriangleMesh* meshInterface = new btTriangleMesh();
        std::vector<Mesh> meshes = model.getMeshes();
        for (size_t i = 0; i < meshes.size(); i++) {
            Mesh mesh = meshes[i];
            std::vector<unsigned int> indices = mesh.indices;
            std::vector<Vertex> vertices = mesh.vertices;
            std::cout << "VERTICES: " << vertices.size() << std::endl;
            // TODO: make sure vertices.size % 3 == 0
            for (size_t j = 0; j < vertices.size(); j += 3) {
                std::cout << j << ": " << vertices[j].Position.x << " " << vertices[j].Position.y << " " << vertices[j].Position.z << std::endl;
                std::cout << j << ": " << vertices[j+1].Position.x << " " << vertices[j+1].Position.y << " " << vertices[j+1].Position.z << std::endl;
                std::cout << j << ": " << vertices[j+2].Position.x << " " << vertices[j+2].Position.y << " " << vertices[j+2].Position.z << std::endl;
                meshInterface->addTriangle(
                    btVector3(vertices[j].Position.x, vertices[j].Position.y, vertices[j].Position.z),
                    btVector3(vertices[j + 1].Position.x, vertices[j + 1].Position.y, vertices[j + 1].Position.z),
                    btVector3(vertices[j + 2].Position.x, vertices[j + 2].Position.y, vertices[j + 2].Position.z),
                    true); // the last bool value is for removing duplicate vertices -> disable it if you want to specify addTriangleIndices()
            }
            //for (size_t k = 0; k < indices.size(); k += 3) {
            //    meshInterface->addTriangleIndices(indices[k], indices[k + 1], indices[k + 2]); // en vrai sert à R car y a plain de vertex en double/triple/...
            //}
            this->collisionShape = new btBvhTriangleMeshShape(meshInterface, false, true);
            btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
            btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(0, motionState, collisionShape, btVector3(0, 0, 0));
            this->rigidBody = new btRigidBody(rigidBodyCI);
            physics->getWorld()->addRigidBody(rigidBody);
        }
    } 
}

void Object::setModel(Model &model){
    this->model = model;
}

void Object::setShader(Shader &shader){
    this->shader = shader;
}

void Object::setModelMatrix(glm::mat4 model)
{   
    this->modelMatrix = model;
}

glm::mat4 Object::getModelMatrix()
{
    return this->modelMatrix;
}

void Object::render(Camera* camera)
{
    glm::mat4 projection = camera->getProjectionMatrix();
    glm::mat4 view = camera->getViewMatrix();
    this->shader.use();
    this->shader.setMat4("projection", projection);
    this->shader.setMat4("view", view);
    this->shader.setMat4("model", this->modelMatrix);
    this->model.Draw(this->shader);
}

glm::vec3 Object::getWorldCoordinates()
{
    btTransform carTransform = this->getRigidBody()->getWorldTransform();
    btVector3 position = carTransform.getOrigin();
    glm::vec3 glmPosition(position.x(), position.y(), position.z());
    return glmPosition;
}

glm::vec3 Object::getRotation() 
{
    btQuaternion rotation = this->getRigidBody()->getOrientation();
    glm::quat quat(rotation.w(), rotation.x(), rotation.y(), rotation.z());
    glm::vec3 eulerAngles = glm::eulerAngles(quat);
    return eulerAngles; // pitch, yaw, roll
}

btRigidBody* Object::getRigidBody()
{
    return this->rigidBody;
}

btCollisionShape* Object::getCollisionShape()
{
    return this->collisionShape;
}
