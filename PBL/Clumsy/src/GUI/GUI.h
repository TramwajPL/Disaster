#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H 

#include <map>
#include <iostream>
#include <glm/glm.hpp>

#include "../RenderEngine/Shader.h"

namespace Clumsy {

	struct Character 
	{
		unsigned int TextureID;  // ID handle of the glyph texture
		glm::ivec2   Size;       // Size of glyph
		glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
		unsigned int Advance;    // Offset to advance to next glyph
	};

	class GUI
	{
	public: 
		GUI();
		~GUI() { CleanUp(); }
		void Init();
		void RenderText(Shader* shader, std::string text, float x, float y, float scale, glm::vec3 color);//TODO: iloœæ drewna i iloœæ akcji do koñca tury
		void CleanUp();
	
	private:
		FT_Library ft;
		FT_Face face;
		unsigned int VAO, VBO;
		std::map<char, Character> Characters;
	};
}