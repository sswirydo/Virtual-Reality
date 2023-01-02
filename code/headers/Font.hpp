#ifndef FONT_HPP
#define FONT_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <stb_truetype.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

struct Glyph
{
	glm::vec2 bbmin;
	glm::vec2 bbmax;
	glm::vec2 uvmin;
	glm::vec2 uvmax;
};

class Font
{
public:
	Font(const char* fontPath);
	~Font();

	Glyph getGlyph(char c, glm::vec2& txtPos);
	int width = 512;
	int height = 512;
	unsigned int Atlas;
	stbtt_bakedchar* chardata = nullptr;
};

#endif //! FONT_HPP