#include "TextManager.h"


TextManager & TextManager::instance()
{
	static TextManager myinstance;

	return myinstance;
}

TextManager::TextManager()
{



}

void TextManager::rendertext(std::string texttoren, glm::vec3 colour, GLfloat x, GLfloat y, float scale)
{
	// Activate corresponding render state	
	textsharder->useProgram();
	glUniform3f(glGetUniformLocation(textsharder->returnprogram(), "textcolour"), colour.x, colour.y, colour.z);
	glUniformMatrix4fv(glGetUniformLocation(textsharder->returnprogram(), "projection"), 1, GL_FALSE, glm::value_ptr(projectionmatrix));
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(charactorVAO);

	// Iterate through all characters
	std::string::const_iterator i;
	for (i = texttoren.begin(); i != texttoren.end(); i++)
	{
		Character ch = charactormap[*i];

		GLfloat xpos = x + ch.offset.x * scale;
		GLfloat ypos = y - (ch.glyphsize.y - ch.offset.y) * scale;

		GLfloat w = ch.glyphsize.x * scale;
		GLfloat h = ch.glyphsize.y * scale;
		//create our charactor 
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		glBindTexture(GL_TEXTURE_2D, ch.charactorID);
		glBindBuffer(GL_ARRAY_BUFFER, charactorVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6); // render our text
		x += (ch.nextcharactor >> 6) * scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);


}


void TextManager::setup() //setup so we can render charactors
{
	charactorVAO = -1;
	charactorVBO = -1;

	GLint dimentions[4];
	glGetIntegerv(GL_VIEWPORT, dimentions);
	projectionmatrix = glm::ortho(0.0f, (float)dimentions[2], 0.0f, (float)dimentions[3]);

	if (FT_Init_FreeType(&libraryload))
	{
		std::cout << "The FreeType library has failed to load \n";
		return;
	}

	if (FT_New_Face(libraryload, "arial.ttf", 0, &fontload))
	{
		std::cout << "Failed to load the arial font \n";
		return;
	}

	FT_Set_Pixel_Sizes(fontload, 0, 48); //set the size for our charactors 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //no more byte alignment (needed for freetype)


	// Load first 128 characters of ASCII set
	for (GLubyte i = 0; i < 128; i++)
	{
		// Load character glyph 
		if (FT_Load_Char(fontload, i, FT_LOAD_RENDER))
		{
			std::cout << "Could not load charactor number" << i << "\n";
			return;
		}
		//create a texture for our charactor
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, fontload->glyph->bitmap.width, fontload->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, fontload->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); //texture paramaters and settings 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character newcharactor;
		newcharactor.charactorID = texture;
		newcharactor.glyphsize = glm::ivec2(fontload->glyph->bitmap.width, fontload->glyph->bitmap.rows);
		newcharactor.offset = glm::ivec2(fontload->glyph->bitmap_left, fontload->glyph->bitmap_top);
		newcharactor.nextcharactor = fontload->glyph->advance.x;
		charactormap[i] = newcharactor;
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	//cleanup
	FT_Done_Face(fontload);
	FT_Done_FreeType(libraryload);

	//our charactor model setup
	glGenVertexArrays(1, &charactorVAO);
	glGenBuffers(1, &charactorVBO);
	glBindVertexArray(charactorVAO);
	glBindBuffer(GL_ARRAY_BUFFER, charactorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


}
