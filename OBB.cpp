#include "OBB.h"


bool overlap(projection projA, projection projB, float &collover);


OBB::OBB()
{


}

void OBB::addobbsetup(std::vector<glm::vec3> meshes) //setup for our OBB box
{
	origvertxes = meshes;
}

void OBB::obbsetup(std::vector<mesh> meshes) //setup for our OBB box
{
	glm::vec3 tempmin = glm::vec3(1000, 1000, 1000);
	glm::vec3 tempmax = glm::vec3(-1000, -1000, -1000);

	for (int i = 0; i < meshes.size(); i++)
	{
		for (int j = 0; j < meshes[i].vertices.size(); j++)
		{
			if (tempmin.x > meshes[i].vertices[j].vertexpos.x)
			{
				tempmin.x = meshes[i].vertices[j].vertexpos.x;
			}
			if (tempmin.y > meshes[i].vertices[j].vertexpos.y)
			{
				tempmin.y = meshes[i].vertices[j].vertexpos.y;
			}
			if (tempmin.z > meshes[i].vertices[j].vertexpos.z)
			{
				tempmin.z = meshes[i].vertices[j].vertexpos.z;
			}

			if (tempmax.x <meshes[i].vertices[j].vertexpos.x)
			{
				tempmax.x = meshes[i].vertices[j].vertexpos.x;
			}
			if (tempmax.y < meshes[i].vertices[j].vertexpos.y)
			{
				tempmax.y = meshes[i].vertices[j].vertexpos.y;
			}
			if (tempmax.z < meshes[i].vertices[j].vertexpos.z)
			{
				tempmax.z = meshes[i].vertices[j].vertexpos.z;
			}
		}
	}

	origvertxes.push_back(tempmin);

	origvertxes.push_back(glm::vec3(tempmin.x, tempmin.y, tempmax.z));
	origvertxes.push_back(glm::vec3(tempmin.x, tempmax.y, tempmax.z));

	origvertxes.push_back(glm::vec3(tempmax.x, tempmin.y, tempmin.z));
	origvertxes.push_back(glm::vec3(tempmax.x, tempmax.y, tempmin.z));

	origvertxes.push_back(glm::vec3(tempmax.x, tempmin.y, tempmax.z));
	origvertxes.push_back(glm::vec3(tempmin.x, tempmax.y, tempmin.z));

	origvertxes.push_back(tempmax);

}



void OBB::update(glm::vec3 pos, glm::quat newqu) //update our info
{
	 
	myquad = newqu;
	rotation = glm::toMat3(myquad);

	vertexes.clear();
	vertexes.resize(origvertxes.size());

	center = pos;

	for (int i = 0; i < origvertxes.size(); i++)
	{
		vertexes[i] = myquad * origvertxes[i];
		vertexes[i] += pos;
	}

	

}


glm::mat3 OBB::returnrotation()
  {
    return rotation;

  }

  bool OBB::collisontest2(std::shared_ptr<OBB> B, glm::vec3 &axisofcol, float &collover) //check for collision between 2 OBB's (needs changing)
 { 
	  double addoncheck = 0.00001;

	  glm::mat3 Brotation = B->returnrotation();

	  axes.clear();
	  axes.push_back(rotation[0]);
	  axes.push_back(rotation[1]);
	  axes.push_back(rotation[2]);
	  axes.push_back(Brotation[0]);
      axes.push_back(Brotation[1]);
	  axes.push_back(Brotation[2]);
	  axes.push_back(glm::cross(rotation[0], Brotation[0]));
	  axes.push_back(glm::cross(rotation[0], Brotation[1]));
	  axes.push_back(glm::cross(rotation[0], Brotation[2]));
	  axes.push_back(glm::cross(rotation[1], Brotation[0]));
	  axes.push_back(glm::cross(rotation[1], Brotation[1]));
	  axes.push_back(glm::cross(rotation[1], Brotation[2]));
	  axes.push_back(glm::cross(rotation[2], Brotation[0]));
	  axes.push_back(glm::cross(rotation[2], Brotation[1]));
	  axes.push_back(glm::cross(rotation[2], Brotation[2]));

	  float CO = 1000000000.0f;

	  for (int i = 0; i < axes.size(); i++)
	  {
		  projection myproject = projectonaxes(axes[i]);
		  projection bproject = B->projectonaxes(axes[i]);
		  if (!overlap(myproject, bproject, CO))
		  {
			  return false;
		  }

		  if (CO < collover && CO != 0)
		  {
			  axisofcol = axes[i];
			  collover = CO;
		  }

	  }


	  return true;
  }

  bool OBB::collisontest1(std::shared_ptr<OBB> B)
  {
	  return true;
  }



 

  projection OBB::projectonaxes(glm::vec3 axes) //project each vertex onto the axes of concern
  {
	  projection newproject;

	  float addon = 0.02f;

	  double min = INFINITY;
	  double max = -INFINITY;

	  for (int i = 0; i < vertexes.size(); i++) 
	  {
		  double temp = glm::dot(axes, vertexes[i]);
		  double temp2 = glm::dot(axes, vertexes[i]);
		  if (temp < min)
		  {
			  min = temp - addon;
		  }
		  else if (temp > max)
		  {
			  max = temp + addon;
		  }

	  }

	  newproject.max = max;
	  newproject.min = min;

	  return newproject;
  }


  bool overlap(projection projA, projection projB, float &collover) //projection check
  {
	  if ((projA.max >= projB.min && projA.max <= projB.max))
	  {
		  collover = projA.max - projB.min;
		  return true;
	  }
	  if ((projA.min >= projB.min && projA.min <= projB.max))
	  {
		  collover = projA.min - projB.min;
		  return true;
	  }

	  if ((projB.max >= projA.min && projB.max <= projA.max))
	  {
		  collover = projB.max - projA.min;
		  return true;
	  }
	  if ((projB.min >= projA.min && projB.min <= projA.max))
	  {
		  collover = projB.min - projA.min;
		  return true;
	  }




	  return false;
  }