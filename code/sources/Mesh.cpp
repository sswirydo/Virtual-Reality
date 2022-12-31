#include "../headers/Mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, Material material)
{
    this->vertices = vertices;
    this->material = material;
    this->indices = indices;
    this->textures = textures;
    setupMesh();
    createTriangles();
    printMaterial();
}

void Mesh::createTriangles() 
{
    for (int i = 0; i < indices.size(); i += 3) {
        Vertex v1 = vertices[indices[i]];
        Vertex v2 = vertices[indices[i + 1]];
        Vertex v3 = vertices[indices[i + 2]];
        glm::vec3 pos1 = glm::vec3(v1.Position.x, v1.Position.y, v1.Position.z);
        glm::vec3 pos2 = glm::vec3(v2.Position.x, v2.Position.y, v2.Position.z);
        glm::vec3 pos3 = glm::vec3(v3.Position.x, v3.Position.y, v3.Position.z);
        positionTriangles.push_back(pos1);
        positionTriangles.push_back(pos2);
        positionTriangles.push_back(pos3);
    }

}

void Mesh::printMaterial() 
{
    std::cout << "diffuse" << material.Diffuse.x << " " << material.Diffuse.y << " " << material.Diffuse.z << std::endl;
    std::cout << "specular" << material.Specular.x << " " << material.Specular.y << " " << material.Specular.z << std::endl;
    std::cout << "ambient" << material.Ambient.x << " " << material.Ambient.y << " " << material.Ambient.z << std::endl;
    std::cout << "emissive" << material.Emissive.x << " " << material.Emissive.y << " " << material.Emissive.z << std::endl;
    std::cout << "transparency" << material.Transparency.x << " " << material.Transparency.y << " " << material.Transparency.z << std::endl;
    std::cout << "reflective" << material.Reflective.x << " " << material.Reflective.y << " " << material.Reflective.z << std::endl;
    std::cout << "Shininess" << material.Shininess << std::endl;
    std::cout << "RefractI" << material.RefractI << std::endl;
    std::cout << "Reflectivity" << material.Reflectivity << std::endl;
    std::cout << "Transparency Factor" << material.TransparencyFactor << std::endl;
}


void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
} 

void Mesh::setUpMaterials(Shader* shader) 
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    // std::cout << "size of textures vector " << textures.size() << std::endl;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;

        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);

        // std::cout << ("material." + name + number).c_str() << std::endl;
        // shader.setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    // send material
    shader->setVec3("material.diffuse", this->material.Diffuse);
    shader->setVec3("material.ambient", this->material.Ambient);
    shader->setVec3("material.specular", this->material.Specular);
    shader->setFloat("material.shininess", this->material.Shininess);
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::Draw(Shader* shader) 
{
    this->setUpMaterials(shader);
    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
} 


void Mesh::InstancedDraw(Shader* shader, std::vector<glm::vec3> translations)
{
    if (!instancedSetUp) 
    {
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * translations.size(), &translations[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(VAO);
        glEnableVertexAttribArray(3);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glVertexAttribDivisor(3, 1); // setting to 1 means "we want to update the content of the vertex attribute when we start to render a new instance"
        glBindVertexArray(0);
        instancedSetUp = true;
    }
    
    this->setUpMaterials(shader);

    glBindVertexArray(VAO);
    glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0, (int)translations.size());
    glBindVertexArray(0);
}