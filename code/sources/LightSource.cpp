#include "../headers/LightSource.hpp"

LightSource::LightSource(glm::vec3 lightPosition, glm::vec3 lightColor)
{
    this->lightShader = Shader("code/shaders/lightShader.vert","code/shaders/lightShader.frag");
    this->setColor(lightColor);
    this->setPosition(lightPosition);
    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

glm::vec3 LightSource::getPosition()
{
    return this->position;
}

glm::vec4 LightSource::getColor()
{
    return this->color;
}

void LightSource::setPosition(glm::vec3 newPosition)
{
    this->position = newPosition;
}

void LightSource::setColor(glm::vec3 newColor)
{
    this->color = glm::vec4(newColor,0.4f);
}

void LightSource::show(Camera * camera)
{
    glm::mat4 model(1.0f);
    this->lightShader.use();
    this->lightShader.setMat4("projection", camera->GetProjectionMatrix());
    this->lightShader.setMat4("view", camera->GetViewMatrix());
    model = glm::mat4(1.0f);
    model = glm::translate(model, this->position);
    model = glm::scale(model, glm::vec3(.1f)); // a smaller cube
    this->lightShader.setMat4("model", model);
    glBindVertexArray(this->lightCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

LightSource::~LightSource()
{
}
