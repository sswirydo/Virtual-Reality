#include "../headers/CarRenderer.hpp"

CarRenderer::CarRenderer() {}

CarRenderer::CarRenderer(Model* model, Shader* shader, LightSource* light) 
	: model(model), shader(shader), light(light) 
{
    this->getWheelsMesh();
    this->getWindowsMesh();
    this->getCarosserieMesh();
}

void CarRenderer::render(std::vector<glm::mat4> modelMatrices, Camera* camera, std::vector<StreetLamp*> lamps) {
    glm::vec3 cameraPos = camera->position;
    glm::mat4 projection = camera->getProjectionMatrix();
    glm::mat4 view = camera->getViewMatrix();

    this->shader->use();
    this->shader->setVec3("sun.direction", ((Sun*)this->light)->getDirection());
    this->shader->setVec3("sun.ambient", this->light->getAmbient());
    this->shader->setVec3("sun.diffuse", this->light->getDiffuse());
    this->shader->setVec3("sun.specular", this->light->getSpecular());
    this->shader->setVec4("sun.lightColor", this->light->getColor());

    this->shader->setVec3("viewPos", cameraPos);
    this->shader->setMat4("projection", projection);
    this->shader->setMat4("view", view);

    //this->shader->setMat4("model", this->modelMatrix); // <--- nope

    this->shader->setBool("isNight", ((Sun*)this->light)->isNight());
    for (size_t i = 0; i < lamps.size(); i++) {
        StreetLamp* lamp = lamps[i];
        LightSource source = lamp->getLightSource();
        this->shader->setVec4(std::string("streetLight[") + std::to_string(i) + std::string("].lightColor"), source.getColor());
        this->shader->setVec3(std::string("streetLight[") + std::to_string(i) + std::string("].position"), source.getPosition());
        this->shader->setVec3(std::string("streetLight[") + std::to_string(i) + std::string("].direction"), glm::vec3(0, -1, 0));
        this->shader->setFloat(std::string("streetLight[") + std::to_string(i) + std::string("].cutOff"), glm::cos(glm::radians(36.f)));
        this->shader->setVec3(std::string("streetLight[") + std::to_string(i) + std::string("].ambient"), source.getAmbient());
        this->shader->setVec3(std::string("streetLight[") + std::to_string(i) + std::string("].diffuse"), source.getDiffuse());
        this->shader->setVec3(std::string("streetLight[") + std::to_string(i) + std::string("].specular"), source.getSpecular());
    }
    // set object opaque by default
    shader->setFloat("material.transparency", 1.0);

    std::cout << this->carosserie.size() << std::endl;
        
    for (size_t i = 0; i < this->carosserie.size(); i++) { this->carosserie[i].InstancedDraw(this->shader, modelMatrices); }
    for (size_t i = 0; i < this->wheels.size(); i++) { this->wheels[i].InstancedDraw(this->shader, modelMatrices); }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    shader->setFloat("material.transparency", 0.4);
    for (size_t i = 0; i < this->windows.size(); i++) { this->windows[i].InstancedDraw(this->shader, modelMatrices); }
    glDisable(GL_BLEND);
}


void CarRenderer::getWheelsMesh()
{
    this->wheels.push_back(this->model->getMeshes()[7]);
    this->wheels.push_back(this->model->getMeshes()[6]);
    this->wheels.push_back(this->model->getMeshes()[5]);
    this->wheels.push_back(this->model->getMeshes()[4]);
}

void CarRenderer::getWindowsMesh()
{
    this->windows.push_back(this->model->getMeshes()[8]);
    this->windows.push_back(this->model->getMeshes()[9]);
    this->windows.push_back(this->model->getMeshes()[10]);
}

void CarRenderer::getCarosserieMesh()
{
    this->carosserie.push_back(this->model->getMeshes()[0]);
    this->carosserie.push_back(this->model->getMeshes()[1]);
    this->carosserie.push_back(this->model->getMeshes()[2]);
    this->carosserie.push_back(this->model->getMeshes()[3]);
    this->carosserie.push_back(this->model->getMeshes()[11]);
}