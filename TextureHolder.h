#pragma once

#include <memory>
#include <vector>
#include <string>
#include "glew.h"
#include "common.h"

struct texture
{
	GLuint mytexture;
};


class textureHolder // this is the class that will contain all of the models within the game
{
public:

	textureHolder();
	~textureHolder();

	void addtexture(std::string texturePaths, std::string textureName, std::string findName);
	std::shared_ptr<texture> getTexture(std::string name);

private:

	std::vector<std::pair<std::shared_ptr<texture>, std::string>> textures;

};