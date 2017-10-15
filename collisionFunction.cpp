#include "collisionFunction.h"

bool rayplanecollion(glm::vec3 raypos, glm::vec3 raydir, glm::vec3 boxmax, glm::vec3 boxmin)
{
	float far = 1.0;
	float near = 0.0;

	glm::vec3 farraypoint = raypos + (raydir * 100.0f);

	for (int i = 0; i < 3; i++)
	{
		float Fnear = (boxmin[i] - raypos[i]) / (farraypoint[i] - raypos[i]);
		float Ffar = (boxmax[i] - raypos[i]) / (farraypoint[i] - raypos[i]);

		if (Ffar < Fnear) // swap the sides around if needed
		{
			float temp = Ffar;
			Ffar = Fnear;
			Fnear = temp;
		}

		far = glm::min(far, Ffar);
		near = glm::max(Fnear, near);

		if (far < near)
		{
			return false;
		}
	}

	return true;
}

glm::vec3 rayplanecollionvec(glm::vec3 raypos, glm::vec3 raydir, glm::vec3 boxmax, glm::vec3 boxmin)
{
	float far = 1.0;
	float near = 0.0;

	glm::vec3 farraypoint = raypos + (raydir * 100.0f);

	for (int i = 0; i < 3; i++)
	{
		float Fnear = (boxmin[i] - raypos[i]) / (farraypoint[i] - raypos[i]);
		float Ffar = (boxmax[i] - raypos[i]) / (farraypoint[i] - raypos[i]);

		if (Ffar < Fnear) // swap the sides around if needed
		{
			float temp = Ffar;
			Ffar = Fnear;
			Fnear = temp;
		}

		far = glm::min(far, Ffar);
		near = glm::max(Fnear, near);

		if (far < near)
		{
			return glm::vec3(0, 0, 0);
		}
	}

	glm::vec3 diffrom = farraypoint - raypos;

	glm::vec3 colpoint = raypos + (diffrom * near); //get the collison point if we need it

	return colpoint;
}
