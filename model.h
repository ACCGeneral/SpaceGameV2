#pragma once

#include <vector>
#include "mesh.h"
#include <string>
#include <memory>
#include <SOIL.h>
#include "shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//reference to https://learnopengl.com/#!Model-Loading/Model
//assimp draw data 

GLint TextureFromFile(const char* path, std::string directory);
GLint cubmaploader(std::string directory, std::vector<std::string> loaded_texture);

class model
{
private:

	std::vector<mesh> mymeshes;
	std::string maindirectory;
	std::vector<textureinfo> loaded_texture;
	bool skybox;
	std::vector<std::string> cubtextures; 
	GLenum Cubemap[6];
	GLenum modeldrawtype;

	void processNode(aiNode *node, const aiScene *scene);
	mesh processmesh(aiMesh *mymesh, const aiScene *scene);
	std::vector<textureinfo> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

public:

	model(GLenum drawtype)
	{
		modeldrawtype = drawtype;
		skybox = false;
	}

	void loadmodel(std::string modelpath);
	void draw(std::shared_ptr<shader> myshader);
	void setskybox(bool sky, std::vector<std::string> faces);
	std::vector<mesh> returnmymeshes()
	{
		return mymeshes;
	}


};