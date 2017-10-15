#include "modelmatrixsystem.h"

modelmatrixsystem::modelmatrixsystem(std::shared_ptr<world> w) : System(w)
{
	my_System_Name = "modelmatrixsystem";

}

void modelmatrixsystem::soiwant(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	for (int i = 0; i < ent.size(); i++)
	{
		if (ent[i]->doihave(transposecomponent::TypeID))
		{
			transformEnts.push_back(ent[i]);
		}
	}


}

void modelmatrixsystem::update(float & dt, bool & go)  //update each ents model matrix
{
	for (auto num : transformEnts)
	{
		std::shared_ptr<transposecomponent> entitytrans = num->getcomponent<transposecomponent>();

		glm::mat4 TMatrix = glm::translate(glm::mat4(1.0), entitytrans->position);
		glm::mat4 Rmatrix = glm::toMat4(entitytrans->myquat);
		glm::mat4 Smatrix = glm::scale(glm::mat4(1.0), entitytrans->scale);

		entitytrans->modelMatrix = TMatrix * Rmatrix * Smatrix; //create the model matrix
		
	}
}

void modelmatrixsystem::doihave(std::vector<std::shared_ptr<ACC::entity>> ent)
{
	std::vector<std::shared_ptr<ACC::entity>> ents = ent;

	for (int i = 0; i < transformEnts.size(); i++)
	{
		for (int j = 0; j < ents.size(); j++)
		{
			if (transformEnts[i] == ents[j])
			{
				transformEnts.erase(transformEnts.begin() + i);
				ents.erase(ents.begin() + j);
				j--;
				i--;
				break;
			}
		}

		if (ents.size() == 0)
		{
			break;
		}
	}

}
