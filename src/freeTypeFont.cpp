#include "freeTypeFont.h"

FreeTypeFont::FreeTypeFont(){}
FreeTypeFont::FreeTypeFont(int SCREEN_WIDTH, int SCREEN_HEIGHT,CGLSLProgram *glslFont){
	// Set OpenGL options
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Compile and setup the shader
    //Shader shader("shaders/text.vs", "shaders/text.frag");


    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(SCREEN_WIDTH), 0.0f, static_cast<GLfloat>(SCREEN_HEIGHT));
    //shader.Use();
	glslFont->enable();
		glslFont->loadUniformMatrix("projection",				GShader::UNIFORM_SIZE_4D, glm::value_ptr(projection));
    glslFont->disable();
	//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // FreeType
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
		printf("Error, la biblioteca FreeType no se pudo inicializar\n");

    // Load font as face
    FT_Face face;
	if (FT_New_Face(ft, "../models/COOPBL.TTF", 0, &face))	
		printf("Error, el tipo de letra de FreeType no pudo cargar");

    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

    // Load first 128 characters of ASCII set
    for (GLubyte c = 0; c < 128; c++){
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)){
            printf("Error, no se pudo cargar el Glyph de FreeType");
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    
    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &VAO_font);
    glGenBuffers(1, &VBO_font);
    glBindVertexArray(VAO_font);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_font);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
FreeTypeFont::~FreeTypeFont(){

}
int FreeTypeFont::print(CGLSLProgram *shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	GLfloat xpos;
    GLfloat ypos;
	GLfloat w;
    GLfloat h;

    // Activate corresponding render state	
    //shader.Use();
	shader->enable();
    
	float color2[] = {color.x, color.y, color.z};
	shader->loadUniformf("textColor",				GShader::UNIFORM_SIZE_3D, color2);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO_font);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) 
    {
        Character ch = Characters[*c];

        xpos = x + ch.Bearing.x * scale;
        ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        w = ch.Size.x * scale;
        h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },            
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }           
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO_font);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
	shader->disable();

	return (xpos + w);
}
