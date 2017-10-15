#pragma once

#include "glew.h"
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//reference to https://learnopengl.com/#!Model-Loading/Mesh

//texture information for our models 

struct textureinfo
{
	GLuint textureID;
	std::string textureType;
	aiString path;
};