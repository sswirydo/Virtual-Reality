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
#include "Camera.hpp"


class Skybox
{
public:
	Skybox(Camera* camera);
	void render();
	
private:
	void loadCubemapFace(const char* path, const GLenum & targetFace);
	Model cubeMapModel;
	Shader cubeMapShader;
	Object cubeMap;
	GLuint cubeMapTexture;
	Camera* camera;
};

#endif // !SKYBOX_HPP
