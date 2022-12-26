#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

#include "Model.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Object.hpp"


class Skybox
{
public:
	Skybox();
	Skybox(Camera * camera);
	void render(glm::mat4 V, glm::mat4 P);
	
private:
	void loadCubemapFace(const char* path, const GLenum & targetFace);
	Model cubeMapModel;
	Shader cubeMapShader;
	Object cubeMap;
	GLuint cubeMapTexture;
};

#endif // !SKYBOX_HPP
