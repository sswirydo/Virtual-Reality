#include "../headers/Object.hpp"

Object::Object(std::string vertexShader, std::string fragmentShader)
{
    this->shader = Shader(vertexShader.c_str(), fragmentShader.c_str());
}

Object::Object()
{

}

std::vector<float> Object::getVertices()
{
    return vertices;
}

void Object::setVertices(std::vector<float> vertices)
{
    this->vertices = vertices;
}

Shader Object::getShader(){
    return this->shader;
}

void Object::setShader(Shader shader ){
    this->shader = shader;
}

Texture Object::getTexture(){
    return this->texture;
}

void Object::setTexture(Texture texture ){
    this->texture = texture;
}

void Object::setTexture(std::string path){
    this->texture = Texture(path.c_str());
}

std::vector<int> Object::getIndices()
{
    return indices;
}

void Object::setIndices(std::vector<int> Indices)
{
    this->indices = Indices;
}


void Object::render(){

}

void Object::configure_VAO_VBO(){
    glGenVertexArrays(1, &(this->VAO));
    glGenBuffers(1, &(this->VBO));
    glBindVertexArray((this->VAO));
    glBindBuffer(GL_ARRAY_BUFFER, (this->VBO));
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*(this->vertices).size(), (this->vertices).data(), GL_STATIC_DRAW);
}

void Object::set_vertex_attributes_pointers(int layoutLocation, int vectSize, int stride, int offset){
    glVertexAttribPointer(layoutLocation, vectSize, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
    glEnableVertexAttribArray(layoutLocation);
}

Object::~Object()
{
    glDeleteVertexArrays(1, &(this->VAO));
    glDeleteBuffers(1, &(this->VBO));
}