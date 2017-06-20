#pragma once
#include "GLSLProgram.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H
using namespace std;

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;    // Horizontal offset to advance to next glyph
};
class FreeTypeFont{
	private:
		map<GLchar, Character> Characters;
		GLuint VAO_font, VBO_font;
		int SCREEN_WIDTH;
		int SCREEN_HEIGHT;
	public:
		FreeTypeFont();
		FreeTypeFont(int SCREEN_WIDTH, int SCREEN_HEIGHT,CGLSLProgram *glslFont);
		~FreeTypeFont();
		int print(CGLSLProgram *shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

};
