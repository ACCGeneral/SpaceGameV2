#include "model.h"

void model::loadmodel(std::string modelpath) //load our model with assimp 
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(modelpath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	maindirectory = modelpath.substr(0, modelpath.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void model::processNode(aiNode *node, const aiScene *scene) //process our assimp model nodes 
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		mymeshes.push_back(processmesh(mesh, scene));
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}

}

mesh model::processmesh(aiMesh *mymesh, const aiScene *scene) //create our meshes from the model
{
	std::vector<vertexinfo> vertices;
	std::vector<int> indices;
	std::vector<textureinfo> textures;

	for (GLuint i = 0; i < mymesh->mNumVertices; i++)
	{
		vertexinfo vertex;
		vertex.vertexpos.x = mymesh->mVertices[i].x;
		vertex.vertexpos.y = mymesh->mVertices[i].y;
		vertex.vertexpos.z = mymesh->mVertices[i].z;
		vertex.vertexnormal.x = mymesh->mNormals[i].x;
		vertex.vertexnormal.y = mymesh->mNormals[i].y;
		vertex.vertexnormal.z = mymesh->mNormals[i].z;

		if (mymesh->mTextureCoords[0])
		{
			vertex.vertextexture.x = mymesh->mTextureCoords[0][i].x;
			vertex.vertextexture.y = mymesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.vertextexture.x = 0.0f;
			vertex.vertextexture.y = 0.0f;
		}

		vertices.push_back(vertex);
	}

	for (GLuint i = 0; i < mymesh->mNumFaces; i++)
	{
		aiFace face = mymesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (skybox == true) //if skybox we need a different setup
	{
		if (cubtextures.size() > 0)
		{
			textureinfo texture;
			texture.textureID = cubmaploader(maindirectory, cubtextures);
			texture.textureType = "cubmap";
			textures.push_back(texture);
		}
	}
	else if (mymesh->mMaterialIndex >= 0) //setup texture data
	{
		aiMaterial* material = scene->mMaterials[mymesh->mMaterialIndex];
		std::vector<textureinfo> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<textureinfo> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}


	return mesh(vertices, indices, textures, modeldrawtype);
}

std::vector<textureinfo> model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName) //load all of our textures 
{
	std::vector<textureinfo> textures;
	
	for (int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		textureinfo texture;
		texture.textureID = TextureFromFile(str.C_Str(), maindirectory);
		texture.textureType = typeName;
		texture.path = str;
		textures.push_back(texture);
	}

	return textures;
}

void model::draw(std::shared_ptr<shader> myshader) //draw our model
{
	if (skybox)
	{
		glDepthMask(GL_FALSE);
		glCullFace(GL_FRONT);
	}

	for (int i = 0; i < mymeshes.size(); i++)
	{
		mymeshes[i].drawme(myshader);
	}

	if (skybox)
	{
		glDepthMask(GL_TRUE);
		glCullFace(GL_BACK);
	}

}

void model::setskybox(bool sky, std::vector<std::string> faces) //setup for skybox 
{
	skybox = sky;
	cubtextures = faces;
}


GLint TextureFromFile(const char* path, std::string directory) //load our textures using SOIL image loader  
{
	//Generate texture ID and load texture data 
	std::string filename = std::string(path);
	filename = directory + '/' + filename;
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);

	printf("SOIL loading error: '%s'\n", SOIL_last_result());

	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}

GLint cubmaploader(std::string directory, std::vector<std::string> loaded_texture) //setup our skybox data 
{
	GLuint textureID;

	GLenum Cubemap[6];
	Cubemap[0] = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	Cubemap[1] = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
	Cubemap[2] = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
	Cubemap[3] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
	Cubemap[4] = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
	Cubemap[5] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	std::string filename;

	for (int i = 0; i < loaded_texture.size(); i++)
	{
		filename = directory + '/' + std::string(loaded_texture[i]);
		int width, height;
		unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

		printf("SOIL loading error: '%s'\n", SOIL_last_result());

		glTexImage2D(Cubemap[i], 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;

}
