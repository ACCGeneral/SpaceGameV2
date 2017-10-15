#include "maincamera.h"
#include "component.h"

maincamera::maincamera()
{
	View = glm::mat4(1.0);
}

void maincamera::setProjection(glm::mat4 &proj)
{
	Projection = proj;
}

void maincamera::setView()
{
	if (playertrans != NULL)
	{
		position = playertrans->position;
		position += direction * 5.3f;
		glm::mat4 translationmatrix = glm::translate(glm::mat4(1.0f), -position);
		glm::mat4 RotationMatrix = glm::toMat4(playertrans->myquat);
		glm::mat4 tempmat = glm::axisAngleMatrix(glm::vec3(0, 1, 0), glm::radians(180.0f));
		RotationMatrix = RotationMatrix * tempmat;
		RotationMatrix = glm::inverse(RotationMatrix);
		View = RotationMatrix * translationmatrix;
	}
	
}

void maincamera::setView(glm::mat4 &setview) //personalsetup for cam
{
	View = setview;
}

void maincamera::setposition(glm::vec3 campos)
{
	position = campos;
}

void maincamera::setdirection(glm::vec3 dir)
{
	direction = dir;

}
