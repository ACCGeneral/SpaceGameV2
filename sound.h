#pragma once

#include <AL/al.h>             // Provide OpenAL audio buffers and sources
#include <AL/alc.h>            // Handle OpenAL contexts
#include <vorbis/vorbisfile.h> 
#include <glm.hpp>
#include <string>
#include <vector>
#include "common.h"


class sound
{
private:
	ALuint bufferID;
	ALuint sourceID;

public:

	void changevol(float vol);
	void playsound();
	void setpos(glm::vec3 pos);
	void setup(std::string filename);
	void repeatcheck();
	void stopsounds();

	void cleanup();


};