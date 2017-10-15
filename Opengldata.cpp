#include "Opengldata.h"

//setup all of the opengl data needed to load a model (buffer VAO, VBO and Index buffer)

OpenglData::OpenglData()
{


}

void OpenglData::genSetup()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

}

void OpenglData::bufferdata(std::vector<vertexinfo> into, GLenum drawtype)
{
	glBufferData(GL_ARRAY_BUFFER, into.size() * sizeof(vertexinfo), &into[0], drawtype);


}

void OpenglData::bindstart()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

}

void OpenglData::Indexbuffersetup(std::vector<int> in, GLenum drawtype)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, in.size() * sizeof(GLuint), &in[0], drawtype);
}

void OpenglData::setupattrib(int index, GLint size, GLsizei stride, const GLvoid * memory)
{
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, memory);
}

void OpenglData::unbind()
{
	glBindVertexArray(0);
}


GLuint OpenglData::returnVAO()
{
	return VAO;
}
