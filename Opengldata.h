#pragma once

#include "glew.h"
#include "vertexinfo.h"
#include <vector>

//general opengl data for loading models and loading model data

class OpenglData
{
private:
	GLuint VAO, VBO, EBO;

public:
	OpenglData();

	void genSetup();
	void bufferdata(std::vector<vertexinfo> into, GLenum drawtype);
	void bindstart();
	void Indexbuffersetup(std::vector<int> in, GLenum drawtype);
	void setupattrib(int index, GLint size, GLsizei stride, const GLvoid * memory);
	void unbind();
	GLuint returnVAO();

};