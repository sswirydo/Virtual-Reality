#include "../headers/Font.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h> 

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>


Font::Font(const char* fontPath)
{
	unsigned char* bitmap = new unsigned char[width * height];

	FILE* fontFile = nullptr;
	fopen_s(&fontFile, fontPath, "rb");
	if (!fontFile) {
		std::cout << "ERROR OPENING FONT FILE" << std::endl;
		exit(1);
	}
	fseek(fontFile, 0, SEEK_END);
	long fileSize = ftell(fontFile);
	unsigned char* data = new unsigned char[fileSize];
	rewind(fontFile);
	fread(data, 1, fileSize, fontFile);

	chardata = new stbtt_bakedchar[96];
	stbtt_BakeFontBitmap(data, 0, 64, bitmap, width, height, 32, 96, chardata);
	delete[] data;

	glGenTextures(1, &Atlas);
	glBindTexture(GL_TEXTURE_2D, Atlas);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_write_png("TextFontBitmap.png", width, height, 1, bitmap, 0); // <-- for testing/visualizing the bitmap font
	delete[] bitmap;

}

Font::~Font()
{
	glDeleteTextures(1, &Atlas);
	free(chardata);
}

Glyph Font::getGlyph(char c, glm::vec2& txtPos)
{
	Glyph glyph = Glyph();
	// characters in range [32 --- 128] ascii check
	if (c >= 32 && c < 128)
	{
		stbtt_aligned_quad q;
		stbtt_GetBakedQuad(chardata, width, height, c - 32, &txtPos.x, &txtPos.y, &q, 1);
		glyph.bbmin = glm::vec2(q.x0, q.y0);
		glyph.bbmax = glm::vec2(q.x1, q.y1);
		glyph.uvmin = glm::vec2(q.s0, q.t0);
		glyph.uvmax = glm::vec2(q.s1, q.t1);
	}
	return glyph;
}
