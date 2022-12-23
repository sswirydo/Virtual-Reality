
#include <vector>

#include "Mesh.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include "Model.hpp"

#ifndef TERRAIN_H
#define TERRAIN_H


Object generateTerrain()
{
    // TEST
    // ----------------------------------------
    // Adapted from:
    // https://www.dropbox.com/s/47qk4yrz5v9lb61/Terrain%20Generation%20Code.txt?dl=0
    std::vector<Vertex> terrainVertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    int vertex_count = 20;
    int size = 40;
    int count = vertex_count * vertex_count;

    Vertex terrain_vertex;
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 texCoords;

    for (int i = 0; i < vertex_count; i++) {
        for (int j = 0; j < vertex_count; j++) {
            position.x = (float)j / ((float)vertex_count - 1) * size;
            position.y = 0;
            position.z = (float)i / ((float)vertex_count - 1) * size;
            position.x -= (float)size / 2; // translate for center (temp)
            position.z -= (float)size / 2;
            normal.x = 0;
            normal.y = 1;
            normal.z = 0;
            texCoords.x = (float)j / ((float)vertex_count - 1);
            texCoords.y = (float)i / ((float)vertex_count - 1);
            terrain_vertex.Position = position;
            terrain_vertex.Normal = normal;
            terrain_vertex.TexCoords = texCoords;
            terrainVertices.push_back(terrain_vertex);
        }
    }
    for (int gz = 0; gz < vertex_count - 1; gz++) {
        for (int gx = 0; gx < vertex_count - 1; gx++) {
            int topLeft = (gz * vertex_count) + gx;
            int topRight = topLeft + 1;
            int bottomLeft = ((gz + 1) * vertex_count) + gx;
            int bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    Mesh terrainMesh = Mesh(terrainVertices, indices, textures);
    std::vector<Mesh> terrainMeshVector;
    terrainMeshVector.push_back(terrainMesh);
    Model terrainModel = Model(terrainMeshVector);
    Shader terrainShader = Shader("code/shaders/car.vert", "code/shaders/car.frag");
    Object terrain = Object(terrainModel, terrainShader);
    return terrain;
}


#endif