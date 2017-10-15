#pragma once

#include <iostream>
#include <vorbis/vorbisfile.h> 
#include <AL/al.h> 
#include <AL/alc.h>
#include <vector>
#include <string>
#include "glew.h"

//common function to check shader binding

bool CheckShaderCompiled(GLint shader);
//refernce to Leigh Mcloughlin for the code

void load_ogg(std::string fileName, std::vector<char> &buffer,
	ALenum &format, ALsizei &freq);

