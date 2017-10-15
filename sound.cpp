#include "sound.h"

void sound::changevol(float vol)
{
	alSourcef(sourceID, AL_GAIN, vol);
}

void sound::playsound()
{
	alSourcePlay(sourceID);
}

void sound::setpos(glm::vec3 pos)
{
	alSource3f(sourceID, AL_POSITION, pos.x, pos.y, pos.z);
}

void sound::setup(std::string filename)
{
	alGenBuffers(1, &bufferID);

	ALenum format = 0;
	ALsizei freq = 0;
	std::vector<char> bufferData;

	load_ogg(filename, bufferData, format, freq);

	alBufferData(bufferID, format, &bufferData[0],
		static_cast<ALsizei>(bufferData.size()), freq);

}

void sound::sourcesetup()
{
	alGenSources(1, &sourceID);
	alSourcei(sourceID, AL_BUFFER, bufferID);
}


void sound::stopsounds()
{
	alSourceStop(sourceID);

}

bool sound::isStoped()
{
	ALint state = 0;

	alGetSourcei(sourceID, AL_SOURCE_STATE, &state);

	if (state == AL_STOPPED)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void sound::cleanup()
{
	alDeleteBuffers(1, &bufferID);
	alDeleteSources(1, &sourceID);

}
