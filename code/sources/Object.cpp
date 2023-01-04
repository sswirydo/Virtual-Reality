#include "../headers/Object.hpp"
#include "../headers/StreetLamp.hpp"

Object::Object() {}

Object::Object(Model* model,Shader* shader, Physics* physics, LightSource *light)
{
    this->light = light;
    this->physics = physics;
    this->setModel(model);
    this->setShader(shader);
}

Object::~Object() 
{
    if (this->rigidBody != nullptr) 
    {
        this->physics->getWorld()->removeRigidBody(this->getRigidBody());
        delete this->rigidBody;
        this->rigidBody = nullptr;

        if (this->collisionShape != nullptr) {
            delete this->collisionShape;
            this->collisionShape = nullptr;
        }
    }
}


void Object::translateModel(glm::vec3 vector)
{
    this->setModelMatrix(glm::translate(this->modelMatrix, vector)); // warning: use this->modelMatrix and NOT this->getModelMatrix() which is VIRTUAL
}
void Object::rotateModel(float degrees, glm::vec3 axis)
{
    this->setModelMatrix(glm::rotate(this->modelMatrix, glm::radians(degrees), axis));
}
void Object::forceTranslate(glm::vec3 vector)
{
    this->translateModel(vector);
    this->forceUpdatePhysics();
}
void Object::forceRotate(float degrees, glm::vec3 axis)
{
    this->rotateModel(degrees, axis);
    this->forceUpdatePhysics();
}
void Object::forceUpdatePhysics() {
    btTransform transform;
    transform.setFromOpenGLMatrix((btScalar*)glm::value_ptr(this->modelMatrix));
    this->getRigidBody()->setWorldTransform(transform);
}

void Object::translatePhysics(glm::vec3 vector) 
{
    this->getRigidBody()->translate(btVector3(vector.x, vector.y, vector.z));
}
void Object::rotatePhysics(float degrees, glm::vec3 axis) 
{

}

void Object::updateModelFromPhysics() 
{
    btTransform transform = this->getRigidBody()->getWorldTransform();
    glm::mat4 modelMatrix;
    transform.getOpenGLMatrix(glm::value_ptr(modelMatrix));
    this->setModelMatrix(modelMatrix);
}

void Object::setModel(Model* model){
    this->model = model;
}

void Object::setShader(Shader* shader){
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
    this->model->Draw(this->shader);
}

void Object::render(Camera* camera, std::vector<StreetLamp*> lamps, bool enableFog)
{
    this->cameraPos = camera->position;
    glm::mat4 projection = camera->getProjectionMatrix();
    glm::mat4 view = camera->getViewMatrix();
    this->shader->use();
    this->shader->setVec3("sun.direction",((Sun*)this->light)->getDirection());
    this->shader->setVec3("sun.ambient",this->light->getAmbient());
    this->shader->setVec3("sun.diffuse",this->light->getDiffuse());
    this->shader->setVec3("sun.specular",this->light->getSpecular());
    this->shader->setVec4("sun.lightColor", this->light->getColor());
    
    this->shader->setVec3("viewPos", this->cameraPos);
    this->shader->setMat4("projection", projection);
    this->shader->setMat4("view", view);
    this->shader->setMat4("model", this->modelMatrix);
    this->shader->setBool("isNight", ((Sun*)this->light)->isNight());
    this->shader->setBool("enableFog", enableFog);

    for(size_t i = 0; i < lamps.size();i++){
        StreetLamp *lamp = lamps[i];
        LightSource source = lamp->getLightSource();
        // std::cout << glm::cos(glm::radians(35.f))  << std::endl;
        // std::cout << lamps.size() << std::endl;
        this->shader->setVec4(std::string("streetLight[")+std::to_string(i)+std::string("].lightColor"),source.getColor() );
        this->shader->setVec3(std::string("streetLight[")+std::to_string(i)+std::string("].position"), source.getPosition());
        this->shader->setVec3(std::string("streetLight[")+std::to_string(i)+std::string("].direction"), glm::vec3(0,-1,0));
        this->shader->setFloat(std::string("streetLight[")+std::to_string(i)+std::string("].outerCutOff"), glm::cos(glm::radians(35.f)));
        this->shader->setFloat(std::string("streetLight[")+std::to_string(i)+std::string("].cutOff"), glm::cos(glm::radians(25.f)));
        this->shader->setVec3(std::string("streetLight[")+std::to_string(i)+std::string("].ambient"), source.getAmbient());
        this->shader->setVec3(std::string("streetLight[")+std::to_string(i)+std::string("].diffuse"), source.getDiffuse());
        this->shader->setVec3(std::string("streetLight[")+std::to_string(i)+std::string("].specular"), source.getSpecular());
    }
    // set object opaque by default
    shader->setFloat("material.transparency",1.0);
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

void Object::setRigidBody(btRigidBody* rigidBody)
{
    this->rigidBody = rigidBody;
}

btRigidBody* Object::getRigidBody()
{
    return this->rigidBody;
}

btCollisionShape* Object::getCollisionShape()
{
    return this->collisionShape;
}




//if (false) 
//{
//    btTriangleMesh* meshInterface = new btTriangleMesh();
//    std::vector<Mesh> meshes = model.getMeshes();
//    for (size_t i = 0; i < meshes.size(); i++) {
//        Mesh mesh = meshes[i];
//        std::vector<glm::vec3> positionTriangles = mesh.positionTriangles;
//        for (size_t j = 0; j < mesh.positionTriangles.size(); j += 3) {
//            glm::vec3 pos1 = mesh.positionTriangles[j];
//            glm::vec3 pos2 = mesh.positionTriangles[j+1];
//            glm::vec3 pos3 = mesh.positionTriangles[j+2];
//            meshInterface->addTriangle(
//                btVector3(pos1.x, pos1.y, pos1.z),
//                btVector3(pos2.x, pos2.y, pos2.z),
//                btVector3(pos3.x, pos3.y, pos3.z)
//            );
//        }            
//    }
//    this->collisionShape = new btBvhTriangleMeshShape(meshInterface, false, true);
//    btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
//    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(0, motionState, collisionShape, btVector3(0, 0, 0));
//    this->rigidBody = new btRigidBody(rigidBodyCI);
//    this->rigidBody->setContactProcessingThreshold(0.f);
//    physics->getWorld()->addRigidBody(rigidBody);
//} 