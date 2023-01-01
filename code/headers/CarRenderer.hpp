#ifndef CAR_RENDERER_HPP
#define CAR_RENDERER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include "Sun.hpp"
#include "StreetLamp.hpp"

#include <string>
#include <iostream>
#include <vector>

class CarRenderer
{
public:
	CarRenderer();
	CarRenderer(Model* model, Shader* shader, LightSource* light);
	void render(std::vector<glm::mat4> modelMatrices, Camera* camera, std::vector<StreetLamp*> lamps);
protected:
	std::vector<Mesh> getWheelsMesh();
	std::vector<Mesh> getWindowsMesh();
	std::vector<Mesh> getCarosserieMesh();

	Model* model = nullptr;
	Shader* shader = nullptr;
	LightSource* light = nullptr;
	
};

#endif // !CAR_RENDERER_HPP
