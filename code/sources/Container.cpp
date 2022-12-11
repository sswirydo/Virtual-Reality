#include "../headers/Container.hpp"
#include "../headers/Vertices.hpp"


Container::Container(std::string vertexShader, std::string fragmentShader) : Object(vertexShader,fragmentShader) 
{
    this->setVertices(get_container_vertices());
    this->setTexture("assets/textures/container.jpg"); 
    this->shader.use();
    this->shader.setInt("texture1", 0); 
}

void Container::render(){
     // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture.getId());

    // activate shader
    this->shader.use();

    // create transformations
    glm::mat4 view          = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    view       = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection    = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
    // pass transformation matrices to the shader
    this->shader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    this->shader.setMat4("view", view);

    // render boxes
    glBindVertexArray(this->VAO);

    //calculate the model matrix for each object and pass it to shader before drawing
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3( 0.0f,  0.0f,  0.0f) );
    float angle = 20.0f * glfwGetTime();
    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    this->shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

}

Container::~Container() {}