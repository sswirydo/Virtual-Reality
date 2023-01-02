#ifndef TEXT_HPP
#define TEXT_HPP

#include <ft2build.h>
#include FT_FREETYPE_H  


// RESSOURCES
// https://learnopengl.com/In-Practice/Text-Rendering
// https://freetype.org/index.html
// https://gitlab.freedesktop.org/freetype/freetype

class Text
{
public:
	Text() 
	{
		FT_Library ft;
		if (FT_Init_FreeType(&ft))
		{
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
			//return -1;
		}

		FT_Face face;
		if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
		{
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
			//return -1;
		}
	};
	

};

#endif // !TEXT_HPP
