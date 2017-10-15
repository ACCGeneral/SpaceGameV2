#pragma once

//reference to https://learnopengl.com/#!Model-Loading/Mesh
#include "vertexinfo.h"
#include "texture.h"
#include <glm.hpp>
#include "glew.h"
#include <vector>
#include "shader.h"
#include "Opengldata.h"
#include "shader.h"
#include <sstream>
#include <memory>

//mesh class for assimp model

class mesh
{

private:
	
	OpenglData mygldata;

	void modelsetup(GLenum drawtype);

public:

	std::vector<vertexinfo> vertices;
	std::vector<int> indices;
	std::vector<textureinfo> textures;

	mesh(std::vector<vertexinfo> ver, std::vector<int> in, std::vector<textureinfo> tex, GLenum drawtype);
	void drawme(std::shared_ptr<shader> myshader);
};