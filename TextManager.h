#pragma once
#include <string>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "glew.h"
#include "shader.h"
#include <map>
#include <memory>


//reference to https://learnopengl.com/#!In-Practice/Text-Rendering

//class that is responsible for rendering text that we want


// Holds all state information relevant to a character as loaded using FreeType
struct Character 
{
	GLuint charactorID;
	glm::ivec2 glyphsize;
	glm::ivec2 offset;
	GLuint nextcharactor;
};


class TextManager
{
private:

	FT_Library libraryload;
	FT_Face fontload;
	GLuint charactorVAO;
	GLuint charactorVBO;
	glm::mat4 projectionmatrix;
	std::shared_ptr<shader> textsharder;
	std::map<char, Character> charactormap;

public:

	static TextManager &instance();
	TextManager();
	void rendertext(std::string texttoren, glm::vec3 colour, GLfloat x, GLfloat y, float scale);
	void setup();

	void setshader(std::shared_ptr<shader> mysha)
	{
		textsharder = mysha;
	}

	void releaseshader()
	{
		textsharder.reset();
	}

};