#pragma once
#include "sound.h"
#include <memory>
 

class soundholder
{
private:

	std::vector<std::pair<sound, std::string>> sounds;

public:

	soundholder();
	void addsound(sound newsound, std::string soundname);
	sound getsound(std::string name);

};