#pragma once

#include <glm.hpp>
#include "AABB.h"
#include "OBB.h"

//overall collison functions that i may need

bool rayplanecollion(glm::vec3 raypos, glm::vec3 raydir, glm::vec3 boxmax, glm::vec3 boxmin);

glm::vec3 rayplanecollionvec(glm::vec3 raypos, glm::vec3 raydir, glm::vec3 boxmax, glm::vec3 boxmin);