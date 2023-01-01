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
	void getWheelsMesh();
	void getWindowsMesh();
	void getCarosserieMesh();

	Model* model = nullptr;
	Shader* shader = nullptr;
	LightSource* light = nullptr;

	std::vector<Mesh> wheels;
	std::vector<Mesh> carosserie;
	std::vector<Mesh> windows;
};

#endif // !CAR_RENDERER_HPP
