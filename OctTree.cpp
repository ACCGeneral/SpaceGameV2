#include "OctTree.h"

bool insidecheck(glm::vec3 octmin, glm::vec3 octmax, glm::vec3 rayorigin)
{
	if (rayorigin.x >= octmin.x && rayorigin.x <= octmax.x)
	{
		if (rayorigin.y >= octmin.y && rayorigin.y <= octmax.y)
		{
			if (rayorigin.z >= octmin.z && rayorigin.z <= octmax.z)
			{
				return true;
			}
		}
	}

	return false;

}

bool IDcheck(Octinfo Octone, Octinfo Octtwo)
{
	if (Octone.unitID != Octtwo.unitID)
	{
		for (int i = 0; i < Octone.cantcollide.size(); i++)
		{
			if (Octone.cantcollide[i] == Octtwo.unitID)
			{
				return false;
			}
		}

		for (int i = 0; i < Octtwo.cantcollide.size(); i++)
		{
			if (Octtwo.cantcollide[i] == Octone.unitID)
			{
				return false;
			}
		}

		return true;
	}

	return false;

}

void OctTree::construct(OctTree *parent, int divides) //create our quadtree with the number of divides we want
{
	root = parent;

	if (parent == NULL)
	{
		mycols = std::make_shared<collison>();
	}

	if (divides != 0)
	{
		divides--;
		child1 = new OctTree(glm::vec3(octmin.x, middle.y, octmin.z), glm::vec3(middle.x, octmax.y, middle.z));
		child1->setcoldatabase(mycols);
		child1->construct(this, divides);
		child2 = new OctTree(glm::vec3(middle.x, middle.y, octmin.z), glm::vec3(octmax.x, octmax.y, middle.z));
		child2->setcoldatabase(mycols);
		child2->construct(this, divides);
		child3 = new OctTree(octmin, middle);
		child3->setcoldatabase(mycols);
		child3->construct(this, divides);
		child4 = new OctTree(glm::vec3(middle.x, octmin.y, octmin.z), glm::vec3(octmax.x, middle.y, middle.z));
		child4->setcoldatabase(mycols);
		child4->construct(this, divides);
		child5 = new OctTree(glm::vec3(octmin.x, middle.y, middle.z), glm::vec3(middle.x, octmax.y, octmax.z));
		child5->setcoldatabase(mycols);
		child5->construct(this, divides);
		child6 = new OctTree(middle, octmax);
		child6->setcoldatabase(mycols);
		child6->construct(this, divides);
		child7 = new OctTree(glm::vec3(octmin.x, octmin.y, middle.z), glm::vec3(middle.x, middle.y, octmax.z));
		child7->setcoldatabase(mycols);
		child7->construct(this, divides);
		child8 = new OctTree(glm::vec3(middle.x, octmin.y, middle.z), glm::vec3(octmax.x, middle.y, octmax.z));
		child8->setcoldatabase(mycols);
		child8->construct(this, divides);
	}

}

void OctTree::removeentity(int ID) //remove unneeded ent
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i].unitID == ID)
		{
			objects.erase(objects.begin() + i);
			break;
		}
	}

}


void OctTree::getparententities(std::vector<Octinfo> &returnents) //get all of our parents col datas 
{
	if (root != NULL)
	{
		root->getparententities(returnents);
	}

	if (objects.size() == 0)
	{
		return;
	}
	else
	{
		returnents.insert(returnents.begin(), objects.begin(), objects.end());
	}
}

void OctTree::collisiondetection() //now check for all collisions on this current frame and store them into the colliison class
{
	if (objects.size() != 0)
	{
		std::vector<Octinfo> entstocheck = objects;
		std::vector<Octinfo> parentent;
		if (root != NULL)
		{
			root->getparententities(parentent);
		}

		entstocheck.insert(entstocheck.end(), parentent.begin(), parentent.end());

		for (int i = 0; i < objects.size(); i++)
		{
			std::shared_ptr<AABB> myaabb = objects[i].objectaabb;
			
			for (int j = i + 1; j < entstocheck.size(); j++)
			{
				int id1 = objects[i].unitID;
				int id2 = entstocheck[j].unitID;

				if (IDcheck(objects[i], entstocheck[j]) && !(objects[i].cantmove == true && entstocheck[j].cantmove == true) && !(objects[i].bullettype == true && entstocheck[j].bullettype == true))
				{
					std::shared_ptr<AABB> otheraabb = entstocheck[j].objectaabb;
		
					if (myaabb->collision(otheraabb))
					{	
						std::vector<std::shared_ptr<OBB>> myobb = objects[i].objectobb;
						std::vector<std::shared_ptr<OBB>> otherobb = entstocheck[j].objectobb;

						bool hascollided = false;

						for (int n = 0; n < myobb.size() && hascollided == false; n++)
						{
							for (int m = 0; m < otherobb.size() && hascollided == false; m++)
							{
								glm::vec3 axisofcol;
								float overlapofcol = 100000000.0f;
								if (myobb[n]->collisontest2(otherobb[m], axisofcol, overlapofcol))
								{
									hascollided = true;
									collisioninfo newcol;
									newcol.ID1 = objects[i].unitID;
									newcol.ID2 = entstocheck[j].unitID;
									newcol.projectionaxis = axisofcol;

									glm::vec3 oneside = myobb[n]->returncenter() + (axisofcol * overlapofcol);
									glm::vec3 otherside = myobb[n]->returncenter() + (axisofcol * -overlapofcol);
									if (glm::length(oneside - otherobb[m]->returncenter()) < glm::length(otherside - otherobb[m]->returncenter()))
									{
										newcol.overlap = -overlapofcol;
									}
									else
									{
										newcol.overlap = overlapofcol;
									}

									mycols->addcol(newcol);
								}
							}
						}
					}
				}

			}
		}
	}
	
}

bool OctTree::fitinoct(glm::vec3 objmin, glm::vec3 objmax, OctTree* lanode) //check if an entity still fits in this node
{
	if (objmin.x > lanode->octmin.x && objmin.x < lanode->octmax.x && objmax.x > lanode->octmin.x && objmax.x < lanode->octmax.x)
	{
		if (objmin.y > lanode->octmin.y && objmin.y < lanode->octmax.y && objmax.y > lanode->octmin.y && objmax.y < lanode->octmax.y)
		{
			if (objmin.z > lanode->octmin.z && objmin.z < lanode->octmax.z && objmax.z > lanode->octmin.z && objmax.z < lanode->octmax.z)
			{
				return true;
			}
		}
		
	}

	return false;
}

void OctTree::update() //update all our collison data then check for collisons
{

	checkendts();
	if (child1 != NULL)
	{
		child1->checkendts();
		child2->checkendts();
		child3->checkendts();
		child4->checkendts();
		child5->checkendts();
		child6->checkendts();
		child7->checkendts();
		child8->checkendts();
	}

	collisiondetection();

	if (child1 != NULL)
	{
		child1->update();
		child2->update();
		child3->update();
		child4->update();
		child5->update();
		child6->update();
		child7->update();
		child8->update();
	}

}

void OctTree::addobject(Octinfo obj) //add an object
{
	if (root != NULL && !fitinoct(obj.objectaabb->newmin, obj.objectaabb->newmax, this))
	{
		root->addobject(obj);
	}
	else if (child1 == NULL)
	{
		objects.push_back(obj);
		return;
	}
	else if (fitinoct(obj.objectaabb->newmin, obj.objectaabb->newmax, child1))
	{
		child1->addobject(obj);
		return;
	}
	else if (fitinoct(obj.objectaabb->newmin, obj.objectaabb->newmax, child2))
	{
		child2->addobject(obj);
		return;
	}
	else if (fitinoct(obj.objectaabb->newmin, obj.objectaabb->newmax, child3))
	{
		child3->addobject(obj);
		return;
	}
	else if (fitinoct(obj.objectaabb->newmin, obj.objectaabb->newmax, child4))
	{
		child4->addobject(obj);
		return;
	}
	else if (fitinoct(obj.objectaabb->newmin, obj.objectaabb->newmax, child5))
	{
		child5->addobject(obj);
		return;
	}
	else if (fitinoct(obj.objectaabb->newmin, obj.objectaabb->newmax, child6))
	{
		child6->addobject(obj);
		return;
	}
	else if (fitinoct(obj.objectaabb->newmin, obj.objectaabb->newmax, child7))
	{
		child7->addobject(obj);
		return;
	}
	else if (fitinoct(obj.objectaabb->newmin, obj.objectaabb->newmax, child8))
	{
		child8->addobject(obj);
		return;
	}
	else
	{
		objects.push_back(obj);
		return;
	}


}

void OctTree::checkendts() // move entities around if neeeded
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i].cantmove != true)
		{
			Octinfo obj = objects[i];

			if (root != NULL && !fitinoct(obj.objectaabb->newmin, obj.objectaabb->newmax, this))
			{
				objects.erase(objects.begin() + i);
				i--;
				root->addobject(obj);
			}
			else if (child1 == NULL)
			{

			}
			else if (fitinoct(obj.objectaabb->newmin, obj.objectaabb->newmax, child1))
			{
				objects.erase(objects.begin() + i);
				i--;
				child1->addobject(obj);
			}
			else if (fitinoct(obj.objectaabb->newmin, obj.objectaabb->newmax, child2))
			{
				objects.erase(objects.begin() + i);
				i--;
				child2->addobject(obj);
			}
			else if (fitinoct(obj.objectaabb->newmin, obj.objectaabb->newmax, child3))
			{
				objects.erase(objects.begin() + i);
				i--;
				child3->addobject(obj);
			}
			else if (fitinoct(obj.objectaabb->newmin, obj.objectaabb->newmax, child4))
			{
				objects.erase(objects.begin() + i);
				i--;
				child4->addobject(obj);
			}
			else if (fitinoct(obj.objectaabb->newmin, obj.objectaabb->newmax, child5))
			{
				objects.erase(objects.begin() + i);
				i--;
				child5->addobject(obj);
			}
			else if (fitinoct(obj.objectaabb->newmin, obj.objectaabb->newmax, child6))
			{
				objects.erase(objects.begin() + i);
				i--;
				child6->addobject(obj);
			}
			else if (fitinoct(obj.objectaabb->newmin, obj.objectaabb->newmax, child7))
			{
				objects.erase(objects.begin() + i);
				i--;
				child7->addobject(obj);
			}
			else if (fitinoct(obj.objectaabb->newmin, obj.objectaabb->newmax, child8))
			{
				objects.erase(objects.begin() + i);
				i--;
				child8->addobject(obj);
			}
		
		}
	}


}

void OctTree::cleanup() //clean up our quad
{
	if (child1 != NULL)
	{
		child1->cleanup();
		child2->cleanup();
		child3->cleanup();
		child4->cleanup();
		child5->cleanup();
		child6->cleanup();
		child7->cleanup();
		child8->cleanup();

		delete child1;
		delete child2;
		delete child3;
		delete child4;
		delete child5;
		delete child6;
		delete child7;
		delete child8;
	}

}

void OctTree::removethis(unsigned long int id, bool &found) //remove an entity from our quad tree
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i].unitID == id)
		{
			objects.erase(objects.begin() + i);
			found = true;
			return;
		}
	}

	if (child1 != NULL)
	{
		child1->removethis(id,found);
		if (found)
		{
			return;
		}
		child2->removethis(id, found);
		if (found)
		{
			return;
		}
		child3->removethis(id, found);
		if (found)
		{
			return;
		}
		child4->removethis(id, found);
		if (found)
		{
			return;
		}
		child5->removethis(id, found);
		if (found)
		{
			return;
		}
		child6->removethis(id, found);
		if (found)
		{
			return;
		}
		child7->removethis(id, found);
		if (found)
		{
			return;
		}
		child8->removethis(id, found);
		if (found)
		{
			return;
		}
	}

}

void OctTree::getclosesttoray(Ray myray, std::vector<int> idstomiss, float maxdis)
{

	if (root == NULL)
	{
		mycols->resetraycol();
	}

	AABB newaabb;
	newaabb.newmin = octmin;
	newaabb.newmax = octmax;
	
	if (objects.size() > 0)
	{
		int test = 1;
	}


	float throwaway;

	if (insidecheck(octmin,octmax,myray.pos) || newaabb.raycollision(myray, throwaway))
	{
		float t = 1000000;

		for (int i = 0; i < objects.size(); i++)
		{
			float distancefrom = glm::length(objects[i].objectaabb->mypos - myray.pos);
			if (distancefrom <= (maxdis + objects[i].objectsphere->rad) && objects[i].physicsobj)
			{
				bool IDC = true;

				for (int j = 0; j < idstomiss.size(); j++)
				{
					if (objects[i].unitID == idstomiss[j])
					{
						IDC = false;
						break;
					}
				}

				if (IDC && objects[i].objectsphere->raycollision(myray, t))
				{
					glm::vec3 hitp = myray.pos + myray.dir * t;
					mycols->setraycol(hitp, objects[i].unitID, t, objects[i].objectsphere->newpos);
				}
			}
		}


		if (child1 != NULL) 
		{
			child1->getclosesttoray(myray, idstomiss, maxdis);
			child2->getclosesttoray(myray, idstomiss, maxdis);
			child3->getclosesttoray(myray, idstomiss, maxdis);
			child4->getclosesttoray(myray, idstomiss, maxdis);
			child5->getclosesttoray(myray, idstomiss, maxdis);
			child6->getclosesttoray(myray, idstomiss, maxdis);
			child7->getclosesttoray(myray, idstomiss, maxdis);
			child8->getclosesttoray(myray, idstomiss, maxdis);
		}
	}

}

void OctTree::getclosetome(glm::vec3 pos, float rad)
{

	if (root == NULL)
	{
		mycols->clearnearme();
	}

	if (insidecheck(octmin, octmax, pos) || glm::length(pos - middle) <= rad)
	{
		for (int i = 0; i < objects.size(); i++)
		{
			glm::vec3 vectorbetween = pos - objects[i].objectaabb->mypos;

			if (glm::length(vectorbetween) <= rad && objects[i].physicsobj)
			{
				mycols->addidnear(objects[i].unitID);
			}
		}


		if (child1 != NULL)
		{
			child1->getclosetome(pos, rad);
			child2->getclosetome(pos, rad);
			child3->getclosetome(pos, rad);
			child4->getclosetome(pos, rad);
			child5->getclosetome(pos, rad);
			child6->getclosetome(pos, rad);
			child7->getclosetome(pos, rad);
			child8->getclosetome(pos, rad);
		}
	}

}

bool OctTree::somethingnearme(glm::vec3 pos, float maxlenght)
{
	if (insidecheck(octmin, octmax, pos) || glm::length(pos - middle) <= maxlenght)
	{
		for (int i = 0; i < objects.size(); i++)
		{
			glm::vec3 vectorbetween = pos - objects[i].objectaabb->mypos;

			if (glm::length(vectorbetween) <= maxlenght && objects[i].physicsobj)
			{
				return true;
			}
		}

		if (child1 != NULL)
		{
			child1->somethingnearme(pos, maxlenght);
			child2->somethingnearme(pos, maxlenght);
			child3->somethingnearme(pos, maxlenght);
			child4->somethingnearme(pos, maxlenght);
			child5->somethingnearme(pos, maxlenght);
			child6->somethingnearme(pos, maxlenght);
			child7->somethingnearme(pos, maxlenght);
			child8->somethingnearme(pos, maxlenght);
		}
	}

	if (root == NULL)
	{
		return 0;
	}

}




