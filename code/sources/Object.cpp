#include "../headers/Object.hpp"

Object::Object() {}

Object::Object(Model &model,Shader &shader, Physics* physics,LightSource *light, bool createRigidBody)
{
    this->light = light;
    this->physics = physics;
    this->setModel(model);
    this->setShader(shader);
    this->modelMatrix = glm::mat4(1.0f);

    if (false) 
    {
        std::cout << "HELLO WORLD " << model.directory << std::endl;
        
        btTriangleMesh* meshInterface = new btTriangleMesh();
        std::vector<Mesh> meshes = model.getMeshes();
        for (size_t i = 0; i < meshes.size(); i++) {
            Mesh mesh = meshes[i];
            std::vector<glm::vec3> positionTriangles = mesh.positionTriangles;
            for (size_t j = 0; j < mesh.positionTriangles.size(); j += 3) {
                glm::vec3 pos1 = mesh.positionTriangles[j];
                glm::vec3 pos2 = mesh.positionTriangles[j+1];
                glm::vec3 pos3 = mesh.positionTriangles[j+2];
                meshInterface->addTriangle(
                    btVector3(pos1.x, pos1.y, pos1.z),
                    btVector3(pos2.x, pos2.y, pos2.z),
                    btVector3(pos3.x, pos3.y, pos3.z)
                );
            }            
        }
        this->collisionShape = new btBvhTriangleMeshShape(meshInterface, false, true);
        btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
        btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(0, motionState, collisionShape, btVector3(0, 0, 0));
        this->rigidBody = new btRigidBody(rigidBodyCI);
        this->rigidBody->setContactProcessingThreshold(0.f);
        physics->getWorld()->addRigidBody(rigidBody);
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

void Object::Draw(){
    this->model.Draw(this->shader);
}

void Object::render(Camera* camera)
{
    this->cameraPos = camera->position;
    glm::mat4 projection = camera->getProjectionMatrix();
    glm::mat4 view = camera->getViewMatrix();
    this->shader.use();
    this->shader.setVec3("lightPos", this->light->getPosition());
    this->shader.setVec4("lightColor", this->light->getColor());
    this->shader.setMat4("projection", projection);
    this->shader.setMat4("view", view);
    this->shader.setMat4("model", this->modelMatrix);
    this->Draw();
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

btRigidBody *Object::getRigidBody()
{
    return this->rigidBody;
}

btCollisionShape* Object::getCollisionShape()
{
    return this->collisionShape;
}
