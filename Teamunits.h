#pragma once

#include <vector>
#include <memory>
#include "entity.h"

struct teamunits
{
	std::vector<std::shared_ptr<ACC::entity>> allunits;

	std::vector<std::shared_ptr<ACC::entity>> fighters;
	std::vector<std::shared_ptr<ACC::entity>> bomber;
	std::vector<std::shared_ptr<ACC::entity>> turrets;
	std::shared_ptr<ACC::entity> capital;
	std::shared_ptr<ACC::entity> player;

	void removeunit(std::shared_ptr<ACC::entity> toremove);
	

};
