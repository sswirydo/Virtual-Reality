#ifndef TEXT_HPP
#define TEXT_HPP

#include "Font.hpp"
#include "Shader.hpp"
#include "Settings.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_truetype.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

// RESSOURCES: 
// How does Font Rendering Work?
// https://youtu.be/MvNqNYLhXkY
// How to draw text opengl c++ ?
// https://youtu.be/7Coea1c_bnA


struct Glyph;
class Font;

struct CharVertex {
	glm::vec2 Position;
	glm::vec2 UV;
};


class Text
{
public:
	Text(Font* font, glm::vec2 startPosition);
	~Text();
	void Update(std::string txt);
	void Draw(Shader* fontShader, bool enableColor = false);
	glm::vec2 startPosition;
protected:
	Font* font;
	std::vector<CharVertex> charVertices;
	unsigned int VAO;
	unsigned int VBO;
};

#endif //! TEXT_HPP