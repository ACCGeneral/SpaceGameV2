#include "TextureHolder.h"

textureHolder::textureHolder()
{
}

textureHolder::~textureHolder()
{
}

void textureHolder::addtexture(std::string texturePaths, std::string textureName, std::string findName)
{
	std::shared_ptr<texture> newTexture = std::make_shared<texture>();
	newTexture->mytexture = TextureFromFile(texturePaths.c_str(), textureName);

	std::pair<std::shared_ptr<texture>, std::string > newpaira;
	newpaira.first = newTexture;
	newpaira.second = findName;
	textures.push_back(newpaira);
}

std::shared_ptr<texture> textureHolder::getTexture(std::string name)
{
	for (int i = 0; i < textures.size(); i++)
	{
		if (textures[i].second == name)
		{
			return textures[i].first;
		}
	}
	return NULL;
}
