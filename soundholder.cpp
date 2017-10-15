#include "soundholder.h"

soundholder::soundholder()
{

}

void soundholder::addsound(sound newsound, std::string soundname)
{
	std::pair<sound, std::string> toadd;
	toadd.first = newsound;
	toadd.second = soundname;
	sounds.push_back(toadd);
}

sound soundholder::getsound(std::string name)
{
	for (int i = 0; i < sounds.size(); i++)
	{
		if (sounds[i].second == name)
		{
			return sounds[i].first;
		}
	}


	return sound();
}
