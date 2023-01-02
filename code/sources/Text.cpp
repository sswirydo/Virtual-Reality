#include "../headers/Text.hpp"


Text::Text(Font* font, glm::vec2 startPosition)
	: font(font), startPosition(startPosition)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}

Text::~Text()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Text::Update(std::string txt)
{
	charVertices = std::vector<CharVertex>();
	glm::vec2 tempPos = startPosition;
	for (auto& character : txt)
	{
		auto glyph = this->font->getGlyph(character, tempPos);

		CharVertex character = CharVertex();
		character.Position = glyph.bbmin;
		character.UV = glyph.uvmin;
		charVertices.push_back(character);

		character = CharVertex();
		character.Position = glm::vec2(glyph.bbmax.x, glyph.bbmin.y);
		character.UV = glm::vec2(glyph.uvmax.x, glyph.uvmin.y);
		charVertices.push_back(character);

		character = CharVertex();
		character.Position = glyph.bbmax;
		character.UV = glyph.uvmax;
		charVertices.push_back(character);

		character = CharVertex();
		character.Position = glyph.bbmin;
		character.UV = glyph.uvmin;
		charVertices.push_back(character);

		character = CharVertex();
		character.Position = glyph.bbmax;
		character.UV = glyph.uvmax;
		charVertices.push_back(character);

		character = CharVertex();
		character.Position = glm::vec2(glyph.bbmin.x, glyph.bbmax.y);
		character.UV = glm::vec2(glyph.uvmin.x, glyph.uvmax.y);
		charVertices.push_back(character);
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, charVertices.size() * sizeof(CharVertex), &charVertices[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(CharVertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CharVertex), (void*)offsetof(CharVertex, UV));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Text::Draw(Shader* fontShader)
{
	fontShader->use();
	fontShader->setMat4("projection", glm::ortho(0.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT, 0.0f, 0.0f, 1.0f));
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // <-- makes text show as texts and not rectangles (transparency)
	glBindTexture(GL_TEXTURE_2D, font->Atlas);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)charVertices.size());
	glDisable(GL_BLEND);
}