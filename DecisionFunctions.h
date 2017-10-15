#pragma once

#include "entity.h"
#include "Teamunits.h"
#include "action.h"
#include "fleeaction.h"
#include "bombingrunaction.h"
#include "attackaction.h"
#include "Idelaction.h"
#include "manaction.h"
#include "toenemycapship.h"
#include "turretfireaction.h"
#include "Maneuvers.h"
#include "utilityfunction.h"
#include "usefulFunctions.h"
#include "attackrun.h"


void toenemycap(std::shared_ptr<ACC::entity> me, std::shared_ptr<teamunits> enemyunits, float maxdistance);

void bombingrun(std::shared_ptr<ACC::entity> me, std::shared_ptr<teamunits> enemyunits, maneuver touse, maneuver startuse);

void attackrun(std::shared_ptr<ACC::entity> me, std::shared_ptr<teamunits> enemyunits, maneuver touse, maneuver startuse);

void attackflyer(std::shared_ptr<ACC::entity> me, std::shared_ptr<teamunits> enemyunits, std::shared_ptr<teamunits> allyteam, maneuver touse);

void turretidel(std::shared_ptr<ACC::entity> me, std::shared_ptr<teamunits> enemyunits, float maxdistance);

void turrettarget(std::shared_ptr<ACC::entity> me, std::shared_ptr<teamunits> enemyunits);

bool flyerunittypes(std::shared_ptr<ACC::entity> unit);
