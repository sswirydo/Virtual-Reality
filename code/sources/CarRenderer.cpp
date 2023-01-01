#include "../headers/CarRenderer.hpp"

CarRenderer::CarRenderer() {}

CarRenderer::CarRenderer(Model* model, Shader* shader, LightSource* light) 
	: model(model), shader(shader), light(light) 
{}

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
    
    /** Draw top*/
    std::vector<Mesh> carosserie = this->getCarosserieMesh();
    for (unsigned int i = 0; i < carosserie.size(); i++)
    {
        carosserie[i].InstancedDraw(this->shader, modelMatrices);

    }
    /** Draw wheels*/
    std::vector<Mesh> wheels = this->getWheelsMesh();
    for (unsigned int i = 0; i < wheels.size(); i++) 
    {
        wheels[i].InstancedDraw(this->shader, modelMatrices);
    }
    /** Draw The windows*/
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    shader->setFloat("material.transparency", 0.4);
    std::vector<Mesh> windows = this->getWindowsMesh();
    for (unsigned int i = 0; i < windows.size(); i++) 
    {
        windows[i].InstancedDraw(this->shader, modelMatrices);
    }
    glDisable(GL_BLEND);
}




std::vector<Mesh> CarRenderer::getWheelsMesh()
{
    std::vector<Mesh> wheels;
    wheels.push_back(this->model->getMeshes()[7]);
    wheels.push_back(this->model->getMeshes()[6]);
    wheels.push_back(this->model->getMeshes()[5]);
    wheels.push_back(this->model->getMeshes()[4]);
    return wheels;
}

std::vector<Mesh> CarRenderer::getWindowsMesh()
{
    std::vector<Mesh> windows;
    windows.push_back(this->model->getMeshes()[8]);
    windows.push_back(this->model->getMeshes()[9]);
    windows.push_back(this->model->getMeshes()[10]);
    return windows;
}

std::vector<Mesh> CarRenderer::getCarosserieMesh()
{
    std::vector<Mesh> carosserie;
    carosserie.push_back(this->model->getMeshes()[0]);
    carosserie.push_back(this->model->getMeshes()[1]);
    carosserie.push_back(this->model->getMeshes()[2]);
    carosserie.push_back(this->model->getMeshes()[3]);
    carosserie.push_back(this->model->getMeshes()[11]);
    return carosserie;
}