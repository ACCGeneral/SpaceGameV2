#include "common.h"

bool CheckShaderCompiled(GLint shader)
{
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLsizei len;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

		// OpenGL will store an error message as a string that we can retrieve and print
		GLchar* log = new GLchar[len + 1];
		glGetShaderInfoLog(shader, len, &len, log);
		std::cerr << "ERROR: Shader compilation failed: " << log << std::endl;
		delete[] log;

		return false;
	}
	return true;
}

void load_ogg(std::string fileName, std::vector<char>& buffer, ALenum & format, ALsizei & freq)
{
	int endian = 0;
	int bitStream = 0;
	long bytes = 0;
	char array[2048] = { 0 };
	vorbis_info *pInfo = NULL;
	OggVorbis_File oggFile = { 0 };

	// Use the inbuilt fopen to create a file descriptor
	if (ov_fopen(fileName.c_str(), &oggFile) != 0)
	{
		std::cout << "Failed to open file '" << fileName << "' for decoding" << std::endl;
		abort();
	}

	// Extract information from the file header
	pInfo = ov_info(&oggFile, -1);

	// Record the format required by OpenAL
	if (pInfo->channels == 1)
	{
		format = AL_FORMAT_MONO16;
	}
	else
	{
		format = AL_FORMAT_STEREO16;
	}

	// Record the sample rate required by OpenAL
	freq = pInfo->rate;

	// Keep reading bytes from the file to populate the output buffer
	while (true)
	{
		// Read bytes into temporary array
		bytes = ov_read(&oggFile, array, 2048, endian, 2, 1, &bitStream);

		if (bytes < 0)
		{
			ov_clear(&oggFile);
			std::cout << "Failed to decode file '" << fileName << "'." << std::endl;
			abort();
		}
		else if (bytes == 0)
		{
			break;
		}

		// Copy from temporary array into output buffer
		buffer.insert(buffer.end(), array, array + bytes);
	}

	// Clean up and close the file
	ov_clear(&oggFile);

}


glm::vec3 getrandomwvec3(float minval, float addition)
{
	float x = float(rand() % (RAND_MAX + 1)) / float(RAND_MAX);
	float y = float(rand() % (RAND_MAX + 1)) / float(RAND_MAX);
	float z = float(rand() % (RAND_MAX + 1)) / float(RAND_MAX);

	x = minval + addition*x;
	y = minval + addition*y;
	z = minval + addition*z;

	return glm::vec3(x, y, z);
}

float getrandomwfloat(float minval, float addition)
{
	float x = float(rand() % (RAND_MAX + 1)) / float(RAND_MAX);
	x = minval + addition*x;
	return x;
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