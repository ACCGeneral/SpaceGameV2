#include "action.h"
#include "entity.h"


void action::addme(std::shared_ptr<ACC::entity> addme)
{
	me = addme;
}
