
#ifndef MESH_H
#define MESH_H

#include<vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.hpp"
#define MAX_BONE_INFLUENCE 4

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
	//bone indexes which will influence this vertex
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	//weights from each bone
	float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};


struct Material {
    glm::vec3 Diffuse;      // diffuse color        // AI_MATKEY_COLOR_DIFFUSE
    glm::vec3 Specular;     // specular color       // AI_MATKEY_COLOR_SPECULAR
    glm::vec3 Ambient;      // ambient color        // AI_MATKEY_COLOR_AMBIENT
    glm::vec3 Emissive;     // emission color       // AI_MATKEY_COLOR_EMISSIVE
    float Shininess;        // specular exponent    // AI_MATKEY_SHININESS          
    float RefractI;         // index of refraction  // AI_MATKEY_REFRACTI
    float Reflectivity;     // reflection factor    // AI_MATKEY_REFLECTIVITY
    glm::vec3 Transparency; //transparency         // AI_MATKEY_COLOR_TRANSPARENT
    float TransparencyFactor; // transparencyFactor // AI_MATKEY_TRANSPARENCYFACTOR
    glm::vec3 Reflective;   // reflection color     // AI_MATKEY_COLOR_REFLECTIVE
};

class Mesh {
    public:
        // mesh data
        std::vector<Vertex>         vertices;
        std::vector<unsigned int>   indices;
        std::vector<Texture>        textures;
        Material                    material;

        std::vector<glm::vec3>      positionTriangles;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures,Material material);
        ~Mesh();

        void Draw(Shader* shader);
        void InstancedDraw(Shader* shader, std::vector<glm::vec3> translations);
        void InstancedDraw(Shader* shader, std::vector<glm::mat4> modelMatrices);

        void printMaterial();
    protected:
        void setUpMaterials(Shader* shader);
    private:
        //  render data
        unsigned int VAO, VBO, EBO, instanceVBO;
        bool instancedSetUp = false;
        
        void setupMesh();
        void createTriangles();
    
};

#endif